#include "Background.h"
#include "Engine.h"
#include "TextureManager.h"
#include <iostream>

Background::Background(const SDL_Rect* s, const SDL_FRect d, float ss) :SpriteObject(*s, d), m_scrollSpeed(ss)
{
	m_startX = GetDst()->x;
	m_endX = m_startX - GetDst()->w;
	std::cout << "Creating background, size of " << sizeof(*this) << std::endl;

}

void Background::Update()
{
	//chekc if at end of journey.
	if (GetDst()->x <= m_endX)
		GetDst()->x = m_startX;
	//scrool the background.
	GetDst()->x -= m_scrollSpeed;
}

void Background::Render()
{
	SDL_RenderCopyF(Engine::Instance().GetRenderer(), TEMA::GetTexture("bg"), GetSrc(), GetDst());
}
