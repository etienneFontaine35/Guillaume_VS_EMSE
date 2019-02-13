#ifndef MENU_H
#define MENU_H
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <ctime>
#include "Objet_Volant.hpp"
#include "Tools.hpp"

//-------------
class Menu;
class Entree;
//-------------

class Menu
{
	public :
		//Constructeur
		Menu();

		//Méthodes
		void ajouterEntree(Entree* entree);
		void resetInteraction();
		bool peutInteragir();
		void resetTimer();
		void afficher();
		void setActivite(bool act);
		bool getActivite();


	private :
		bool m_actif;
		double m_cooldownInteraction;
		sf::Clock m_timerInteraction;
		std::vector<Entree*> m_listeEntrees;
};


class Entree
{
	public :
		//Constructeur
		Entree(sf::RenderWindow &fenetre, std::wstring texte, std::string fontFile, int textSize, std::string textureFile, int posX, int posY, int hitBoxX, int hitBoxY);

		//Méthodes
		void interagir(Inputs &inputs);
		void survoler(Inputs &inputs);
		void afficher();

		bool getInteraction();
		void resetInteraction();
		void setPos(int x, int y);

	private :
		bool m_survol;
		bool m_interaction;
		sf::RenderWindow& m_fenetre;
		std::wstring m_texte;
		Hud m_hud;
		int m_posX;
		int m_posY;
		int m_hitBoxX;
		int m_hitBoxY;
		sf::Texture m_texture;
		sf::Sprite m_sprite;

};

#endif
