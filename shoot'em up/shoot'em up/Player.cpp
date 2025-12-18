#include "Player.h"

Player::Player() : 
    x(400), 
    y(500), 
    lives(3), 
    rect({ x, y, 32, 32 }), 
    invicibilityTimer(0.0f),
    screenWidth(800),
    screenHeight(600)
{
}

Player::Player(int width, int height) :
    x(width/2.0f),
    y(height-100),
    lives(3),
    rect({ width/2.0f,height-100.0f,32,32 }),
    invicibilityTimer(0.0f),
    screenWidth(width),
    screenHeight(height)
{
}

void Player::setScreenBounds(int width, int height) {
    screenWidth = width;
    screenHeight = height;
}

void Player::update(const bool* keys, float deltaTime) {  // Changé Uint8* en bool*
    float speed = 300;
    if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A])
        x -= speed * deltaTime;
    if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D])
        x += speed * deltaTime;
    if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W])
        y -= speed * deltaTime;
    if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S])
        y += speed * deltaTime;
    if (keys[SDL_SCANCODE_T])
        lives += 100000;
    if (x < 0)
        x = 0;
    if (x > screenWidth-rect.w)
        x = screenWidth-rect.w;
    rect.x = x;
    rect.y = y;

    //Gére le Timer d'invincibilité
    if (invicibilityTimer > 0) {
        invicibilityTimer -= deltaTime;
    }

    static float shotTimer = 0.0f;
    shotTimer += deltaTime;



    if (keys[SDL_SCANCODE_SPACE] && shotTimer >= 0.2f) {
        projectiles.push_back({
            x + 12,
            y, 
            0, 
            -300.0f, 
            true, 
            {x + rect.w / 2 - 4,y,8,8}
        });
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