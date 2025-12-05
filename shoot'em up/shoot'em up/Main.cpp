#include <SDL3/SDL.h>
#include <string.h>
#include <stdbool.h>
#include <algorithm>

#define MAX_TEXT 33

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("TextBox SDL3", 800, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    SDL_Cursor* cursorIBeam = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_TEXT);
    SDL_Cursor* cursorArrow = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);



    SDL_FRect box = { 100, 100, 400 , 30 };
    char text[MAX_TEXT] = "";
    bool focused = false;

    bool cursorVisible = true;
    Uint32 lastBlink = 0;

    SDL_StartTextInput(window);

    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = false;

            // clic pour focus
            if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT) {
                float x = (float)e.button.x;
                float y = (float)e.button.y;
                focused = (x >= box.x && x <= box.x + box.w &&
                    y >= box.y && y <= box.y + box.h);
            }

            // saisie texte
            if (e.type == SDL_EVENT_TEXT_INPUT && focused) {
                size_t oldTextSize = strnlen_s(text, MAX_TEXT - 1);
                size_t newTextSize = strnlen_s(e.text.text, MAX_TEXT - 1);

                if (oldTextSize < MAX_TEXT - 1)
                    strncat_s(text, e.text.text, newTextSize);
            }

            // backspace
            if (e.type == SDL_EVENT_KEY_DOWN && focused) {
                if (e.key.scancode == SDL_SCANCODE_BACKSPACE && strlen(text) > 0) {
                    text[strlen(text) - 1] = '\0';
                }
            }
        }

        Uint32 now = SDL_GetTicks();
        if (now - lastBlink >= 500) {
            cursorVisible = !cursorVisible;
            lastBlink = now;
        }

        float mx, my;
        SDL_GetMouseState(&mx, &my);

        bool hover = (mx >= box.x && mx <= box.x + box.w &&
            my >= box.y && my <= box.y + box.h);

        if (hover)
            SDL_SetCursor(cursorIBeam);
        else
            SDL_SetCursor(cursorArrow);

        SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderFillRect(renderer, &box);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderRect(renderer, &box);

        SDL_SetRenderScale(renderer, 1.5, 1.5);
        SDL_RenderDebugText(renderer, (box.x + 5)/1.5, (box.y + (box.h / 2) - 8)/1.5, text);
        SDL_SetRenderScale(renderer, 1, 1);

        if (focused && cursorVisible) {
            float cursorX = box.x + 5 + (float)strlen(text)*8.0f*1.5;
            float cursorY = box.y + 5;

            SDL_FRect cursorRect = {
                cursorX,cursorY,2,box.h - 10
            };

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderFillRect(renderer, &cursorRect);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_StopTextInput(window);
    SDL_DestroyCursor(cursorIBeam);
    SDL_DestroyCursor(cursorArrow);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
