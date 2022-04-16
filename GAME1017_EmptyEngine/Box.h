#pragma once
#ifndef _BOX_H_
#define _BOX_H_
#include <SDL.h>
#include <string>

class Sprite { //could be reanme ovbsacle for A2

private:
	SDL_Rect m_src;
	SDL_Rect m_dst;
	SDL_Color m_color;
	std::string m_key;


public:
	Sprite();
	Sprite(const SDL_Rect r, const SDL_Color c, const SDL_Rect source, std::string spriteKey);
	//void update();//may need for a2; e.d spining blade

	void Render();
	friend class Box;//atlerantive you can make a setter for m_dst

};

class Box //prodzy for obstpacle/sprite
{
private:
	SDL_Point m_pos;
	Sprite* m_pSprite;
	int m_numSprites;

public:
	Box(const SDL_Point p, bool makeSprite = false, const SDL_Rect r = { 0,0,0,0 },
		const SDL_Color c = { 255, 255, 255, 255 }, const SDL_Rect source = { 0,0,0,0 }, std::string spriteKey = "");

	~Box();
	Box* Clone();
	void Update();
	void Render();
	const SDL_Point GetPos() { return m_pos; }

	const SDL_Rect GetRect() { if (m_pSprite == nullptr) return{ 0,0,0,0 }; return m_pSprite->m_dst; }

	//psrite access

};

#endif

