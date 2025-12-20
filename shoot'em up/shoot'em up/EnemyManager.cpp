#include "EnemyManager.h"

std::unique_ptr<Enemy> createEnemy(int type, float x, float y, int screenWidth, int screenHeight, SDL_Renderer* renderer) {
    std::unique_ptr<Enemy> enemy = nullptr;

    if (type == 23) {
        enemy = std::make_unique<BasicEnemy>(x, y, screenWidth, screenHeight);
    }
    else if (type == 24) {
        enemy = std::make_unique<ZigzagEnemy>(x, y, screenWidth, screenHeight);
    }
    else if (type == 1) {
        enemy = std::make_unique<Drowned>(x, y, screenWidth, screenHeight);
        if (enemy) enemy->loadTexture(renderer, "Assets/Drowned.png");
    }
    else if (type == 2) {
        enemy = std::make_unique<Guardian>(x, y, screenWidth, screenHeight);
        if (enemy) enemy->loadTexture(renderer, "Assets/Guardian.png");
    }
    else if (type == 3) {
        enemy = std::make_unique<Elder_Guardian>(x, y, screenWidth, screenHeight);
    }
    else if (type == 4) {
        enemy = std::make_unique<SkeletonEnemy>(x, y, screenWidth, screenHeight);
        if (enemy) enemy->loadTexture(renderer, "Assets/Skeleton.png");
    }
    else if (type == 5) {
        enemy = std::make_unique<BlazeEnemy>(x, y, screenWidth, screenHeight);
        if (enemy) enemy->loadTexture(renderer, "Assets/Blaze.png");
    }
    else if (type == 6) {
        enemy = std::make_unique<WitherBoss>(x, y, screenWidth, screenHeight);
    }
    else if (type == 7) {
        enemy = std::make_unique<ShulkerEnemy>(x, y, screenWidth, screenHeight);
        if (enemy) enemy->loadTexture(renderer, "Assets/Shulker.png");
    }
    else if (type == 8) {
        enemy = std::make_unique<EndermanEnemy>(x, y, screenWidth, screenHeight);
        if (enemy) enemy->loadTexture(renderer, "Assets/Enderman.png");
    }
    else if (type == 9) {
        enemy = std::make_unique<DragonBoss>(x, y, screenWidth, screenHeight);
    }
    else if (type == 10) {
        enemy = std::make_unique<PhantomEnemy>(x, y, screenWidth, screenHeight);
        if (enemy) enemy->loadTexture(renderer, "Assets/Phantom.png");
    }
    else if (type == 11) {
        enemy = std::make_unique<SculkEnemy>(x, y, screenWidth, screenHeight);
        if (enemy) enemy->loadTexture(renderer, "Assets/Sculk.png");
    }
    else if (type == 13) {
        enemy = std::make_unique<WitherBoss>(x, y, screenWidth, screenHeight);
    }

    return enemy;
}