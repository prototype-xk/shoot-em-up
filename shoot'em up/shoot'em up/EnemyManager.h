#pragma once
#include <SDL3/SDL.h>
#include "Enemy.h"
#include "EnemyBoss.h"
#include <vector>
#include <memory>
#include <cmath>
#include "random.h"
#include "Projectile.h"

std::unique_ptr<Enemy> createEnemy(int type, float x, float y, int screenWidth, int screenHeight, SDL_Renderer* renderer);