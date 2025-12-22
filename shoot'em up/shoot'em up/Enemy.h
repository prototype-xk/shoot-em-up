#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <memory>
#include <cmath>
#include "random.h"
#include "Projectile.h"

class Player;

class Enemy {
protected:
    int screenWidth;
    int screenHeight;
    float shotTimer;
public:
    float x, y;
    SDL_FRect rect;
    std::vector<Projectile> projectiles;
    float moveTimer;
    SDL_Texture* texture;

    Enemy(float px, float py, int sw, int sh);
    virtual ~Enemy();

    void loadTexture(SDL_Renderer* renderer, const char* imagePath);

    virtual void update(float deltaTime, Player& player) = 0;
    virtual void render(SDL_Renderer* renderer) = 0;
    virtual bool checkCollision(const SDL_FRect& other);
    virtual int getType() const = 0;
};

class BasicEnemy : public Enemy {
public:
    BasicEnemy(float px, float py, int sw, int sh);

    void update(float deltaTime, Player& player) override;
    void render(SDL_Renderer* renderer) override;
    int getType() const override;
};

class ZigzagEnemy : public Enemy {
public:
    ZigzagEnemy(float px, float py, int sw, int sh);

    void update(float deltaTime, Player& player) override;
    void render(SDL_Renderer* renderer) override;
    int getType() const override;
};

class Drowned : public Enemy {
public:
    Drowned(float px, float py, int sw, int sh);

    void update(float deltaTime, Player& player) override;
    void render(SDL_Renderer* renderer) override;
    int getType()const override;
};

class Guardian : public Enemy {
public:
    Guardian(float px, float py, int sw, int sh);

    void update(float deltaTime, Player& player) override;
    void render(SDL_Renderer* renderer) override;
    int getType()const override;

private:
    enum Phase { BURST, PAUSE };
    Phase currentPhase = BURST;
    float phaseTimer = 0.0f;
    float shotTimer = 0.0f;
};

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
class SkeletonEnemy : public Enemy {
public:
    SkeletonEnemy(float px, float py, int sw, int sh);

    void update(float deltaTime, Player& player) override;
    void render(SDL_Renderer* renderer) override;
    int getType() const override;
};

class BlazeEnemy : public Enemy {
public:
    BlazeEnemy(float px, float py, int sw, int sh);

    void update(float deltaTime, Player& player) override;
    void render(SDL_Renderer* renderer) override;
    int getType() const override;
};

class WitherBoss : public Enemy {
public:
    WitherBoss(float px, float py, int sw, int sh);

    void update(float deltaTime, Player& player) override;
    void render(SDL_Renderer* renderer) override;
    int getType() const override;
};


class ShulkerEnemy : public Enemy {
private:
    float invulnerabilityTimer;
    bool isInvulnerable;
public:
    ShulkerEnemy(float px, float py, int sw, int sh);

    void update(float deltaTime, Player& player) override;
    void render(SDL_Renderer* renderer) override;
    bool checkCollision(const SDL_FRect& other) override;
    int getType() const override;
};

class DragonBoss : public Enemy {

public:
    DragonBoss(float px, float py, int sw, int sh);
    void update(float deltaTime, Player& player) override;
    void render(SDL_Renderer* renderer) override;
    int getType() const override;
};

class EndermanEnemy : public Enemy {

public:
    EndermanEnemy(float px, float py, int sw, int sh);
    void update(float deltaTime, Player& player) override;
    void render(SDL_Renderer* renderer) override;
    int getType() const override;
};

class PhantomEnemy : public Enemy {
public:
    PhantomEnemy(float px, float py, int sw, int sh);

    void update(float deltaTime, Player& player) override;
    void render(SDL_Renderer* renderer) override;
    int getType() const override;
};

class SculkEnemy : public Enemy {
public:
    SculkEnemy(float px, float py, int sw, int sh);

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

std::unique_ptr<Enemy> createEnemy(int type, float x, float y, int screenWidth, int screenHeight,SDL_Renderer* renderer);