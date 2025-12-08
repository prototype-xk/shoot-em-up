#include "Select.h"

Select::Select() : inTransition(false) {
    buttonLevel1 = createButton(100, 100, 150, 50, "Level 1");
    buttonLevel2 = createButton(390, 100, 150, 50, "Level 2");
    buttonLevel3 = createButton(100, 330, 150, 50, "Level 3");
    buttonLevel4 = createButton(390, 330, 150, 50, "Level 4");

    // Boutons pour l'ecran de transition
    buttonBackToMenu = createButton(170, 300, 120, 50, "Menu");
    buttonNextWorld = createButton(350, 300, 120, 50, "Next World");
}

void Select::showWorldTransition(int worldNumber) {
    inTransition = true;
    currentWorld = worldNumber;
}

void Select::handleEvent(const SDL_Event& event, int& selectedLevel) {
    if (inTransition) {
        // Gérer les evenements de l'ecran de transition
        handleButtonEvent(&buttonBackToMenu, const_cast<SDL_Event*>(&event));
        handleButtonEvent(&buttonNextWorld, const_cast<SDL_Event*>(&event));

        if (isButtonClicked(&buttonBackToMenu, const_cast<SDL_Event*>(&event))) {
            inTransition = false;
            selectedLevel = -1; // Code special pour retour au menu
        }
        else if (isButtonClicked(&buttonNextWorld, const_cast<SDL_Event*>(&event))) {
            inTransition = false;
            selectedLevel = 4 + currentWorld; // ? Monde suivant !
        }
    }
    else {
        // Gerer les evenements normaux de selection de niveau
        handleButtonEvent(&buttonLevel1, const_cast<SDL_Event*>(&event));
        handleButtonEvent(&buttonLevel2, const_cast<SDL_Event*>(&event));
        handleButtonEvent(&buttonLevel3, const_cast<SDL_Event*>(&event));
        handleButtonEvent(&buttonLevel4, const_cast<SDL_Event*>(&event));

        if (isButtonClicked(&buttonLevel1, const_cast<SDL_Event*>(&event))) {
            selectedLevel = 1;
        }
        else if (isButtonClicked(&buttonLevel2, const_cast<SDL_Event*>(&event))) {
            selectedLevel = 2;
        }
        else if (isButtonClicked(&buttonLevel3, const_cast<SDL_Event*>(&event))) {
            selectedLevel = 3;
        }
        else if (isButtonClicked(&buttonLevel4, const_cast<SDL_Event*>(&event))) {
            selectedLevel = 4;
        }
    }
}


void Select::draw(SDL_Renderer* renderer) {
    if (inTransition) {
        // Dessiner l'ecran de transition
        SDL_SetRenderDrawColorFloat(renderer, 0.2f, 0.2f, 0.3f, 1.0f);
        SDL_RenderFillRect(renderer, nullptr);

        // Pour l'instant, on dessine juste les boutons
        renderButton(renderer, &buttonBackToMenu);
        if (!isFinalWorld) {
            renderButton(renderer, &buttonNextWorld);
        }
    }
    else {
        // Dessiner l'ecran de selection normal
        SDL_SetRenderDrawColorFloat(renderer, 1.0f, 1.0f, 0.0f, 1.0f);
        SDL_RenderFillRect(renderer, nullptr);

        renderButton(renderer, &buttonLevel1);
        renderButton(renderer, &buttonLevel2);
        renderButton(renderer, &buttonLevel3);
        renderButton(renderer, &buttonLevel4);
    }
}