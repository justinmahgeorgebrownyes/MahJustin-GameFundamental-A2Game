#pragma once

#ifndef _HEALTHBAR_H_
#define _HEALTHBAR_H_
#include "GameObject.h"
#endif // !_HEALTHBAR_H_


class HealthBar : public GameObject
{
private:
	SDL_FRect m_bar;
	//using m-dst from gameovect as bacgroudn bar
	int m_health;

public:
	HealthBar(/*agent pointer*/ int maxHealth);
	void Update();
	void Render();
	void TakeDamage(int amount);
};

