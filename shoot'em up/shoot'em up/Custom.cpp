#include "Custom.h"

Custom::Custom(SDL_Window* window, TTF_Font* font) : font(font)
{
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    buttonWidth = windowWidth * 0.25;
    buttonHeight = 50;
    buttonX = (windowWidth - buttonWidth) / 2;
    buttonY = windowHeight * 0.6;
    textboxWidth = windowWidth * 0.5;
    textboxHeight = 40;
    textboxX = (windowWidth - textboxWidth) / 2;
    textboxY = windowHeight * 0.4;
    titleY = textboxY - 60;

    button = createButton(buttonX, buttonY, buttonWidth, buttonHeight, "Continue ");
    titleLabel = Label(windowWidth / 2 - 100, titleY, "Enter your name :", 1.5f);
    textbox = TextBox(textboxX, textboxY, textboxWidth, textboxHeight, 32);
    titleLabel.setScale(2);
    titleLabel.setColor(255, 255, 255);
}

void Custom::handleEvent(const SDL_Event& event, bool& shouldSwitchToSelect) {
    handleButtonEvent(&button, const_cast<SDL_Event*>(&event));
    if (isButtonClicked(&button, const_cast<SDL_Event*>(&event))) {
        shouldSwitchToSelect = true;
    }

    textbox.handleEvent(event);
}

void Custom::update() {
    textbox.update();
}

void Custom::draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColorFloat(renderer, 1.0f, 0.5f, 0.0f, 1.0f);
    SDL_RenderFillRect(renderer, nullptr);

    titleLabel.render(renderer);
    textbox.render(renderer);

    renderButton(renderer, &button, font);
}