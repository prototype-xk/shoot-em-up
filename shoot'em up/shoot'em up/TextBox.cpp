#include "TextBox.h"

TextBox::TextBox()
    :m_box{ 0.0f, 0.0f, 0.0f, 0.0f },
    m_text(""),
    m_maxLength(32),
    m_focused(false),
    m_cursorVisible(true),
    m_lastBlink(0),
    m_cursorIBeam(nullptr),
    m_cursorArrow(nullptr),
    m_font(nullptr)
{
    std::cout << "[INFO] TextBox default constructor called\n";
}

TextBox::TextBox(float x, float y, float width, float height, size_t maxLength,TTF_Font* font)
    : m_box{ x, y, width, height }
    , m_text("")
    , m_maxLength(maxLength)
    , m_focused(false)
    , m_cursorVisible(true)
    , m_lastBlink(0)
    , m_font(font)
{
    m_cursorIBeam = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_TEXT);
    m_cursorArrow = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
    std::cout << "[INFO] TextBox created at (" << x << ", " << y << ")\n";
}

TextBox::~TextBox() {
    SDL_DestroyCursor(m_cursorIBeam);
    SDL_DestroyCursor(m_cursorArrow);
}

void TextBox::handleEvent(const SDL_Event& e) {

    if (e.type == SDL_EVENT_TEXT_INPUT) {
        std::cout << "[DEBUG] Texte input received: " << e.text.text << "\n";
    }

    bool wasFocused = m_focused;
    if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT) {
        float x = static_cast<float>(e.button.x);
        float y = static_cast<float>(e.button.y);
        m_focused = isPointInside(x, y);
        std::cout << "[DEBUG] TextBox focused: " << m_focused << "\n";
    }

    if (e.type == SDL_EVENT_TEXT_INPUT && m_focused) {
        if (m_text.length() < m_maxLength) {
            m_text += e.text.text;
            std::cout << "[DEBUG] Text updated: " << m_text << "\n";
        }
    }

    if (e.type == SDL_EVENT_KEY_DOWN && m_focused) {
        if (e.key.scancode == SDL_SCANCODE_BACKSPACE && !m_text.empty()) {
            m_text.pop_back();
            std::cout << "[DEBUG] Backspace pressed\n";
        }
    }
}

void TextBox::update() {
    Uint32 now = SDL_GetTicks();
    if (now - m_lastBlink >= 500) {
        m_cursorVisible = !m_cursorVisible;
        m_lastBlink = now;
    }
}

void TextBox::render(SDL_Renderer* renderer) {
    updateCursor(renderer);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &m_box);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderRect(renderer, &m_box);
    SDL_SetRenderScale(renderer, 1.5f, 1.5f);
    SDL_RenderDebugText(renderer,
        (m_box.x + 5.0f) / 1.5f,
        (m_box.y + (m_box.h / 2.0f) - 8.0f) / 1.5f,
        m_text.c_str());
    SDL_SetRenderScale(renderer, 1.0f, 1.0f);
    if (m_focused && m_cursorVisible) {
        float cursorX = m_box.x + 5.0f + static_cast<float>(m_text.length()) * 8.0f * 1.5f;
        float cursorY = m_box.y + 5.0f;
        SDL_FRect cursorRect = { cursorX, cursorY, 2.0f, m_box.h - 10.0f };

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &cursorRect);
    }
}

void TextBox::setPosition(float x, float y) {
    m_box.x = x;
    m_box.y = y;
}

void TextBox::setSize(float width, float height) {
    m_box.w = width;
    m_box.h = height;
}

void TextBox::setText(const std::string& newText) {
    m_text = newText.substr(0, m_maxLength);
}

std::string TextBox::getText() const {
    return m_text;
}

void TextBox::clear() {
    m_text.clear();
}

void TextBox::setFocused(bool focus) {
    m_focused = focus;
}

bool TextBox::isFocused() const {
    return m_focused;
}

bool TextBox::isPointInside(float x, float y) const {
    return (x >= m_box.x && x <= m_box.x + m_box.w &&
        y >= m_box.y && y <= m_box.y + m_box.h);
}

void TextBox::updateCursor(SDL_Renderer* renderer) {
    float mx, my;
    SDL_GetMouseState(&mx, &my);

    if (isPointInside(mx, my)) {
        SDL_SetCursor(m_cursorIBeam);
    }
    else {
        SDL_SetCursor(m_cursorArrow);
    }
}