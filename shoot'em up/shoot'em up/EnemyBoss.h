#pragma once
#include <SDL3/SDL.h>
#include "Enemy.h"
#include <vector>
#include <memory>
#include <cmath>
#include "random.h"
#include "Projectile.h"
class Elder_Guardian : public Enemy {
public:
    Elder_Guardian(float px, float py, int sw, int sh);

    void update(float deltaTime, Player& player) override;
    void render(SDL_Renderer* renderer) override;
    int getType()const override;

private:
    enum Phase { BURST, PAUSE };
    Phase currentPhase = BURST;
    float phaseTimer = 0.0f;
    float shotTimer = 0.0f;
};

class WitherBoss : public Enemy {
public:
    WitherBoss(float px, float py, int sw, int sh);

    void update(float deltaTime, Player& player) override;
    void render(SDL_Renderer* renderer) override;
    int getType() const override;
};

class DragonBoss : public Enemy {

public:
    DragonBoss(float px, float py, int sw, int sh);
    void update(float deltaTime, Player& player) override;
    void render(SDL_Renderer* renderer) override;
    int getType() const override;
};

class WardenBoss : public Enemy {
public:
    WardenBoss(float px, float py, int sw, int sh, int& wardenHealthRef);
    void update(float deltaTime, Player& player) override;
    void render(SDL_Renderer* renderer) override;
    int getType() const override;
private:
    int& wardenHealthRef;
    int maxHealth;
};

std::unique_ptr<Enemy> createEnemy(int type, float x, float y, int screenWidth, int screenHeight, SDL_Renderer* renderer);