#include "Player.h"

Player::Player() : 
    x(400), 
    y(500), 
    lives(3), 
    rect({ 400, 500, 32, 32 }), 
    invincibilityTimer(0.0f),
    screenWidth(800),
    screenHeight(600),
    keyboard(0),
    wasd(0),
    shootKey(false),
    shotTimer(0.0f)
{
}

Player::Player(int width, int height) :
    x(width/2.0f),
    y(height-100),
    lives(3),
    rect({ width/2.0f,height-100.0f,32,32 }),
    invincibilityTimer(0.0f),
    screenWidth(width),
    screenHeight(height),
    keyboard(0),
    wasd(0),
    shootKey(false),
    shotTimer(0.0f)
{
}

void Player::setScreenBounds(int width, int height) {
    screenWidth = width;
    screenHeight = height;
}

void Player::takeDamage() {
    if (!isInvincible() && lives > 0) {
        lives--;
        invincibilityTimer = 2.0f;
    }
}

void Player::shoot() {
    projectiles.push_back({
        x + rect.w / 2 - 4,
        y,
        0,
        -300.0f,
        true,
        { x + rect.w / 2 - 4, y, 8, 8 }
        });
}

void Player::update(float deltaTime) {  // Changé Uint8* en bool*
    float speed = 300.0f;
    float dx = 0, dy = 0;

    if (wasd & 2) dy -= 1;
    if (wasd & 4) dy += 1;
    if (wasd & 1) dy -= 1
        ;
    if (wasd & 8) dy += 1;

    if (dx != 0 && dy != 0) {
        dx *= 0.707f;
        dy *= 0.707f;
    }

    x += dx * speed * deltaTime;
    y += dy * speed * deltaTime;

    if (x < 0) x = 0;
    if (x > screenWidth - rect.w) x = screenWidth - rect.w;
    if (y < 0) y = 0;
    if (y > screenHeight - rect.h) y = screenHeight - rect.h;

    rect.x = x;
    rect.y = y;

    if (invincibilityTimer > 0) {
        invincibilityTimer -= deltaTime;
    }

    shotTimer += deltaTime;
    if (shootKey && shotTimer >= 0.2f) {
        shoot();
        shotTimer = 0.0f;
    }
    
    for (auto it = projectiles.begin(); it != projectiles.end(); ) {
        it->update(deltaTime);
        if (it->isOffScreen(screenWidth, screenHeight))
            it = projectiles.erase(it);
        else
            ++it;
    }
}

void Player::render(SDL_Renderer* renderer) {
    if (isInvincible()) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 230, 123, 209, 255);
    }
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    for (auto& p : projectiles)
        SDL_RenderFillRect(renderer, &p.rect);
}

bool Player::checkCollision(const SDL_FRect& other) {
    return SDL_HasRectIntersectionFloat(&rect, &other);
}