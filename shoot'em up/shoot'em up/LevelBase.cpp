#include "LevelBase.h"
#include <iostream>
#include "Projectile.h"



LevelBase::LevelBase(TTF_Font* font,int width, int height)
    : currentCommand(0),
    levelStartTime(0),
    levelCompleted(false),
    levelFailed(false),
    score(0),
    font(font),
    DragonHealth(100),
    screenWidth(width),
    screenHeight(height),
    scoreTexture(nullptr),
    player(width,height)
{
    menuButton = createButton(10.0f, 425.0f, 75.0f, 50.0f, "Menu");
}

bool LevelBase::loadFromFile(const std::string& scriptPath) {
    if (!LevelLoader::loadLevelScript(scriptPath, script)) {
        std::cerr << "[ERROR] Failed to load level script: " << scriptPath << "\n";
        return false;
    }

    levelStartTime = SDL_GetTicks();
    currentCommand = 0;
    levelCompleted = false;
    levelFailed = false;
    score = 0;
    DragonHealth = 100;
    enemies.clear();
    allProjectiles.clear();

    std::cout << "[INFO] Level loaded with " << script.size() << " commands\n";

    return true;
}

void LevelBase::executeCommand(const ScriptCommand& cmd) {
    std::cout << "[CMD] " << cmd.command << " at time " << cmd.time << "\n";

    if (cmd.command == "SPAWN" && cmd.params.size() >= 4 && cmd.params[0] == "ENEMY") {
        float xPercent = std::stof(cmd.params[1]);
        float yPercent = std::stof(cmd.params[2]);
        float x = xPercent * screenWidth;
        float y = yPercent * screenHeight;
        int type = std::stoi(cmd.params[3]);

        auto enemy = createEnemy(type, x, y, screenWidth, screenHeight);
        if (enemy) {
            enemies.push_back(std::move(enemy));
            std::cout << "[INFO] Enemy spawned at (" << x << ", " << y << ") type " << type << "\n";
        }
    }
    else if (cmd.command == "END") {
        levelCompleted = true;
        std::cout << "[INFO] Level completed!\n";
    }
}

void LevelBase::handleCollisions() {
    allProjectiles = player.projectiles;
    for (auto& e : enemies) {
        for (auto& p : e->projectiles) {
            allProjectiles.push_back(p);
        }
    }
    for (auto pit = allProjectiles.begin(); pit != allProjectiles.end(); ) {
        bool hit = false;

        if (pit->isPlayer) {
            for (auto eit = enemies.begin(); eit != enemies.end(); ) {
                if ((*eit)->checkCollision(pit->rect)) {
                    if ((*eit)->getType() == 9) {
                        DragonHealth--;
                        std::cout << "Dragon touche HP restant: " << DragonHealth << "\n";
                        hit = true;
                        
                        if (DragonHealth <= 0) {
                            levelCompleted = true;
                            score += 500;
                            std::cout << "Dragon vaincu\n";
                            eit = enemies.erase(eit);
                        }
                        else {
                            ++eit;
                        }
                    }
                    else {
                        score += 100;
                        std::cout << "Enemy destroyed Score = " << score << "\n";
                        eit = enemies.erase(eit);
                        hit = true;
                    }
                    break;
                }
                else {
                    ++eit;
                }
            }
        }
        else {
            if (player.checkCollision(pit->rect)) {
                if (!player.isInvincible()) {
                    player.lives--;
                    player.invincibilityTimer = 1.0f;
                    std::cout << "Player hit! Lives : " << player.lives << "\n";
                    if (player.lives <= 0) {
                        levelFailed = true;
                    }
                }
                hit = true;
            }
        }

        if (hit || pit->isOffScreen(screenWidth, screenHeight)) {
            pit = allProjectiles.erase(pit);
        }
        else {
            ++pit;
        }
    }

    player.projectiles.clear();
    for (auto& p : allProjectiles) {
        if (p.isPlayer) {
            player.projectiles.push_back(p);
        }
    }
}

void LevelBase::handleEvent(const SDL_Event& event, bool& shouldSwitchToMenu) {
    SDL_Event ev = event;
    handleButtonEvent(&menuButton, &ev);
    if (isButtonClicked(&menuButton, &ev)) {
        shouldSwitchToMenu = true;
    }
}

void LevelBase::update(float deltaTime) {
    const bool* keys = SDL_GetKeyboardState(NULL);
    player.update(deltaTime);

    float elapsed = (SDL_GetTicks() - levelStartTime) / 1000.0f;
    while (currentCommand < script.size() && elapsed >= script[currentCommand].time) {
        executeCommand(script[currentCommand]);
        currentCommand++;
    }

    for (auto& e : enemies) {
        e->update(deltaTime, player);
    }

    for (auto& enemy : enemies) {
        enemy->update(deltaTime, player);
    }

    for (auto& enemy : enemies) {
        if (!player.isInvincible() && player.checkCollision(enemy->rect)) {
            player.lives--;
            player.invincibilityTimer = 2.0f;
            std::cout << "Player touched" << player.lives << "\n";
            break;
        }
    }

    handleCollisions();

    if (enemies.empty() && currentCommand >= script.size() && !levelCompleted) {
        levelCompleted = true;
    }
}

void LevelBase::draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, nullptr);

    player.render(renderer);

    for (auto& e : enemies) {
        e->render(renderer);
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    for (auto& p : allProjectiles) {
        if (!p.isPlayer) {
            SDL_RenderFillRect(renderer, &p.rect);
        }
    }

    renderButton(renderer, &menuButton, font);

    std::string scoreText = "Score : " + std::to_string(score);
    SDL_RenderDebugText(renderer, 10, 10, scoreText.c_str());
    std::string livesText = "Lives : " + std::to_string(player.lives);
    SDL_RenderDebugText(renderer, 10, 30, livesText.c_str());
}


bool LevelBase::isCompleted() const {
    return levelCompleted;
}

bool LevelBase::isFailed() const {
    return levelFailed;
}

int LevelBase::getScore() const {
    return score;
}