#include "States.h"
#include "StateManager.h"
#include "TextureManager.h"
#include "Engine.h"
#include "EventManager.h"
#include "CollisionManager.h"
#include "SoundManager.h"
#include "Primitives.h"
#include "Button3.h"
#include "PlatformPlayer.h"
#include "GameObject.h"
#include "FontManager.h"
#include <iostream>
#include <ctime>

#define WIDTH 1024
#define HEIGHT 768
#define FPS 60

using namespace std;

void State::Render()
{
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}

void State::Resume() {}

GameObject* State::GetGo(const std::string& s)
{ // Using 'std' just to show origin.
	auto it = std::find_if(m_objects.begin(), m_objects.end(),
		// Lambda expression/function. An in-function function.
		[&](const std::pair<std::string, GameObject*>& element)
		{
			return element.first == s;
		}
	); // End of find_if.
	if (it != m_objects.end())
		return it->second;
	else return nullptr;
}

auto State::GetIt(const std::string& s)
{
	auto it = std::find_if(m_objects.begin(), m_objects.end(),
		[&](const std::pair<std::string, GameObject*>& element)
		{
			return element.first == s;
		});
	return it;
}

// Begin TitleState
TitleState::TitleState() {}

void TitleState::Enter()
{

	TEMA::Load("Img/Title.png", "title");


	TEMA::Load("Img/button.png", "play");
	m_objects.push_back(pair<string, GameObject*>("title",
		new Image({ 0, 0, 10000, 1600 }, { 112, 100, 800, 500 }, "title")));
	m_objects.push_back(pair<string, GameObject*>("play",
		new PlayButton({ 0, 0, 400, 100 }, { 412, 350, 200, 50 }, "play")));

}

void TitleState::Update()
{
	for (auto const& i : m_objects)
	{
		i.second->Update();
		if (STMA::StateChanging()) return;
	}
}

void TitleState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 64, 0, 16, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	for (auto const& i : m_objects)
		i.second->Render();
	State::Render();
}

void TitleState::Exit()
{
	TEMA::Unload("play");
	for (auto& i : m_objects)
	{
		delete i.second;
		i.second = nullptr;
	}
	m_objects.clear();
	m_objects.shrink_to_fit();
}
// End TitleState

// Begin GameState
GameState::GameState(){



}

void GameState::BackgroundMusic() {

	// play music forever
	// Mix_Music *music; // I assume this has been loaded already
	if (Mix_PlayMusic(m_backmusic, -1) == -1) {
		printf("Mix_PlayMusic: %s\n", Mix_GetError());
		// well, there's no music, but most games don't break without music...
	}

}


void GameState::Enter() // Used for initialization.
{

	m_vec.reserve(9);
	for (int i = 0; i < 9; i++) {
		m_vec.push_back(new Box({ 128 * i, 384 }));
	}
	FOMA::Load("Img/ltype.TTF", "Label", 24);

	m_fps = (Uint32)round((1 / (double)FPS) * 1000); // Sets FPS in milliseconds and rounds.
	m_iKeystates = SDL_GetKeyboardState(nullptr);
	srand((unsigned)time(NULL)); // Seed random number sequence.
	m_label = new Label("Label", 300, 300, "Time: 0");
	m_timer.Start();

	m_healthBar = new HealthBar(100);


	SOMA::Load("Aud/Background.mp3", "background", SOUND_MUSIC);
	GameState::BackgroundMusic();

	Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 2048);
	Mix_AllocateChannels(16);

	m_backmusic = Mix_LoadMUS("Aud\\background.mp3");
	SOMA::AllocateChannels(16);
	SOMA::SetMusicVolume(32);
	SOMA::PlayMusic("background", -1, 2000);


	m_exitButtonRect = { 112, 100, 1000, 250 };

	TEMA::Load("Img/BG.png", "bg");

	TEMA::Load("Img/Exit.png", "exit");
	m_objects.push_back(pair<string, GameObject*>("exit",
		new Image({ 0, 0, 10000, 800 }, { 112, 100, 1000, 250 }, "exit")));

	// Create the vector now.
	 m_vec_background.reserve(10);//to prevent growth of 1.5x in push_back
	m_sources.reserve(3);


	// Backgrounds.
	m_sources.push_back(new SDL_Rect{ 0,0,1024,768 });
	 m_vec_background.push_back(new Background(m_sources[0], { 0,0,1024,768 }, 1));
	 m_vec_background.push_back(new Background(m_sources[0], { 1024,0,1024,768 }, 1));


	// Midgrounds.
	m_sources.push_back(new SDL_Rect{ 1024,0,256,512 });

	 m_vec_background.push_back(new Background(m_sources[1], { 0,0,256,512 }, 3));
	 m_vec_background.push_back(new Background(m_sources[1], { 256,0,256,512 }, 3));
	 m_vec_background.push_back(new Background(m_sources[1], { 512,0,256,512 }, 3));
	 m_vec_background.push_back(new Background(m_sources[1], { 768,0,256,512 }, 3));
	 m_vec_background.push_back(new Background(m_sources[1], { 1024,0,256,512 }, 3));

	// Foregrounds.
	m_sources.push_back(new SDL_Rect{ 1024,512,512,256 });

	 m_vec_background.push_back(new Background(m_sources[2], { 0,512,512,256 }, 4));
	 m_vec_background.push_back(new Background(m_sources[2], { 512,512,512,256 }, 4));
	 m_vec_background.push_back(new Background(m_sources[2], { 1024,512,512,256 }, 4));



	TEMA::Load("Img/yeti.png", "yeti");//390 * 443
	TEMA::Load("Img/abominable.png", "abominable");//340 * 417
	TEMA::Load("Img/snowman.png", "snowman");//1000 * 1000

	m_protos.emplace("yeti", new Box({ 1024, 384 }, true, { 1024, 384, 128, 128 }, { 255, 64, 128, 255 }, { 0, 0, 390, 443 }, "yeti"));
	m_protos.emplace("abominable", new Box({ 1024, 384 }, true, { 1056, 0, 64, 384 }, { 64, 255, 32, 255 }, { 0, 0, 340, 417 }, "abominable"));
	m_protos.emplace("snowman", new Box({ 1024, 384 }, true, { 1024, 448, 128, 64 }, { 234, 215, 84, 255 }, { 0, 0, 1000, 1000 }, "snowman"));

	m_gapCtr = 0;
	m_gapMax = 3;



	TEMA::Load("Img/Tiles.png", "tiles");

	//loda player image and give it a string key of "player"..

	TEMA::Load("Img/Player.png", "player");


	m_objects.push_back(pair<string, GameObject*>("level", new TiledLevel(
		24, 32, 32, 32, "Dat/Tiledata.txt", "Dat/Level1.txt", "tiles")));
	m_objects.push_back(pair<string, GameObject*>("player", new PlatformPlayer({ 0,0,0,0 }, { 288, 480, 64, 64 }, 128, 128)));


}

void GameState::Update()
{


	if (EventManager::ButtonPressed(1, m_exitButtonRect)) {
		cout << "exiting" << endl;

		GameState::Exit();
	}

	



	m_timer.Update();
	//scrollingpaart
	for (auto bg : m_vec_background)
	bg->Update();


	SDL_FRect* playerFRect = GetGo("player")->GetDst();
	SDL_Rect playerRect = { (int)playerFRect->x , (int)playerFRect-> y, (int)playerFRect->w, (int)playerFRect->h };

	for (int i = 0; i < m_vec.size(); i++)
	{
		SDL_Rect hi = m_vec[i]->GetRect();


		bool hhello = SDL_HasIntersection(&playerRect, &(hi));

		if (hhello == SDL_TRUE) {
			STMA::ChangeState(new LoseState());
			return;
		}
	
	}

	
	

	//if (GetGo("player") != nullptr)
	//{
	//	// BUT OUR CODE IS IN ANOTHER CASTLE!
	//	// Creating some temporary fields (all as pointers for consistency) for convenience.
	//	//vector<Box*>* field = &static_cast<Box*>(GetGo("player")->GetDst());
	//	PlatformPlayer* ship = static_cast<PlatformPlayer*>(GetGo("player"));
	//	// Player vs. asteroids first.
	//	for (unsigned int i = 0; i < m_vec.size(); i++)
	//	{
	//		Box* ast = m_vec[i];
	//		if (COMA::CircleCircleCheck(ship->GetDst(), ast->GetRect(),
	//			ship->GetDst(), ast->GetRect()) )
	//		{
	//			SOMA::PlaySound("explode");
	//			delete ship;
	//			m_objects.erase(GetIt("player")); // Erases whole ship std::pair.
	//			m_objects.shrink_to_fit();
	//			return;
	//		}
	//	}
	//}



	//from scrolling sprites part
		// Check if first column of main vector goes out of bounds.
	if (m_vec[0]->GetPos().x <= -128) {
		delete m_vec[0];
		m_vec.erase(m_vec.begin());

		if (m_gapCtr++ % m_gapMax == 0) {
			SDL_Color col = { 100 + rand() % 156, 100 + rand() % 156, 100 + rand() % 156, 255 };

			m_vec.push_back(m_protos[m_keys[rand() % 3]]->Clone());

			//m_vec.push_back(new Box({ 1024, 284 }, true, { 1024, 384, 128, 128 }, col));
		}
		else m_vec.push_back(new Box({ 1024, 384 }));
	}



	// Scroll the boxes.

	//update the boxes which scrol themselves
	for (unsigned int i = 0; i < m_vec.size(); i++) {
		m_vec[i]->Update();
	}

	if (EVMA::KeyPressed(SDL_SCANCODE_P)) {
		cout << "changing to pausestate" << endl;
		//pause the music track
		STMA::PushState(new PauseState());
	}

	if (EVMA::KeyPressed(SDL_SCANCODE_X))
	{
		STMA::ChangeState(new TitleState());
		return;
	}

	if (EVMA::KeyPressed(SDL_SCANCODE_D)) {
		cout << "walking sounds" << endl;
		//pause the music track

		SOMA::Load("Aud/Walk.wav", "walk", SOUND_MUSIC);
		GameState::BackgroundMusic();

		Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 2048);
		Mix_AllocateChannels(16);

		m_backmusic = Mix_LoadMUS("Aud\\Walk.wav");
		SOMA::AllocateChannels(16);
		SOMA::SetMusicVolume(32);
		SOMA::PlayMusic("walk", -1, 2000);
	}

	for (auto const& i : m_objects)
	{
		i.second->Update();
		if (STMA::StateChanging()) return;
	}
	//check collision
	PlatformPlayer* pObj = static_cast<PlatformPlayer*>(GetGo("player"));
	SDL_FRect* pBound = pObj->GetDst();
	TiledLevel* pLevel = static_cast<TiledLevel*>(GetGo("level"));

	for (unsigned int i = 0; i < pLevel->GetObstacles().size(); i++) {
		SDL_FRect* pTile = pLevel->GetObstacles()[i]->GetDst();
		if (COMA::AABBCheck(*pBound, *pTile)) {

			bool colTop = false;

			//top side
			if ((pBound->y + pBound->h) - (float)pObj->GetVelY() <= pTile->y) {
				colTop = true;
				pObj->StopY();
				pObj->SetY(pTile->y - pBound->h);
				pObj->SetGrounded(true);
			}
			//	if colliding with bottom side of tile
			else if (pBound->y - (float)pObj->GetVelY() >= (pTile->y + pTile->h)) {
				pObj->StopY();
				pObj->SetY(pTile->y + pTile->h);
			}
			//left side
			if ((pBound->x + pBound->w) - (float)pObj->GetVelX() <= pTile->x) {
				pObj->StopX();
				pObj->SetX(pTile->x - pTile->w);
			}
			//right side
			else if (pBound->x - (float)pObj->GetVelX() >= (pTile->x + pTile->w)) {
				pObj->StopX();
				pObj->SetX(pTile->x + pTile->w);
			}
		}
	}

}




// Keyboard utility function.
bool GameState::KeyDown(SDL_Scancode c)
{
	if (m_iKeystates != nullptr)
	{
		if (m_iKeystates[c] == 1)
			return true;
		else
			return false;
	}
	return false;
}


int GameState::Run()
{
	
	return 0;
}


void GameState::Render()
{

	// Render stuff.
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());


	for (auto bg : m_vec_background)
		bg->Render();


	//timers
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer); // Clear the screen with the draw color.
	// Render stuff.
	if (m_timer.HasChanged()) {
		string temp = "Time: " + m_timer.GetTime();
		m_label->SetText(temp.c_str());
	}

	m_label->Render();
	m_healthBar->Render();
	// Draw anew.

	SDL_RenderPresent(m_pRenderer);



	for (auto const& i : m_objects)
		i.second->Render();


	//
	//herw we draw a transparent box
	//SDL_SetRenderDrawBlendMode(Engine::Instance().GetRenderer(), SDL_BLENDMODE_BLEND);
	//SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 0, 0, 128);
	//SDL_Rect rect = { 255, 128, 512, 512 };
	//SDL_RenderFillRect(Engine::Instance().GetRenderer(), &rect);




	//here we drwaw a non transparent resume button.
	SDL_SetRenderDrawBlendMode(Engine::Instance().GetRenderer(), SDL_BLENDMODE_NONE);
	//SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 0, 0, 128);
	//SDL_RenderFillRect(Engine::Instance().GetRenderer(), &m_exitButtonRect);


	//sprite scrolling
	for (unsigned int i = 0; i < m_vec.size(); i++) {
		m_vec[i]->Render();

	}






	
	if (dynamic_cast<GameState*>(STMA::GetStates().back()))
		State::Render();
}

void GameState::Exit()
{

	cout << "Cleaning game." << endl;
	delete m_healthBar;
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	FOMA::Quit();
	TEMA::Quit();
	IMG_Quit();
	SDL_Quit();
	Mix_FreeMusic(m_backmusic);
	Mix_CloseAudio();
	Mix_Quit();


	for (auto& i : m_objects)
	{
		delete i.second;
		i.second = nullptr;
	}
	m_objects.clear();
	m_objects.shrink_to_fit();



	for (unsigned int i = 0; i < m_vec.size(); i++) {
		delete m_vec[i];
		m_vec[i] = nullptr;
	}
}

void GameState::Resume() {}
// End GameState



LoseState::LoseState()
{
	cout << "you lose";
}

void LoseState::Enter()
{
//;
}

void LoseState::Update()
{
}

void LoseState::Render()
{
}

void LoseState::Exit()
{
}



PauseState::PauseState()
{
	m_resumeButtonRect = { 255, 128, 512, 512 };

}

void PauseState::Enter()
{
	
	//m_backgroundImage = IMG_LoadTexture(m_pRenderer, "Img\\background_ccexpress.jpeg");
	//m_startImage = IMG_LoadTexture(m_pRenderer, "Img\\start.png");
	//m_exitButtonImage = IMG_LoadTexture(m_pRenderer, "Img\\exit.jfif");
	m_resumeButtonTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img\\resume.png");
}

void PauseState::Update()
{

	if (EventManager::ButtonPressed(1, m_resumeButtonRect)){
		cout << "changing to gamstate" << endl;
		STMA::ChangeState(new GameState());
	}
}

void PauseState::Render()
{
	STMA::GetStates().front()->Render();

	//herw we draw a transparent box
	SDL_SetRenderDrawBlendMode(Engine::Instance().GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 0, 0, 128);
	SDL_Rect rect = { 255, 128, 512, 512 };
	SDL_RenderFillRect(Engine::Instance().GetRenderer(), &rect);




	//here we drwaw a non transparent resume button.
	SDL_SetRenderDrawBlendMode(Engine::Instance().GetRenderer(), SDL_BLENDMODE_NONE);
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 0, 0, 128);
	SDL_RenderFillRect(Engine::Instance().GetRenderer(), &m_resumeButtonRect);

	//Engine::Instance().ResumeButton(&m_button2);

	SDL_RenderCopy(Engine::Instance().GetRenderer(), m_resumeButtonTexture, NULL, &m_resumeButtonRect);

	State::Render();
}

void PauseState::Exit()
{
}

