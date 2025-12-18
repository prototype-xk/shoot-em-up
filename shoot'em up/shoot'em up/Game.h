#pragma once
#include <SDL3/SDL.h>
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include "Button.h"
#include "Select.h"
#include "Custom.h"
#include "LevelBase.h"
#include "LevelLoader.h"
#include "Start.h"
#include "Player.h"

class Game
{
public:
    Game();
    int run();
    int getScreenWidth() const { return screenWidth; }
    int getScreenHeight() const { return screenHeight; }

private:
    int screenWidth;
    int screenHeight;
    Start* start=nullptr;
    Select* select = nullptr;
    Custom* custom = nullptr;
    std::vector<std::string> levelsOrder;
    int currentLevelIndex;
    std::unique_ptr<LevelBase> currentLevel;
    bool shouldQuit = false;
    int totalScore;

    enum class State { MENU, CUSTOM, SELECT, LEVEL};
    State currentState;
    Button menuButton;
    TTF_Font* font = nullptr;

    std::vector<Player> players;
    int player_count;
    int MAX_PLAYER_COUNT;

    int whoseKeyboard(SDL_KeyboardID id, const std::vector<Player>& players, int player_count);

    void loadLevel(int index);
    void handleMenuEvent(const SDL_Event& event, bool& shouldSwitchToCustom);
    void drawMenu(SDL_Renderer* renderer);
    bool initializeSDL();
    bool CreateWindowAndRenderer(SDL_Window*& window, SDL_Renderer*& renderer);
};