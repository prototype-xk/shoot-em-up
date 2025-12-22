#include "Projectile.h"

Projectile::Projectile(float x, float y, float vx, float vy, bool isPlayer, SDL_FRect rect, SDL_Texture* tex)
    : x(x), y(y), vx(vx), vy(vy), isPlayer(isPlayer), rect(rect), texture(tex) {
}

void Projectile::update(float deltaTime) {
    x += vx * deltaTime;
    y += vy * deltaTime;
    rect.x = x;
    rect.y = y;
}

bool Projectile::isOffScreen(int w, int h) {
	return x<0 || x>w || y<0 || y>h;
}