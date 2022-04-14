#pragma once
#ifndef _STATES_H_
#define _STATES_H_

#include "TiledLevel.h"
#include "GameObject.h"
#include "Box.h"
#include "Background.h"
#include"HealthBar.h"
#include "Timer.h"
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL.h>
#include "Primitives.h"
using namespace std;
class State // This is the abstract base class for all states
{
private:
	

public:
	virtual void Enter() = 0; // = 0 means pure virtual - must be defined in subclass
	virtual void Update() = 0;
	virtual void Render();
	virtual void Exit() = 0;
	virtual void Resume();
	virtual ~State() {} // or = default;
	GameObject* GetGo(const std::string& s);
	auto GetIt(const std::string& s);
protected: // Private but inherited
	State() {} // What does this prevent?
	vector<std::pair<std::string, GameObject*>> m_objects;
};

class TitleState : public State
{
public:
	TitleState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
};

class GameState : public State
{
protected:
	

private:

	void Wake();
	void Sleep();
	void HandleEvents();


	void Clean();



	//bool m_bRunning;
	Label* m_label;
	const Uint8* m_iKeystates;
	Uint32 m_start, m_end, m_delta, m_fps; // Fixed timestep variables.
	SDL_Window* m_pWindow; // This represents the SDL window.
	SDL_Renderer* m_pRenderer;
	
	Timer m_timer;
	HealthBar* m_healthBar;// 
	//scrolling psirce part

	vector<Box*> m_vec;
	map<string, Box*> m_protos;

	string m_keys[3] = { "yeti", "abominable", "snowman" };

	int m_gapCtr, m_gapMax;

	vector<Background*> m_vec_background;
	vector<SDL_Rect*> m_sources;

public:


	int Run();
// This static method creates the static instance that can be accessed 'globally'
	bool KeyDown(SDL_Scancode c);
	SDL_Renderer* GetRenderer();

	
	GameState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
	virtual void Resume();
};

#endif

