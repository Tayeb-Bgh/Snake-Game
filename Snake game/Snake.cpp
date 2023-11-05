#include "Snake.hpp"

Snake::Snake() : m_pos({600,400}),m_vitesse({-5,0}),m_vit(5)
{
}

Snake::~Snake()
{}

Snake::Snake(Vector2 position,Vector2 vitesse,Vector2 dim) : m_vitesse({ vitesse.x,vitesse.y }),m_vit(5)
{	
	if (vitesse.x == 0 && vitesse.y > 0)
		m_pos = { position.x ,position.y - dim.y -1 };
	else if (vitesse.x == 0 && vitesse.y < 0)
		m_pos = { position.x ,position.y + dim.y + 1};
	else if (vitesse.y == 0 && vitesse.x > 0)
		m_pos = { position.x - dim.x -1,position.y };
	else
		m_pos = { position.x + dim.x +1, position.y };
}


Vector2 Snake::DonnePos()
{
	return m_pos;
}

Vector2 Snake::DonneVit()
{
	return m_vitesse;
}

void Snake::Deplace(Vector2 posActuel)
{
	m_pos.x = posActuel.x + m_vitesse.x;
	m_pos.y = posActuel.y + m_vitesse.y;

	if (m_pos.x > 800)
		m_pos.x = 0;
	if (m_pos.y > 795)
		m_pos.y = 100;
	if (m_pos.x < 0)
		m_pos.x = 800;
	if (m_pos.y < 100)
		m_pos.y = 795;

}

void Snake::ChangeDir(int touche)
{
	if (m_vitesse.y==0 && touche==KEY_W)
		m_vitesse = { 0,static_cast<float>(-m_vit) }; //{0,-1}
	else if (m_vitesse.y == 0 && touche==KEY_S)
		m_vitesse = { 0,static_cast<float>(m_vit) }; //{0,1}
	else if (m_vitesse.x == 0 && touche==KEY_A)
		m_vitesse = { static_cast<float>(-m_vit),0 }; //{-1,0}
	else if (m_vitesse.x == 0 && touche==KEY_D)
		m_vitesse = { static_cast<float>(m_vit),0 };//{1,0}
}

void Snake::Perdu()
{	
	
	m_vitesse.x = 0;
	m_vitesse.y = 0;
}

void Snake::RestartGame(int nbr, std::vector<Vector2> stockVit)
{	
	int i;
	if (nbr % 2 == 0)
	{
		if (stockVit[nbr].x == 0)
			m_vitesse = { static_cast<float>(-m_vit),0 };
		else
			m_vitesse = { 0,static_cast<float>(-m_vit) };
	}
	else
	{
		if (stockVit[nbr].x == 0)
			m_vitesse = { static_cast<float>(m_vit),0 };
		else
			m_vitesse = { 0,static_cast<float>(m_vit) };
	}
}



