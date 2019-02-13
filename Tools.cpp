#include "Tools.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <map>


//-------------------------
//   METHODES DE Inputs
//-------------------------


Inputs::Inputs()
{
	m_touche.gauche = m_touche.droite = m_touche.haut = m_touche.bas = m_touche.espace = m_touche.esc = m_touche.z = m_touche.rightMouse = m_touche.leftMouse = m_touche.backSpace = m_touche.entree = false;
	m_touche.mouseX = m_touche.mouseY = 0;

}


void Inputs::gererEvents(sf::RenderWindow &fenetre)
{
	while (fenetre.pollEvent(m_event))
	{
		switch (m_event.type)
		{
			case sf::Event::Closed :
				fenetre.close();
				break;

			
			case sf::Event::KeyPressed :
				
				if (m_event.key.code == sf::Keyboard::Up)
					m_touche.haut = true;
				
				else if (m_event.key.code == sf::Keyboard::Down)
					m_touche.bas = true;
			
				else if (m_event.key.code == sf::Keyboard::Left)
					m_touche.gauche = true;
		
				else if (m_event.key.code == sf::Keyboard::Right)
					m_touche.droite = true;
				
				else if (m_event.key.code == sf::Keyboard::Z)
					m_touche.z = true;
				
				else if (m_event.key.code == sf::Keyboard::Escape)
					m_touche.esc = true;

				else if (m_event.key.code == sf::Keyboard::BackSpace)
					m_touche.backSpace = true;

				else if (m_event.key.code == sf::Keyboard::Return)
					m_touche.entree = true;

				break;
			
			
			case sf::Event::KeyReleased :
				if (m_event.key.code == sf::Keyboard::Up)
					m_touche.haut = false;

				else if (m_event.key.code == sf::Keyboard::Down)
					m_touche.bas = false;

				else if (m_event.key.code == sf::Keyboard::Left)
					m_touche.gauche = false;

				else if (m_event.key.code == sf::Keyboard::Right)
					m_touche.droite = false;

				else if (m_event.key.code == sf::Keyboard::Z)
					m_touche.z = false;

				else if (m_event.key.code == sf::Keyboard::Escape)
					m_touche.esc = false;

				else if (m_event.key.code == sf::Keyboard::BackSpace)
					m_touche.backSpace = false;

				else if (m_event.key.code == sf::Keyboard::Return)
					m_touche.entree = false;

				break;


			case sf::Event::MouseMoved :

				m_touche.mouseX = m_event.mouseMove.x;
				m_touche.mouseY = m_event.mouseMove.y;
				break;


			case sf::Event::MouseButtonPressed :
				if (m_event.mouseButton.button == sf::Mouse::Right)
					m_touche.rightMouse = true;

				if (m_event.mouseButton.button == sf::Mouse::Left)
					m_touche.leftMouse = true;

				break;


			case sf::Event::MouseButtonReleased :
				if (m_event.mouseButton.button == sf::Mouse::Right)
					m_touche.rightMouse = false;

				if (m_event.mouseButton.button == sf::Mouse::Left)
					m_touche.leftMouse = false;

				break;
			
			case sf::Event::TextEntered :
				if (m_event.text.unicode != 13 && m_event.text.unicode != 8 && m_event.text.unicode != 1)
					m_texte += m_event.text.unicode;
					
				break;

				
			default :
				break;
		}
	}	
}


Touche Inputs::getTouche() const
{
	return m_touche;
}


std::string Inputs::getTexte() const
{
	return m_texte;
}


void Inputs::setTexte(std::string texte)
{
	m_texte = texte;
}

void Inputs::setTexte()
{
	m_texte.clear();
}


//---------------------
//   METHODES DE Hud
//---------------------

Hud::Hud(sf::RenderWindow& fenetre, std::string police_file, int taille) : m_fenetre(fenetre), m_tailleTexteStand(taille)
{
	if (!m_police.loadFromFile(police_file))
	{
		std::cout << "erreur de chargement de la police" << std::endl;
	}

	m_texte.setFont(m_police);
}


void Hud::afficher(int posX, int posY, int taille, sf::Color couleur)
{
	m_texte.setFillColor(couleur);
	m_texte.setCharacterSize(taille);
	m_texte.setPosition(sf::Vector2f(posX, posY));
	m_fenetre.draw(m_texte);	
}


void Hud::afficher(int posX, int posY, sf::Color couleur)
{
	m_texte.setFillColor(couleur);
	m_texte.setCharacterSize(m_tailleTexteStand);
	m_texte.setPosition(sf::Vector2f(posX, posY));
	m_fenetre.draw(m_texte);	
}


void Hud::setTexte(std::wstring texte)
{
	m_texte.setString(texte);
}


void Hud::setTexte(std::string texte)
{
	m_texte.setString(texte);
}


void Hud::setTaille(int taille)
{
	m_tailleTexteStand = taille;
}


int Hud::getTailleStand()
{
	return m_tailleTexteStand;
}


sf::Text Hud::getTexte()
{
	return m_texte;
}


int Hud::getLargeurTexte()
{
	sf::FloatRect bounds(m_texte.getLocalBounds());
	return int(bounds.width);
}


int Hud::getHauteurTexte()
{
	sf::FloatRect bounds(m_texte.getLocalBounds());
       	return int(bounds.height);	
}



//----------------------
//   METHODES DE Save
//----------------------

Save::Save(int max_saves) : m_maxSaves(max_saves)
{

}


void Save::ajouterEntree(std::string pseudo, int score)
{
	if (m_listeSaves.size() < m_maxSaves)
		m_listeSaves[pseudo] = score;
	else
	{
		std::pair<std::string, int> petitScore(Save::plusPetitScore());
		if (petitScore.second < score)
		{
			m_listeSaves.erase(petitScore.first);
			m_listeSaves[pseudo] = score;
		}
	}
}


std::pair<std::string, int> Save::plusPetitScore()
{
	std::pair<std::string, int> petitScore(*(m_listeSaves.begin()));

	for (std::map<std::string, int>::iterator entree = m_listeSaves.begin(); entree != m_listeSaves.end(); entree++)
	{
		if (entree->second < petitScore.second)
			petitScore = *entree;
	}

	return petitScore;
}


int Save::getNbreEntrees()
{
	return m_maxSaves;
}


int Save::pseudo2Score(std::string pseudo)
{
	return m_listeSaves[pseudo];
}


std::map<std::string, int> Save::getListe()
{
	return m_listeSaves;
}



//-----------------------------
// FONCTIONS POUR L'Aleatoire
//-----------------------------

double probUniform()
{
	double max(RAND_MAX);
	double nbreAlea(rand());
	return nbreAlea/max;
}


int randInteger(int a, int b)
{
	return std::round(probUniform() * (b - a)) + a;
}


int randInteger(int b)
{
	return randInteger(0, b);
}


int randInteger()
{
	return randInteger(RAND_MAX);
}