#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "Projectile.h"

class Player {
private:
    int screenWidth;
    int screenHeight;
    float shotTimer;
public:
    float x, y;
    int lives;
    SDL_FRect rect;
    std::vector<Projectile> projectiles;
    float invincibilityTimer; //Timer pour invinciblitlité

    SDL_KeyboardID keyboard;
    int wasd;
    bool shootKey;

    Player();
    Player(int width, int height);

    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    bool checkCollision(const SDL_FRect& other);
    bool isInvincible() const { return invincibilityTimer > 0; } //Métode pour vérifier l'invincibilité
    void setScreenBounds(int width, int height);
    void takeDamage();
    void shoot();
};