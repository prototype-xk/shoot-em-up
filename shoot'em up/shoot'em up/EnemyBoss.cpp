#include "EnemyBoss.h"
Elder_Guardian::Elder_Guardian(float px, float py, int sw, int sh) : Enemy(px, py, sw, sh) {}
void Elder_Guardian::update(float deltaTime, Player& player)
{
    rect.w = 150;
    rect.h = 200;

    phaseTimer += deltaTime;
    if (currentPhase == BURST) {
        shotTimer += deltaTime;
        if (shotTimer >= 0.001f) {
            float dx = player.x - x;
            float dy = player.y - y;
            float distance = sqrt(dx * dx + dy * dy);
            float speed = 200.0f;
            float vx = (dx / distance) * speed;
            float vy = (dy / distance) * speed;
            projectiles.push_back({
                x + rect.w / 2 - 4,
                y + rect.h,
                vx,
                vy,
                false,
                {x + rect.w / 2 ,y + rect.h,8,8}
                });
            shotTimer = 0.0f;
        }
        if (phaseTimer >= 1.5f) {
            currentPhase = PAUSE;
            phaseTimer = 0.0f;
            shotTimer = 0.0f;
        }
    }
    else {
        if (phaseTimer >= 2.0f) {
            currentPhase = BURST;
            phaseTimer = 0.0f;
        }
    }
    for (auto it = projectiles.begin();
        it != projectiles.end(); ) {

        it->update(deltaTime);
        if (it->isOffScreen(screenWidth, screenHeight))
            it = projectiles.erase(it);
        else ++it;
    }
}
void Elder_Guardian::render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 62, 137, 134, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    for (auto& p : projectiles) SDL_RenderFillRect(renderer, &p.rect);
}
int Elder_Guardian::getType() const { return 3; }

WitherBoss::WitherBoss(float px, float py, int sw, int sh) : Enemy(px, py, sw, sh) {}

void WitherBoss::update(float deltaTime, Player& player) {
    rect.x = x;
    rect.y = y;

    static float shotTimer = 0.0f;
    shotTimer += deltaTime;
    if (shotTimer >= 2.0f) {
        float dx = player.x - x;
        float dy = player.y - y;
        float distance = sqrt(dx * dx + dy * dy);
        float speed = 200.0f;
        float vx = (dx / distance) * speed;
        float vy = (dy / distance) * speed;
        projectiles.push_back({
            x + rect.w / 2 - 4,
            y + rect.h,
            vx,
            vy,
            false,
            {x + rect.w / 2 - 4,y + rect.h,8,8}
            });
        shotTimer = 0.0f;
    }
    for (auto it = projectiles.begin();
        it != projectiles.end(); ) {

        it->update(deltaTime);
        if (it->isOffScreen(screenWidth, screenHeight))
            it = projectiles.erase(it);
        else ++it;
    }
}

void WitherBoss::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 109, 31, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    for (auto& p : projectiles) SDL_RenderFillRect(renderer, &p.rect);
}

int WitherBoss::getType() const { return 6; }

DragonBoss::DragonBoss(float px, float py, int sw, int sh) : Enemy(px, py, sw, sh) {
    rect.w = 180;
    rect.h = 250;
}

void DragonBoss::update(float deltaTime, Player& player) {
    static float shotTimer = 0.0f;
    shotTimer += deltaTime;
    if (shotTimer >= 2.0f) {
        projectiles.push_back({
            x + 80,
            y + 250,
            0,
            350.0f,
            false,
            {x + rect.w / 2 - 4,
            y + rect.h,
            8,
            8} });
        shotTimer = 0.0f;
        projectiles.push_back({
            x + 100,
            y + 250,
            0,
            350.0f,
            false,
            {x + rect.w / 2 - 4,
            y + rect.h,
            8,
            8} });
        shotTimer = 0.0f;
        projectiles.push_back({
            x + 0,
            y + 250,
            0,
            450.0f,
            false,
            {x + rect.w / 2 - 4,
            y + rect.h,
            8,
            8} });
        shotTimer = 0.0f;
        projectiles.push_back({
            x + 10,
            y + 250,
            0,
            450.0f,
            false,
            {x + rect.w / 2 - 4,
            y + rect.h,
            8,
            8} });
        shotTimer = 0.0f;
        projectiles.push_back({
            x + 180,
            y + 250,
            0,
            450.0f,
            false,
            {x + rect.w / 2 - 4,
            y + rect.h,
            8,
            8} });
        shotTimer = 0.0f;
        projectiles.push_back({
            x + 170,
            y + 250,
            0,
            450.0f,
            false,
            {x + rect.w / 2 - 4,
            y + rect.h,
            8,
            8} });
        shotTimer = 0.0f;
    }
    for (auto it = projectiles.begin();
        it != projectiles.end(); ) {

        it->update(deltaTime);
        if (it->isOffScreen(screenWidth, screenHeight))
            it = projectiles.erase(it);
        else ++it;
    }

}

void DragonBoss::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    for (auto& p : projectiles) SDL_RenderFillRect(renderer, &p.rect);
}

int DragonBoss::getType() const { return 9; }

WardenBoss::WardenBoss(float px, float py, int sw, int sh, int& wardenHealthRef) : Enemy(px, py, sw, sh), wardenHealthRef(wardenHealthRef) {
    maxHealth = 150;
    rect.w = 120;
    rect.h = 100;
}

void WardenBoss::update(float deltaTime, Player& player) {

}


void WardenBoss::render(SDL_Renderer* renderer) {
    if (wardenHealthRef > 120) {
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    }
    else if (wardenHealthRef > 80) {
        SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);
    }
    else if (wardenHealthRef > 40) {
        SDL_SetRenderDrawColor(renderer, 150, 0, 0, 255);
    }
    else if (wardenHealthRef > 15) {
        SDL_SetRenderDrawColor(renderer, 200, 50, 0, 255);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    }

    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (auto& p : projectiles) SDL_RenderFillRect(renderer, &p.rect);
}

int WardenBoss::getType() const { return 12; }

