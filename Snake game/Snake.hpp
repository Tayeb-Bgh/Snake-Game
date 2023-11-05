#ifndef DEF_SNAKE
#define DEF_SNAKE

#include <vector>
#include "raylib.h"


class Snake
{
	private : 
		Vector2 m_pos;
		Vector2 m_vitesse;
		int m_vit;

	public :
		Snake();
		Snake(Vector2 position,Vector2 vitesse,Vector2 dim);

		~Snake();

		Vector2 DonnePos();
		Vector2 DonneVit();

		void Deplace(Vector2 posActuel);
		void ChangeDir(int touche);
		void Perdu();
		void RestartGame(int nbr, std::vector<Vector2> stockDir);

	

};


#endif