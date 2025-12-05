#ifndef SELECT_LEVEL_H
#define SELECT_LEVEL_H

#include <SDL3/SDL.h>
#include <stdbool.h>
#include <string.h>

using Button = struct {
    SDL_FRect rect;
    const char* text;
    SDL_Color color;
    SDL_Color hoverColor;
    SDL_Color textColor;
    bool isHovered;
    bool isPressed;
    bool wasPressedOn;
};

// Déclarations des fonctions
Button createButton(float x, float y, float w, float h, const char* text);
bool isPointInButton(Button* btn, float x, float y);
void handleButtonEvent(Button* btn, SDL_Event* e);
bool isButtonClicked(Button* btn, SDL_Event* e);
void renderButton(SDL_Renderer* renderer, Button* btn);

#endif // SELECT_LEVEL_H