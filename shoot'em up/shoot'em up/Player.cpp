#include "Player.h"

Player::Player() : 
    x(400),
    y(500),
    lives(5),
    rect({ x, y, 32, 32 }),
    invicibilityTimer(0.0f),
    screenWidth(800),
    screenHeight(600),
    texture(nullptr),
    projectileTexture(nullptr)
{
}

Player::Player(int width, int height) :
    x(width / 2.0f),
    y(height - 100),
    lives(5),
    rect({ width / 2.0f, height - 100.0f, 32, 32 }),
    invicibilityTimer(0.0f),
    screenWidth(width),
    screenHeight(height),
    texture(nullptr),
    projectileTexture(nullptr)
{
}

Player::Player(int width, int height, SDL_Renderer* renderer, const char* imagePath) :
    x(width / 2.0f),
    y(height - 100),
    lives(5),
    rect({ width / 2.0f, height - 100.0f, 32, 32 }),
    invicibilityTimer(0.0f),
    screenWidth(width),
    screenHeight(height),
    texture(nullptr),
    projectileTexture(nullptr)
{
    SDL_Surface* surface = IMG_Load(imagePath);
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_DestroySurface(surface);

        if (texture) {
            std::cout << "[INFO] Player texture loaded successfully\n";
        }
        else {
            std::cerr << "[ERROR] Failed to create texture: " << SDL_GetError() << "\n";
        }
    }
    else {
        std::cerr << "[ERROR] Failed to load image: " << SDL_GetError() << "\n";
    }
}

Player::~Player() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    if (projectileTexture) {
        SDL_DestroyTexture(projectileTexture);
        projectileTexture = nullptr;
    }
}

void Player::setScreenBounds(int width, int height) {
    screenWidth = width;
    screenHeight = height;
}

void Player::update(const bool* keys, float deltaTime) {
    float speed = 500;
    if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A])
        x -= speed * deltaTime;
    if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D])
        x += speed * deltaTime;
    if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])
        y -= speed * deltaTime;
    if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])
        y += speed * deltaTime;
    if (keys[SDL_SCANCODE_T])
        lives += 100000;
    if (x < 0)
        x = 0;
    if (x > screenWidth-rect.w)
        x = screenWidth-rect.w;
    if (y < 0)
        y = 0;
    if (y > screenHeight - rect.h)
        y = screenHeight - rect.h;
    rect.x = x;
    rect.y = y;

    if (invicibilityTimer > 0) {
        invicibilityTimer -= deltaTime;
    }

    float shotTimer = 0.0f;
    shotTimer += deltaTime;



    if (keys[SDL_SCANCODE_SPACE] && shotTimer >= 0.1f) {
        projectiles.push_back(Projectile(
            x + rect.w / 2 - 8,
            y,
            0,
            -1000.0f,
            true,
            { x + rect.w / 2 - 8, y, 16, 16 },
            projectileTexture
        ));
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

void Player::loadTexture(SDL_Renderer* renderer, const char* imagePath) {
    if (texture) {
        SDL_DestroyTexture(texture);
    }

    SDL_Surface* surface = IMG_Load(imagePath);
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_DestroySurface(surface);

        if (texture) {
            std::cout << "[INFO] Player texture loaded\n";
        }
        else {
            std::cerr << "[ERROR] Failed to create texture: " << SDL_GetError() << "\n";
        }
    }
    else {
        std::cerr << "[ERROR] Failed to load image: " << SDL_GetError() << "\n";
    }
}

void Player::loadProjectileTexture(SDL_Renderer* renderer, const char* imagePath) {
    if (projectileTexture) {
        SDL_DestroyTexture(projectileTexture);
    }

    SDL_Surface* surface = IMG_Load(imagePath);
    if (surface) {
        projectileTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_DestroySurface(surface);

        if (projectileTexture) {
            std::cout << "[INFO] Projectile texture loaded\n";
        }
        else {
            std::cerr << "[ERROR] Failed to create projectile texture: " << SDL_GetError() << "\n";
        }
    }
    else {
        std::cerr << "[ERROR] Failed to load projectile image: " << SDL_GetError() << "\n";
    }
}

void Player::render(SDL_Renderer* renderer) {
    if (texture) {
        if (isInvincible()) {
            SDL_SetTextureColorMod(texture, 255, 100, 100);
        }
        else {
            SDL_SetTextureColorMod(texture, 255, 255, 255);
        }
        SDL_RenderTexture(renderer, texture, nullptr, &rect);
    }
    else {
        if (isInvincible()) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        }
        else {
            SDL_SetRenderDrawColor(renderer, 230, 123, 209, 255);
        }
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    for (auto& p : projectiles) {
        if (p.texture) {
            SDL_RenderTexture(renderer, p.texture, nullptr, &p.rect);
        }
        else {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            SDL_RenderFillRect(renderer, &p.rect);
        }
    }
}

bool Player::checkCollision(const SDL_FRect& other) {
    return SDL_HasRectIntersectionFloat(&rect, &other);
}