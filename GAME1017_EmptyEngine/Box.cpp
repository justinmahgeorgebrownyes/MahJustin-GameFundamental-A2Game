#include "Box.h"
#include "Engine.h"
#include "TextureManager.h"
#define SCROLLSPEED 4

Sprite::Sprite() :m_dst({ 0,0,0,0 }), m_color({ 255,255,255,255 }) {


}


Sprite::Sprite(const SDL_Rect r, const SDL_Color c, const SDL_Rect source, std::string spriteKey) :m_dst(r), m_color(c), m_src(source), m_key(spriteKey)
{

}
//note as i've done before you acn just asdd defoatl values to the parameters and not need a default constructor

void Sprite::Render() {
	//for this inital lab, we are just drawgin rectabnlges
	SDL_FRect dst = { m_dst.x, m_dst.y, m_dst.w, m_dst.h };



	SDL_RenderCopyF(Engine::Instance().GetRenderer(), TEMA::GetTexture(m_key), &m_src, &dst);




	//SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(),m_color.r, m_color.g, m_color.b, m_color.a);
	//SDL_RenderFillRect(Engine::Instance().GetRenderer(),&m_dst);

}


Box::Box(const SDL_Point p, bool makeSprite, const SDL_Rect r,
	const SDL_Color c, const SDL_Rect source, std::string spriteKey) :m_pos(p), m_pSprite(nullptr)//note r an dc are for sprite 
{
	if (makeSprite) {
		m_pSprite = new Sprite(r, c, source, spriteKey);
	}
}

Box::~Box() {

	if (m_pSprite != nullptr) {
		delete m_pSprite;
	}
}


Box* Box::Clone() {
	Box* clone = new Box(this->m_pos, false);
	clone->m_pSprite = new Sprite(this->m_pSprite->m_dst, this->m_pSprite->m_color, this->m_pSprite->m_src, this->m_pSprite->m_key);
	return clone;
}





void Box::Update() {

	m_pos.x -= SCROLLSPEED;

	if (m_pSprite != nullptr) {
		//m_pos.x -= SCROLLSPEED;
		m_pSprite->m_dst.x -= SCROLLSPEED;
	}

}


void Box::Render() {



	//if we want to render a border around each box


	SDL_Rect dst = { m_pos.x, m_pos.y, 128, 128 };
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 156, 230, 200, 255);
	SDL_RenderFillRect(Engine::Instance().GetRenderer(), &dst);



	if (m_pSprite != nullptr) {
		m_pSprite->Render();
	}
}



