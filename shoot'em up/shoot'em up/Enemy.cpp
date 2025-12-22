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

BasicEnemy::BasicEnemy(float px, float py,int sw,int sh) : Enemy(px, py,sw,sh) {}

void BasicEnemy::update(float deltaTime, Player& player) {
    y += 100.0f * deltaTime;
    rect.x = x;
    rect.y = y;

    float shotTimer = 0.0f;
    shotTimer += deltaTime;
    if (shotTimer >= 2.0f) {
        projectiles.push_back({
            x + 12,
            y + 32,
            0,
            200.0f,
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
void BasicEnemy::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 62, 137, 134, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    for (auto& p : projectiles) SDL_RenderFillRect(renderer, &p.rect);
}

int BasicEnemy::getType() const { return 23; }

ZigzagEnemy::ZigzagEnemy(float px, float py, int sw, int sh) : Enemy(px, py,sw,sh) {}

void ZigzagEnemy::update(float deltaTime, Player& player) {
    y += 100.0f * deltaTime;
    moveTimer += deltaTime;
    x += sin(moveTimer * 5.0f) * 150.0f * deltaTime;
    rect.x = x; rect.y = y;

    float shotTimer = 0.0f;
    shotTimer += deltaTime;
    if (shotTimer >= 1.0f) {
        projectiles.push_back({
            x + 12,
            y + 32,
            0,
            250.0f,
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
void ZigzagEnemy::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 58, 164, 118, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    for (auto& p : projectiles) SDL_RenderFillRect(renderer, &p.rect);
}

int ZigzagEnemy::getType() const { return 24; }

Drowned::Drowned(float px, float py, int sw, int sh) : Enemy(px, py, sw, sh) {}

void Drowned::update(float deltaTime, Player& player)
{
    y += 50.f * deltaTime;
    float vx = 0.0f;
    float dx = player.x - x;
    float distance = sqrt(dx * dx);

    if (distance > 0.0f) {
        float speed = 100.0f;
        float targetVx = (dx / distance) * speed;
        float homingStrength = 5.0f;
        vx += (targetVx - vx) * homingStrength * deltaTime;
        x += vx * deltaTime;
    }

    rect.x = x;
    rect.y = y;

    float shotTimer = 1.5f;
    shotTimer += deltaTime;
    if (shotTimer >= 2.0f) {
        projectiles.push_back({
            x + 12,
            y + 32,
            0,
            250.0f,
            false,
            {x + rect.w / 2 - 4, y + rect.h, 8, 8}
            });
        shotTimer = 1.5f;
    }
    for (auto it = projectiles.begin();
        it != projectiles.end(); ) {

        it->update(deltaTime);
        if (it->isOffScreen(screenWidth, screenHeight))
            it = projectiles.erase(it);
        else ++it;
    }
}

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

SkeletonEnemy::SkeletonEnemy(float px, float py, int sw, int sh) : Enemy(px, py, sw, sh) {}

void SkeletonEnemy::update(float deltaTime, Player& player) {
    y += 50.0f * deltaTime;
    rect.x = x;
    rect.y = y;

    float shotTimer = 0.0f;
    shotTimer += deltaTime;
    if (shotTimer >= 2.0f) {
        projectiles.push_back({
            x + 12,
            y + 32,
            0,
            200.0f,
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

    float shotTimer = 0.0f;
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

WitherBoss::WitherBoss(float px, float py, int sw, int sh) : Enemy(px, py, sw, sh) {}

void WitherBoss::update(float deltaTime, Player& player) {
    rect.x = x;
    rect.y = y;

    float shotTimer = 0.0f;
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

    float shotTimer = 0.0f;
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

DragonBoss::DragonBoss(float px, float py, int sw, int sh) : Enemy(px, py,sw,sh) {
    rect.w = 180;
    rect.h = 250;
}

void DragonBoss::update(float deltaTime, Player& player) {
    float shotTimer = 0.0f;
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

    float shotTimer = 0.0f;
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

    float shotTimer = 0.0f;
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

WardenBoss::WardenBoss(float px, float py, int sw, int sh, int& wardenHealthRef) : Enemy(px, py, sw, sh), wardenHealthRef(wardenHealthRef) {
    maxHealth = 150;
    rect.w = 120;
    rect.h = 100;
}

void WardenBoss::update(float deltaTime, Player& player) {
    moveTimer += deltaTime;
    float changeDirectionTimer = 0.0f;
    float direction = -1.0f;
    changeDirectionTimer += deltaTime;
    random rd;

    int currentPhase = 0;
    if (wardenHealthRef <= 120) currentPhase = 1;
    if (wardenHealthRef <= 80) currentPhase = 2;
    if (wardenHealthRef <= 40) currentPhase = 3;
    if (wardenHealthRef <= 15) currentPhase = 4;
    float directionChangeDelay = 1.5f - (currentPhase * 0.1f);
    if (changeDirectionTimer >= directionChangeDelay + rd.getRandomNumber(0, 80) / 100.0f) {
        int randomChoice = rd.getRandomNumber(0, 3);
        if (randomChoice == 0) direction = -1.0f;
        else if (randomChoice == 1) direction = 1.0f;
        else direction = 0.0f;
        changeDirectionTimer = 0.0f;
    }

    float baseSpeed = 100 + (currentPhase * 50);
    float speed = rd.getRandomNumber(baseSpeed, baseSpeed + 200);
    x += direction * speed * deltaTime;

    if (x < 0) x = 0;
    else if (x + rect.w > screenWidth) x = screenWidth - rect.w;
    rect.x = x;
    rect.y = y;

    float shotTimer = 0.0f;
    float specialAttackTimer = 0.0f;
    float spiralTimer = 0.0f;
    float waveTimer = 0.0f;
    float phantomSpawnTimer = 0.0f;
    float sculkSpawnTimer = 0.0f;

    shotTimer += deltaTime;
    specialAttackTimer += deltaTime;
    spiralTimer += deltaTime;
    waveTimer += deltaTime;
    phantomSpawnTimer += deltaTime;
    sculkSpawnTimer += deltaTime;
    float shotDelay = 0.9f;
    int numProjectiles = 4;
    float spreadAngle = 0.5f;
    if (currentPhase >= 1) {
        shotDelay = 0.6f;
        numProjectiles = 5;
        spreadAngle = 0.6f;
        if (waveTimer >= 3.0f) {
            int numColumns = 4;
            float columnSpacing = screenWidth / (numColumns + 1.0f);

            for (int col = 0; col < numColumns; col++) {
                float xPos = columnSpacing * (col + 1);
                projectiles.push_back({
                    xPos,
                    y + rect.h / 2,
                    0.0f,
                    200.0f,
                    false,
                    {xPos - 8, y + rect.h / 2 - 8, 16, 16}
                    });
            }
            waveTimer = 0.0f;
        }
    }
    if (currentPhase >= 2) {
        shotDelay = 0.5f;
        numProjectiles = 0;
        spreadAngle = 0.8f;
        if (spiralTimer >= 0.2f) {
            float spiralAngle = 0.0f;
            spiralAngle += 0.4f;
            for (int i = 0; i < 3; i++) {
                float angle = spiralAngle + (i * 3.14159f * 2 / 3);
                projectiles.push_back({
                    x + rect.w / 2,
                    y + rect.h / 2,
                    cos(angle) * 120.0f,
                    sin(angle) * 120.0f,
                    false,
                    {x + rect.w / 2 - 6, y + rect.h / 2 - 6, 12, 12}
                    });
            }
            spiralTimer = 0.0f;
        }
        if (specialAttackTimer >= 4.0f) {
            for (int i = 0; i < 10; i++) {
                float angle = (i / 10.0f) * 3.14159f * 2;
                projectiles.push_back({
                    x + rect.w / 2,
                    y + rect.h / 2,
                    cos(angle) * 180.0f,
                    sin(angle) * 180.0f,
                    false,
                    {x + rect.w / 2 - 6, y + rect.h / 2 - 6, 12, 12}
                    });
            }
            specialAttackTimer = 0.0f;
        }
    }
    if (currentPhase >= 3) {
        shotDelay = 0.3f;
        numProjectiles = 0;
        spreadAngle = 1.0f;
        if (spiralTimer >= 0.15f) {
            float spiralAngle = 0.0f;
            spiralAngle += 0.5f;
            for (int spiral = 0; spiral < 2; spiral++) {
                for (int i = 0; i < 2; i++) {
                    float angle = spiralAngle + (i * 3.14159f) + (spiral * 3.14159f / 2);
                    projectiles.push_back({
                        x + rect.w / 2,
                        y + rect.h / 2,
                        cos(angle) * (150.0f + spiral * 20.0f),
                        sin(angle) * (150.0f + spiral * 20.0f),
                        false,
                        {x + rect.w / 2 - 6, y + rect.h / 2 - 6, 12, 12}
                        });
                }
            }
            spiralTimer = 0.0f;
        }
        if (specialAttackTimer >= 3.0f) {
            int numCols = 5;
            float spacing = screenWidth / (numCols + 1.0f);

            for (int i = 0; i < numCols; i++) {
                float xPos = spacing * (i + 1);
                projectiles.push_back({
                    xPos,
                    y + rect.h / 2,
                    0.0f,
                    250.0f,
                    false,
                    {xPos - 8, y + rect.h / 2 - 8, 16, 16}
                    });
            }
            specialAttackTimer = 0.0f;
        }
    }
    if (currentPhase >= 4) {
        shotDelay = 0.2f;
        numProjectiles = 0;
        spreadAngle = 1.2f;
        if (spiralTimer >= 0.12f) {
            float spiralAngle = 0.0f;
            spiralAngle += 0.6f;
            for (int spiral = 0; spiral < 3; spiral++) {
                float angle = spiralAngle + (spiral * 3.14159f * 2 / 3);
                projectiles.push_back({
                    x + rect.w / 2,
                    y + rect.h / 2,
                    cos(angle) * (170.0f + spiral * 30.0f),
                    sin(angle) * (170.0f + spiral * 30.0f),
                    false,
                    {x + rect.w / 2 - 7, y + rect.h / 2 - 7, 14, 14}
                    });
            }
            spiralTimer = 0.0f;
        }
        if (specialAttackTimer >= 2.5f) {
            int alternatePattern = 0;
            alternatePattern = (alternatePattern + 1) % 2;
            for (int i = 0; i < 12; i++) {
                float angle = (i / 12.0f) * 3.14159f * 2 + (alternatePattern * 0.22f);
                projectiles.push_back({
                    x + rect.w / 2,
                    y + rect.h / 2,
                    cos(angle) * 220.0f,
                    sin(angle) * 220.0f,
                    false,
                    {x + rect.w / 2 - 8, y + rect.h / 2 - 8, 16, 16}
                    });
            }
            specialAttackTimer = 0.0f;
        }
        if (waveTimer >= 2.0f) {
            int numCols = 10;
            float spacing = screenWidth / (numCols + 1.0f);

            int offset = 0;
            offset = (offset + 1) % 2;

            for (int i = offset; i < numCols; i += 2) {
                float xPos = spacing * (i + 1);
                projectiles.push_back({
                    xPos,
                    y + rect.h / 2,
                    0.0f,
                    300.0f,
                    false,
                    {xPos - 10, y + rect.h / 2 - 10, 20, 20}
                    });
            }
            waveTimer = 0.0f;
        }
    }

    // Tir principal (NERF : Vitesse réduite)
    if (numProjectiles > 0 && shotTimer >= shotDelay) {
        for (int i = 0; i < numProjectiles; i++) {
            float angle = (i - (numProjectiles - 1) / 2.0f) * spreadAngle;
            projectiles.push_back({
                x + rect.w / 2,
                y + rect.h,
                sin(angle) * 80.0f,
                350.0f + (currentPhase * 40.0f),
                false,
                {x + rect.w / 2 - 4 + i * 8, y + rect.h, 8, 8}
                });
        }
        shotTimer = 0.0f;
    }
    if (currentPhase >= 2 && phantomSpawnTimer >= 6.0f + rd.getRandomNumber(0, 200) / 100.0f) {
        phantomSpawnTimer = 0.0f;
    }
    if (currentPhase >= 3 && sculkSpawnTimer >= 9.0f + rd.getRandomNumber(0, 200) / 100.0f) {
        sculkSpawnTimer = 0.0f;
    }

    // Update projectiles
    for (auto it = projectiles.begin(); it != projectiles.end();) {
        it->update(deltaTime);
        if (it->isOffScreen(screenWidth, screenHeight))
            it = projectiles.erase(it);
        else ++it;
    }
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