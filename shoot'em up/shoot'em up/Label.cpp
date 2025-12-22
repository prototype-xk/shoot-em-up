#include "Label.h"
Label::Label(float x, float y, const std::string& text, float scale)
	: m_x(x)
	, m_y(y)
	, m_text(text)
	, m_scale(scale)
	, m_r(255)
	, m_g(255)
	, m_b(255)
	, m_a(255)
{
}

void Label::render(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, m_r, m_g, m_b, m_a);

	SDL_RenderDebugText(renderer, m_x / m_scale, m_y / m_scale, m_text.c_str());

	SDL_SetRenderScale(renderer, 1.0f, 1.0f);
}

void Label::setPosition(float x, float y) {
	m_x = x;
	m_y = y;
}

void Label::setScale(float scale) {
	m_scale = scale;
}

void Label::setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	m_r = r;
	m_g = g;
	m_b = b;
	m_a = a;
}

std::string Label::getText() const {
	return m_text;
}

float Label::getX() const {
	return m_x;
}

float Label::getY() const {
	return m_y;
}