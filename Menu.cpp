#include "Menu.hpp"
#include <iostream>
#include <vector>

//---------------------------
//    METHODES DE Entree
//---------------------------


Entree::Entree(sf::RenderWindow& fenetre, std::wstring texte, std::string fontFile, int textSize, std::string textureFile, int posX, int posY, int hitBoxX, int hitBoxY) : m_survol(false), m_interaction(false), m_fenetre(fenetre), m_texte(texte), m_hud(fenetre, fontFile, textSize), m_posX(posX), m_posY(posY), m_hitBoxX(hitBoxX), m_hitBoxY(hitBoxY)
{
	if (!m_texture.loadFromFile(textureFile))
		std::cout << "erreur chargement texture de Entree" << std::endl;
	m_sprite.setTexture(m_texture);
}


void Entree::survoler(Inputs &inputs)
{
	Touche touche(inputs.getTouche());
	m_survol = (m_posX <= touche.mouseX) && (touche.mouseX<= m_posX + m_hitBoxX) && (m_posY <= touche.mouseY) && (touche.mouseY <= m_posY + m_hitBoxY);
}


void Entree::interagir(Inputs &inputs)
{
	m_interaction = false;
	if (m_survol && inputs.getTouche().leftMouse)
	{
		m_interaction = true;
	}

}


void Entree::afficher()
{
	m_sprite.setTextureRect(sf::IntRect(m_survol*m_hitBoxX, 0, m_hitBoxX, m_hitBoxY));
	m_sprite.setPosition(sf::Vector2f(m_posX, m_posY));
	m_fenetre.draw(m_sprite);
	m_hud.setTexte(m_texte);
	m_hud.afficher(m_posX + (m_hitBoxX-m_hud.getLargeurTexte()) / 2, m_posY + (m_hitBoxY-m_hud.getHauteurTexte()) / 2, sf::Color(229, 229, 229));
}


bool Entree::getInteraction()
{
	return m_interaction;
}


void Entree::resetInteraction()
{
	m_interaction = false;
}


void Entree::setPos(int x, int y)
{
	m_posX = x;
	m_posY = y;
}

//-----------------------------
//    METHODES DE Menu
//-----------------------------


Menu::Menu() : m_actif(false), m_cooldownInteraction(0.3)
{
	m_timerInteraction.restart();
}


void Menu::ajouterEntree(Entree* ptEntree)
{
	m_listeEntrees.push_back(ptEntree);
}


void Menu::resetInteraction()
{
	for (std::vector<Entree*>::iterator ptEntree = m_listeEntrees.begin(); ptEntree != m_listeEntrees.end(); ptEntree++)
		(*ptEntree)->resetInteraction();
}


bool Menu::peutInteragir()
{
	return m_timerInteraction.getElapsedTime().asSeconds() > m_cooldownInteraction;
}


void Menu::resetTimer()
{
	m_timerInteraction.restart();
}


void Menu::afficher()
{
	for (std::vector<Entree*>::iterator ptEntree = m_listeEntrees.begin(); ptEntree != m_listeEntrees.end(); ptEntree++)
		(*ptEntree)->afficher();
}


void Menu::setActivite(bool act)
{
	m_actif = act;
}


bool Menu::getActivite()
{
	return m_actif;
}
