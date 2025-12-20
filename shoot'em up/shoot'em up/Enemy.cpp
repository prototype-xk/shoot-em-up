#include "Enemy.h"
#include "Player.h"

Enemy::Enemy(float px, float py,int sw, int sh) :
    x(px),
    y(py),
    rect({ x, y, 32, 32 }),
    moveTimer(0.0f),
    shotTimer(0.0f),
    screenWidth(sw),
    screenHeight(sh),
    texture(nullptr)
{
}

Enemy::~Enemy() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

void Enemy::loadTexture(SDL_Renderer* renderer, const char* imagePath) {
    if (texture) {
        SDL_DestroyTexture(texture);
    }

    SDL_Surface* surface = IMG_Load(imagePath);
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_DestroySurface(surface);

        if (texture) {
            std::cout << "[INFO] Enemy texture loaded: " << imagePath << "\n";
        }
        else {
            std::cerr << "[ERROR] Failed to create enemy texture: " << SDL_GetError() << "\n";
        }
    }
    else {
        std::cerr << "[ERROR] Failed to load enemy image: " << SDL_GetError() << "\n";
    }
}

bool Enemy::checkCollision(const SDL_FRect& other) {
    return SDL_HasRectIntersectionFloat(&rect, &other);
}
/* Fait Par IA : */
//BasicEnemy::BasicEnemy(float px, float py,int sw,int sh) : Enemy(px, py,sw,sh) {}
//
//void BasicEnemy::update(float deltaTime, Player& player) {
//    y += 100.0f * deltaTime;
//    rect.x = x;
//    rect.y = y;
//
//    static float shotTimer = 0.0f;
//    shotTimer += deltaTime;
//    if (shotTimer >= 2.0f) { // Tire toute les 2 secondes
//        // Taille, la postion et vitesse du projectile
//        projectiles.push_back({
//            x + 12,
//            y + 32,
//            0,
//            200.0f,
//            false,
//            {x + rect.w / 2 - 4,y + rect.h,8,8}
//            });
//        shotTimer = 0.0f; // Timer réinitialiser
//    }
//    // Si le projectile sort de la fenêtre et n'est donc plus visible alors on le supprime
//    for (auto it = projectiles.begin();
//        it != projectiles.end(); ) {
//
//        it->update(deltaTime);
//        if (it->isOffScreen(screenWidth, screenHeight))
//            it = projectiles.erase(it);
//        else ++it;
//    }
//}
//
//// Créer l'ennemi ainsi que sa couleur et la couleur de ses projectiles
//void BasicEnemy::render(SDL_Renderer* renderer) {
//    SDL_SetRenderDrawColor(renderer, 62, 137, 134, 255);
//    SDL_RenderFillRect(renderer, &rect);
//    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
//    for (auto& p : projectiles) SDL_RenderFillRect(renderer, &p.rect);
//}
//
//int BasicEnemy::getType() const { return 23; } // ID de l'ennemi
//
//ZigzagEnemy::ZigzagEnemy(float px, float py, int sw, int sh) : Enemy(px, py,sw,sh) {}
//
//void ZigzagEnemy::update(float deltaTime, Player& player) {
//    y += 100.0f * deltaTime;
//    moveTimer += deltaTime;
//    x += sin(moveTimer * 5.0f) * 150.0f * deltaTime;
//    rect.x = x; rect.y = y;
//
//    static float shotTimer = 0.0f;
//    shotTimer += deltaTime;
//    if (shotTimer >= 1.0f) {// Tire toute les 1 secondes
//        // Taille, la postion et vitesse du projectile
//        projectiles.push_back({
//            x + 12,
//            y + 32,
//            0,
//            250.0f,
//            false,
//            {x + rect.w / 2 - 4,
//            y + rect.h,
//            8,
//            8} });
//        shotTimer = 0.0f; // Timer réinitialiser
//    }
//    // Si le projectile sort de la fenêtre et n'est donc plus visible alors on le supprime
//    for (auto it = projectiles.begin();
//        it != projectiles.end(); ) {
//
//        it->update(deltaTime);
//        if (it->isOffScreen(screenWidth, screenHeight))
//            it = projectiles.erase(it);
//        else ++it;
//    }
//}
//
//// Créer l'ennemi ainsi que sa couleur et la couleur de ses projectiles
//void ZigzagEnemy::render(SDL_Renderer* renderer) {
//    SDL_SetRenderDrawColor(renderer, 58, 164, 118, 255);
//    SDL_RenderFillRect(renderer, &rect);
//    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
//    for (auto& p : projectiles) SDL_RenderFillRect(renderer, &p.rect);
//}
//
//int ZigzagEnemy::getType() const { return 24; } // ID de l'ennemi
//
//Drowned::Drowned(float px, float py, int sw, int sh) : Enemy(px, py, sw, sh) {}
//
//void Drowned::update(float deltaTime, Player& player)
//{
//    y += 50.f * deltaTime;
//    float vx = 0.0f;
//    float dx = player.x - x;
//    float distance = sqrt(dx * dx);
//
//    if (distance > 0.0f) {
//        float speed = 100.0f;
//        float targetVx = (dx / distance) * speed;
//        float homingStrength = 5.0f;
//        vx += (targetVx - vx) * homingStrength * deltaTime;
//        x += vx * deltaTime;
//    }
//
//    rect.x = x;
//    rect.y = y;
//
//    static float shotTimer = 1.5f;
//    shotTimer += deltaTime;
//    if (shotTimer >= 2.0f) {
//        projectiles.push_back({
//            x + 12,
//            y + 32,
//            0,
//            250.0f,
//            false,
//            {x + rect.w / 2 - 4, y + rect.h, 8, 8}
//            });
//        shotTimer = 1.5f;
//    }
//    for (auto it = projectiles.begin();
//        it != projectiles.end(); ) {
//
//        it->update(deltaTime);
//        if (it->isOffScreen(screenWidth, screenHeight))
//            it = projectiles.erase(it);
//        else ++it;
//    }
//}

void Drowned::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_RenderTexture(renderer, texture, nullptr, &rect);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 62, 137, 134, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    for (auto& p : projectiles) SDL_RenderFillRect(renderer, &p.rect);
}

int Drowned::getType() const { return 1; }


Guardian::Guardian(float px, float py, int sw, int sh) : Enemy(px, py, sw, sh) {}

void Guardian::update(float deltaTime, Player& player) {
    y += 50.0f * deltaTime;
    rect.x = x;
    rect.y = y;

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
                {x + rect.w / 2 - 4,y + rect.h,8,8}
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
void Guardian::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_RenderTexture(renderer, texture, nullptr, &rect);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 62, 137, 134, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    for (auto& p : projectiles) SDL_RenderFillRect(renderer, &p.rect);
}
int Guardian::getType() const { return 2; }

void SkeletonEnemy::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_RenderTexture(renderer, texture, nullptr, &rect);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 206, 206, 206, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    for (auto& p : projectiles) SDL_RenderFillRect(renderer, &p.rect);
}
int SkeletonEnemy::getType() const { return 4; }
BlazeEnemy::BlazeEnemy(float px, float py, int sw, int sh) : Enemy(px, py, sw, sh) {}
void BlazeEnemy::update(float deltaTime, Player& player) {
    y += 100.0f * deltaTime;
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
            x + rect.w/2-4,
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

void BlazeEnemy::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_RenderTexture(renderer, texture, nullptr, &rect);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 255, 109, 31, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    for (auto& p : projectiles) SDL_RenderFillRect(renderer, &p.rect);
}

int BlazeEnemy::getType() const { return 5; }



ShulkerEnemy::ShulkerEnemy(float px, float py, int sw, int sh) : Enemy(px, py, sw, sh),
    invulnerabilityTimer(0.0f),
    isInvulnerable(false)
{
}

void ShulkerEnemy::update(float deltaTime, Player& player) {
    y += 50.0f * deltaTime;
    rect.x = x;
    rect.y = y;

    invulnerabilityTimer += deltaTime;
    if (invulnerabilityTimer < 3.0f) {
        isInvulnerable = false;
    }
    else if (invulnerabilityTimer < 4.0f) {
        isInvulnerable = true;
    }
    else {
        invulnerabilityTimer = 0.0f;
    }

    static float shotTimer = 0.0f;
    shotTimer += deltaTime;
    if (shotTimer >= 5.0f) {
        if (player.y > y) {
            float dx = player.x - x;
            float dy = player.y - y;
            float distance = sqrt(dx * dx + dy * dy);
            float speed = 150.0f;
            float vx = (dx / distance) * speed;
            float vy = (dy / distance) * speed;

            projectiles.push_back({
            x + rect.w / 2 - 4,
            y + rect.h,
            vx,
            vy,
            false,
            {x + rect.w / 2 - 4, y + rect.h, 8, 8}
                });
        }
        shotTimer = 0.0f;

        
    }
    for (auto it = projectiles.begin(); it != projectiles.end(); ) {
        float dx = player.x - it->x;
        float dy = player.y - it->y;
        float distance = sqrt(dx * dx + dy * dy);

        float minHomingDistance = 800.0f;

        if (distance > minHomingDistance) {
            float homingStrength = 100.0f;
            float targetVx = (dx / distance) * 200.0f;
            float targetVy = (dy / distance) * 200.0f;
            it->vx += (targetVx - it->vx) * homingStrength * deltaTime / 200.0f;
            it->vy += (targetVy - it->vy) * homingStrength * deltaTime / 200.0f;
        }
        it->update(deltaTime);

        if (it->isOffScreen(screenWidth, screenHeight))
            it = projectiles.erase(it);
        else
            ++it;
    }
}

void ShulkerEnemy::render(SDL_Renderer* renderer) {
    if (texture) {
        if (isInvulnerable) {
            SDL_SetTextureColorMod(texture, 128, 128, 128);
        }
        else {
            SDL_SetTextureColorMod(texture, 255, 255, 255);
        }
        SDL_RenderTexture(renderer, texture, nullptr, &rect);
    }
    else {
        if (isInvulnerable) {
            SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
        }
        else {
            SDL_SetRenderDrawColor(renderer, 100, 100, 0, 255);
        }
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_SetRenderDrawColor(renderer, 200, 0, 255, 255);
    for (auto& p : projectiles)
        SDL_RenderFillRect(renderer, &p.rect);
}

bool ShulkerEnemy::checkCollision(const SDL_FRect& other) {
    if (isInvulnerable) {
        return false;
    }
    return SDL_HasRectIntersectionFloat(&rect, &other);
}

int ShulkerEnemy::getType() const { return 7; }



EndermanEnemy::EndermanEnemy(float px, float py, int sw, int sh) : Enemy(px, py, sw, sh) {}

void EndermanEnemy::update(float deltaTime, Player& player) {
    y += 100.0f * deltaTime;
    rect.x = x;
    rect.y = y;
}

void EndermanEnemy::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_RenderTexture(renderer, texture, nullptr, &rect);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 255, 30, 90, 160);
        SDL_RenderFillRect(renderer, &rect);
    }
}

int EndermanEnemy::getType() const { return 8; }

PhantomEnemy::PhantomEnemy(float px, float py, int sw, int sh) : Enemy(px, py, sw, sh) {}

void PhantomEnemy::update(float deltaTime, Player& player) {
    y += 150.0f * deltaTime;
    moveTimer += deltaTime;
    x += sin(moveTimer * 10.0f) * 200.0f * deltaTime;
    rect.x = x;
    rect.y = y;

    static float shotTimer = 0.0f;
    shotTimer += deltaTime;
    if (shotTimer >= 0.6f) {
        projectiles.push_back({
            x + 12,
            y + 32,
            0,
            350.0f,
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

void PhantomEnemy::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_RenderTexture(renderer, texture, nullptr, &rect);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 0, 191, 255, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    for (auto& p : projectiles) SDL_RenderFillRect(renderer, &p.rect);
}

int PhantomEnemy::getType() const { return 10; }

SculkEnemy::SculkEnemy(float px, float py, int sw, int sh) : Enemy(px, py, sw, sh) {}

void SculkEnemy::update(float deltaTime, Player& player) {
    y += 150.0f * deltaTime;
    moveTimer += deltaTime;
    x += sin(moveTimer * 7.0f) * 200.0f * deltaTime;
    rect.x = x; rect.y = y;

    static float shotTimer = 0.0f;
    shotTimer += deltaTime;
    if (shotTimer >= 0.6f) {
        projectiles.push_back({
            x + 12,
            y + 32,
            0,
            350.0f,
            false,
            {x + rect.w / 2 - 4,
            y + rect.h,
            8,
            8} });
        projectiles.push_back({
            x + 12,
            y + 32,
            -50,
            350.0f,
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

void SculkEnemy::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_RenderTexture(renderer, texture, nullptr, &rect);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 139, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    for (auto& p : projectiles) SDL_RenderFillRect(renderer, &p.rect);
}

int SculkEnemy::getType() const { return 11; }