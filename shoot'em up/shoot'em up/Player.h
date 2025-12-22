#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <string>
#include <iostream>
#include "Projectile.h"

class Player {
private:
    int screenWidth;
    int screenHeight;
public:
    Player();
    Player(int width, int height);
    Player(int width, int height, SDL_Renderer* renderer, const char* imagePath);
    ~Player();
    float x, y;
    int lives;
    SDL_FRect rect;
    std::vector<Projectile> projectiles;
    float invicibilityTimer;
    SDL_Texture* texture;
    SDL_Texture* projectileTexture;
    float width, height;
    void update(const bool* keys, float deltaTime);
    void loadTexture(SDL_Renderer* renderer, const char* imagePath);
    void loadProjectileTexture(SDL_Renderer* renderer, const char* imagePath);
    void render(SDL_Renderer* renderer);
    bool checkCollision(const SDL_FRect& other);
    bool isInvincible() const { return invicibilityTimer > 0; }
    void setScreenBounds(int width, int height);
};