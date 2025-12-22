#include "Game.h"

bool Game::initializeSDL() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "[ERROR] SDL_Init failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        return false;
    }

    if (!TTF_Init()) {
        std::cerr << "[ERROR] TTF_Init failed\n";
        SDL_Quit();
        return false;
    }

    const char* fontPaths[] =
    {
        "C:\\Windows\\Fonts\\arial.ttf",
        NULL
    };

    for (int i = 0; fontPaths[i] != NULL; ++i) {
        this->font = TTF_OpenFont(fontPaths[i], 24);
        if (this->font) {
            std::cout << "[INFO] Police chargee depuis : " << fontPaths[i] << "\n";
            break;
        }
        else {
            std::cerr << "[WARNING] Echec de chargement depuis : " << fontPaths[i] << " (" << SDL_GetError() << ")\n";
        }
    }

    return true;
}

bool Game::CreateWindowAndRenderer(SDL_Window*& window, SDL_Renderer*& renderer) {
    if (!SDL_CreateWindowAndRenderer("AeroBlade", 800, 600, SDL_WINDOW_FULLSCREEN, &window, &renderer)) {
        std::cerr << "[ERROR] SDL_CreateWindowAndRenderer failed: " << SDL_GetError() << "\n";
        return false;
    }

    if (SDL_SetRenderVSync(renderer, 1)) {
        std::cerr << "[WARNING] VSync failed: " << SDL_GetError() << "\n";
    }

    int windowWidth, windowHeight;
    if (!SDL_GetWindowSize(window, &windowWidth, &windowHeight)) {
        std::cerr << "[ERROR] Failed to get window size: " << SDL_GetError() << "\n";
        return false;
    }
    std::cout << "[INFO] Window size: " << windowWidth << "x" << windowHeight << "\n";

    this->screenWidth = windowWidth;
    this->screenHeight = windowHeight;
    std::cout << "[INFO] Game configured for resolution: " << screenWidth << "x" << screenHeight << "\n";
    return true;
}

void Game::loadLevel(int index) {
    if (index < 0 || index >= (int)levelsOrder.size()) {
        std::cerr << "[ERROR] Invalid level index: " << index << "\n";
        return;
    }

    currentLevel = std::make_unique<LevelBase>(font, screenWidth, screenHeight);
    if (!currentLevel->loadFromFile(levelsOrder[index], renderer)) {
        std::cerr << "[ERROR] Failed to load level: " << levelsOrder[index] << "\n";
        currentLevel = nullptr;
    }
    else {
        std::cout << "[INFO] Level loaded: " << levelsOrder[index] << "\n";
    }
}

void Game::handleMenuEvent(const SDL_Event& event, bool& shouldSwitchToCustom) {
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        std::cout << "[DEBUG] Mouse click at: " << event.button.x << ", " << event.button.y << "\n";
        std::cout << "[DEBUG] Start button: x=" << start->startButton.rect.x
            << " y=" << start->startButton.rect.y
            << " w=" << start->startButton.rect.w
            << " h=" << start->startButton.rect.h << "\n";
        std::cout << "[DEBUG] Leave button: x=" << start->leaveButton.rect.x
            << " y=" << start->leaveButton.rect.y
            << " w=" << start->leaveButton.rect.w
            << " h=" << start->leaveButton.rect.h << "\n";
    }

    SDL_Event ev = event;

    handleButtonEvent(&start->startButton, &ev);
    handleButtonEvent(&start->leaveButton, &ev);

    if (isButtonClicked(&start->startButton, &ev)) {
        std::cout << "[INFO] Start button clicked!\n";
        shouldSwitchToCustom = true;
    }

    if (isButtonClicked(&start->leaveButton, &ev)) {
        std::cout << "[INFO] Leave button clicked!\n";
        shouldQuit = true;
    }
}

void Game::drawMenu(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColorFloat(renderer, 0.0f, 0.5f, 1.0f, 1.0f);
    SDL_RenderFillRect(renderer, nullptr);

    renderButton(renderer, &start->startButton, font);
    renderButton(renderer, &start->leaveButton, font);

    std::string totalScoreText = "Total Score: " + std::to_string(totalScore);
    SDL_RenderDebugText(renderer, 700, 10, totalScoreText.c_str());
}

int Game::run() {
    SDL_Window* window = nullptr;

    SDL_SetAppMetadata("AeroBlade", "1.0", "games.anakata.test-sdl");

    if (!this->initializeSDL()) {
        SDL_Quit();
        return 1;
    }

    if (!CreateWindowAndRenderer(window, this->renderer)) {
        SDL_Quit();
        return 1;
    }

    custom = new Custom(window, this->font);
    select = new Select(window, this->font);
    start = new Start();

    if (!LevelLoader::loadLevelsOrder("Levels_order.txt", levelsOrder)) {
        std::cerr << "[ERROR] Failed to load levels order\n";
        delete custom;
        delete select;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_StartTextInput(window);
    std::cout << "[INFO] Text input started\n";

    bool keepGoing = true;
    Uint64 lastTime = SDL_GetTicks();
    currentLevelIndex = 0;
    currentState = State::MENU;

    do {
        Uint64 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                keepGoing = false;
            }

            if (currentState == State::MENU) {
                bool shouldSwitch = false;
                handleMenuEvent(event, shouldSwitch);

                if (shouldQuit) {
                    keepGoing = false;
                }

                if (shouldSwitch) {
                    currentState = State::CUSTOM;
                }
            }
            else if (currentState == State::CUSTOM) {
                bool shouldSwitch = false;
                custom->handleEvent(event, shouldSwitch);
                if (shouldSwitch) {
                    currentState = State::SELECT;
                }
            }
            else if (currentState == State::SELECT) {
                int selectedLevel = 0;
                select->handleEvent(event, selectedLevel);
                if (selectedLevel >= 1 && selectedLevel <= (int)levelsOrder.size()) {
                    currentLevelIndex = selectedLevel - 1;
                    loadLevel(currentLevelIndex);
                    if (currentLevel) {
                        currentState = State::LEVEL;
                    }
                }
            }
            else if (currentState == State::LEVEL && currentLevel) {
                bool shouldSwitch = false;
                currentLevel->handleEvent(event, shouldSwitch);
                if (shouldSwitch) {
                    currentLevel = nullptr;
                    currentState = State::MENU;
                }
            }
        }

        if (currentState == State::CUSTOM) {
            custom->update();
        }
        else if (currentState == State::LEVEL && currentLevel) {
            currentLevel->update(deltaTime,renderer);

            if (currentLevel->isCompleted()) {
                std::cout << "[INFO] Level completed!\n";

                if (currentLevelIndex + 1 < (int)levelsOrder.size()) {
                    select->showWorldTransition(currentLevelIndex + 1);
                    currentLevel = nullptr;
                    currentState = State::SELECT;
                }
                else {
                    std::cout << "[INFO] All levels completed!\n";
                    currentLevel = nullptr;
                    currentState = State::MENU;
                }
            }
            else if (currentLevel->isFailed()) {
                select ->showWorldGameOver(currentLevelIndex + 1);
                std::cout << "[INFO] Level failed!\n" << "[INFO] Score for this run : " << totalScore << "\n";
                totalScore = 0;
                currentLevel = nullptr;
                currentState = State::SELECT;
            }
        }

        SDL_SetRenderDrawColorFloat(renderer, 0.0f, 0.0f, 0.0f, 1.0f);
        SDL_RenderClear(renderer);

        if (currentState == State::MENU) {
            drawMenu(renderer);
        }
        else if (currentState == State::CUSTOM) {
            custom->draw(renderer);
        }
        else if (currentState == State::SELECT) {
            select->draw(renderer);
        }
        else if (currentState == State::LEVEL && currentLevel) {
            currentLevel->draw(renderer);
        }

        SDL_RenderPresent(renderer);
    } while (keepGoing);

    delete custom;
    delete select;

    if (this->font) {
        TTF_CloseFont(this->font);
        this->font = nullptr;
    }

    SDL_StopTextInput(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
