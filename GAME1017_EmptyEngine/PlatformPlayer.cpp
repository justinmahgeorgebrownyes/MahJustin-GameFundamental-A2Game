#include "PlatformPlayer.h"
#include "EventManager.h"
#include "TextureManager.h"
#include <cmath>
#include "States.h"

PlatformPlayer::PlatformPlayer(SDL_Rect s, SDL_FRect d, const int w, const int h) : AnimatedSpriteObject(s, d),

m_state(STATE_JUMPING), m_isGrounded(false), m_isFacingLeft(false),
m_maxVelX(9.0), m_maxVelY(JUMPFORCE), m_grav(GRAVITY), m_drag(0.85)


{
	m_accelX = m_accelY = m_velX = m_velY = 0.0;

	char key;
	m_playerkey = 'a';
	for (int y = 0; y < 2; y++)
	{
		for (int x = 0; x < 9; x++) {

			key = (char)'a' + x + (9 * y);




			m_tiles.emplace(key, new Tile({ x * w, y * h, w, h },
				{ 0.0f, 0.0f, (float)64, (float)64 }, 0, 0));
		}
	}






}


void PlatformPlayer::Update()
{
	switch (m_state) { //inside each case is the behaving in and transiton from that state
	case STATE_IDLING:
		//transitoin to run

		m_playerkey = (char)'a' + 9 + (0 % 4);



		if (EVMA::KeyPressed(SDL_SCANCODE_A) || EVMA::KeyPressed(SDL_SCANCODE_D)) {



			m_state = STATE_RUNNING;
			//SetAnimation(?,?,?,?);
			SetAnimation(288, 480, 64, 64);
		}
		//transition to jump
		else if (EVMA::KeyPressed(SDL_SCANCODE_SPACE) && m_isGrounded) {

			m_accelY = -JUMPFORCE;
			m_isGrounded = false;
			m_state = STATE_JUMPING;
			//set animation (?,?,?,?);
			SetAnimation(288, 480, 64, 64);
		}


		break;
	case STATE_RUNNING:

		m_playerkey = (char)'a' + 0 + (m_frame % 8);


		//move on ground
		if (EVMA::KeyHeld(SDL_SCANCODE_A)) {
			m_accelX = -1.5;
			if (!m_isFacingLeft)
				m_isFacingLeft = true;
		}
		else if (EVMA::KeyHeld(SDL_SCANCODE_D)) {
			m_accelX = 1.5;
			if (m_isFacingLeft)
				m_isFacingLeft = false;
		}



		//transition to idle
		if (!EVMA::KeyHeld(SDL_SCANCODE_A) && !EVMA::KeyHeld(SDL_SCANCODE_D)) {

			m_state = STATE_IDLING;

		}

		// 
		// 
		//transitiont to jump
		else if (EVMA::KeyPressed(SDL_SCANCODE_SPACE) && m_isGrounded) {

			m_accelY = -JUMPFORCE;
			m_isGrounded = false;
			m_state = STATE_JUMPING;
			//set animation (?,?,?,?);
			SetAnimation(288, 480, 64, 64);
		}


		break;
	case STATE_JUMPING:


		m_playerkey = (char)'a' + 17;

		if (EVMA::KeyHeld(SDL_SCANCODE_A)) {

			m_accelX = -1.5;
			if (!m_isFacingLeft)
				m_isFacingLeft = true;

		}
		else if (EVMA::KeyHeld(SDL_SCANCODE_D)) {

			m_accelX = 1.5;
			if (m_isFacingLeft)
				m_isFacingLeft = false;
		}

		if (m_isGrounded) {
			m_state = STATE_RUNNING;


			//setanimation(?,?,?,?);
			SetAnimation(288, 480, 64, 64);



		}

		break;
	}


	m_velX += m_accelX;
	m_velX *= (m_isGrounded ? m_drag : 1.0);
	m_velX = std::min(std::max(m_velX, -m_maxVelX), m_maxVelX);
	m_dst.x += (float)m_velX;

	//yaxis
	m_velY += m_accelY + m_grav;
	m_velY = std::min(std::max(m_velY, -m_maxVelY), m_maxVelY);
	m_dst.y += (float)m_velY;

	m_accelX = m_accelY = 0.0;
	Animate();
}

void PlatformPlayer::Render()
{
	//to animate the sprite, use sdl_rendercopyexf() and you wll ahve to access the texture manager and pass in a key such as "player".

//	SDL_RenderCopyExF(Engine::Instance().GetRenderer(), TEMA::GetTexture("player"), &m_src, &m_dst, m_sprite, nullptr, SDL_FLIP_NONE);




	// 
	// 
	// 
	//but for this part, w'erd going to use a colored square


	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 0, 0, 255);
	//	SDL_RenderFillRectF(Engine::Instance().GetRenderer(), &m_dst);

	Tile* temp = m_tiles[m_playerkey]->Clone();

	temp->SetXY(m_dst.x, m_dst.y);

	SDL_RenderCopyExF(Engine::Instance().GetRenderer(), TEMA::GetTexture("player"), temp->GetSrc(), temp->GetDst(), 0, nullptr, SDL_FLIP_NONE);

}

void PlatformPlayer::Stop()
{
	StopX();
	StopY();
}

void PlatformPlayer::StopX()
{
	m_velX = 0.0;
}

void PlatformPlayer::StopY()
{
	m_velY = 0.0;
}

void PlatformPlayer::SetAccelX(double a)
{
	m_accelX = a;
}

void PlatformPlayer::SetAccelY(double a)
{
	m_accelY = a;
}

void PlatformPlayer::SetX(float x)
{
	m_dst.x = x;
}

void PlatformPlayer::SetY(float y)
{
	m_dst.y = y;
}

void PlatformPlayer::SetGrounded(bool g)
{
	m_isGrounded = g;

}

bool PlatformPlayer::IsGrounded()
{
	return m_isGrounded;
}

double PlatformPlayer::GetVelX()
{
	return m_velX;
}

double PlatformPlayer::GetVelY()
{
	return m_velY;
}
