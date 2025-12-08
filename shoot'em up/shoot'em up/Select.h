#pragma once
#include <SDL3/SDL.h>
#include "Button.h"

class Select
{
public:
    Select();
    void handleEvent(const SDL_Event& event, int& selectedLevel);
    void draw(SDL_Renderer* renderer);

    // Nouvelle methode pour afficher l'ecran de transition
    void showWorldTransition(int worldNumber);
    bool isInTransition() const { return inTransition; }

private:
    Button buttonLevel1;
    Button buttonLevel2;
    Button buttonLevel3;
    Button buttonLevel4;

    // Boutons pour l'ecran de transition
    Button buttonBackToMenu;
    Button buttonNextWorld;

    // Etat de transition
    bool inTransition;
    int currentWorld;
    bool isFinalWorld;
};
