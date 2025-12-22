#include "Select.h"

Select::Select(SDL_Window* window, TTF_Font* font) : font(font), inTransition(false), inGameOver(false) {
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    margin = 20;
    button1Width = windowWidth * 0.45;
    button1Height = windowHeight*0.4;
    button1X = margin;
    button1Y = margin;
    buttonLevel1 = createButton(button1X, button1Y, button1Width, button1Height, "Level 1");

    button2Width = windowWidth * 0.45;
    button2Height = windowHeight * 0.4;
    button2X = windowWidth - button2Width - margin;
    button2Y = margin;
    buttonLevel2 = createButton(button2X, button2Y, button2Width, button2Height, "Level 2");

    button3Width = windowWidth * 0.45;
    button3Height = windowHeight * 0.4;
    button3X = margin;
    button3Y = button1Height + margin * 2;
    buttonLevel3 = createButton(button3X, button3Y, button3Width, button3Height, "Level 3");

    button4Width = windowWidth * 0.45;
    button4Height = windowHeight * 0.4;
    button4X = windowWidth - button4Width - margin;
    button4Y = button2Height + margin*2;
    buttonLevel4 = createButton(button4X, button4Y, button4Width, button4Height, "Level 4");

    transitionButtonWidth = windowHeight * 0.3;
    transitionButtonHeight = windowHeight * 0.1;
    spacingBetweenButtons = windowHeight * 0.5;

    centerX = windowWidth / 2;
    centerY = windowHeight / 2;

    menuButtonX = centerX - transitionButtonWidth / 2;
    menuButtonY = centerY - transitionButtonHeight - spacingBetweenButtons / 2;
    buttonBackToMenu = createButton(menuButtonX, menuButtonY, transitionButtonWidth, transitionButtonHeight, "Menu");

    nextButtonX = centerX - transitionButtonWidth / 2;
    nextButtonY = centerY + spacingBetweenButtons / 2;
    buttonNextWorld = createButton(nextButtonX, nextButtonY, transitionButtonWidth, transitionButtonHeight, "Next World");

    GameOverButtonWidth = windowHeight * 0.3;
    GameOverButtonHeight = windowHeight * 0.1;

    centerX = windowWidth / 2;
    centerY = windowHeight / 2;

    menuButtonX = centerX - GameOverButtonWidth / 2;
    menuButtonY = centerY - GameOverButtonHeight / 2;
    buttonBackToMenu = createButton(menuButtonX, menuButtonY, GameOverButtonWidth, GameOverButtonHeight, "Menu");
}

void Select::showWorldTransition(int worldNumber) {
    inTransition = true;
    currentWorld = worldNumber;
}

void Select::showWorldGameOver(int worldNumber)
{
    inGameOver = true;
    currentWorld = worldNumber;
}

void Select::handleEvent(const SDL_Event& event, int& selectedLevel) {
    if (inTransition) {
        handleButtonEvent(&buttonBackToMenu, const_cast<SDL_Event*>(&event));
        handleButtonEvent(&buttonNextWorld, const_cast<SDL_Event*>(&event));

        if (isButtonClicked(&buttonBackToMenu, const_cast<SDL_Event*>(&event))) {
            inTransition = false;
            selectedLevel = -1;
        }
        else if (isButtonClicked(&buttonNextWorld, const_cast<SDL_Event*>(&event))) {
            inTransition = false;
            selectedLevel = 1 + currentWorld;
        }
    }

    if (inGameOver) {
        handleButtonEvent(&buttonBackToMenu, const_cast<SDL_Event*>(&event));

        if (isButtonClicked(&buttonBackToMenu, const_cast<SDL_Event*>(&event))) {
            inGameOver = false;
            selectedLevel = -1;
        }
    }

    else {
        handleButtonEvent(&buttonLevel1, const_cast<SDL_Event*>(&event));
        handleButtonEvent(&buttonLevel2, const_cast<SDL_Event*>(&event));
        handleButtonEvent(&buttonLevel3, const_cast<SDL_Event*>(&event));
        handleButtonEvent(&buttonLevel4, const_cast<SDL_Event*>(&event));

        if (isButtonClicked(&buttonLevel1, const_cast<SDL_Event*>(&event))) {
            selectedLevel = 1;
        }
        else if (isButtonClicked(&buttonLevel2, const_cast<SDL_Event*>(&event))) {
            selectedLevel = 2;
        }
        else if (isButtonClicked(&buttonLevel3, const_cast<SDL_Event*>(&event))) {
            selectedLevel = 3;
        }
        else if (isButtonClicked(&buttonLevel4, const_cast<SDL_Event*>(&event))) {
            selectedLevel = 4;
        }
    }
}

void Select::draw(SDL_Renderer* renderer) {
    if (inTransition) {
        SDL_SetRenderDrawColorFloat(renderer, 0.2f, 0.2f, 0.3f, 1.0f);
        SDL_RenderFillRect(renderer, nullptr);

        renderButton(renderer, &buttonBackToMenu, font);
        renderButton(renderer, &buttonNextWorld, font);
    }
    else if (inGameOver) {
        SDL_SetRenderDrawColorFloat(renderer, 0.2f, 0.2f, 0.3f, 1.0f);
        SDL_RenderFillRect(renderer, nullptr);

        renderButton(renderer, &buttonBackToMenu, font);
    }
    else {
        SDL_SetRenderDrawColorFloat(renderer, 1.0f, 1.0f, 0.0f, 1.0f);
        SDL_RenderFillRect(renderer, nullptr);

        renderButton(renderer, &buttonLevel1, font);
        renderButton(renderer, &buttonLevel2, font);
        renderButton(renderer, &buttonLevel3, font);
        renderButton(renderer, &buttonLevel4, font);
    }
}