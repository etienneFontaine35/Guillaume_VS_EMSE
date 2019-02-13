#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <ctime>
#include "Objet_Volant.hpp"
#include "Background.hpp"
#include "Tools.hpp"

//------------------------------
//    METHODE DE Background
//------------------------------


//Constructeur

Background::Background(std::string textureFondPremier_file, std::string textureFondDeuxieme_file, std::string textureParticuleBack_file, std::string textureParticuleFront_file, sf::RenderWindow& fenetre, int const dimension[]) : m_premierFond(0, 0, 0, 1, 0, 0, &m_spriteFondPremier), m_deuxiemeFond(0, -dimension[1], 0, 1, 0, 0, &m_spriteFondDeuxieme), m_fenetre(fenetre)
{
	if (!m_textureFondPremier.loadFromFile(textureFondPremier_file))
		std::cout << "erreur de chargement de texture Fond Premier" << std::endl;
	m_spriteFondPremier.setTexture(m_textureFondPremier);

	if (!m_textureFondDeuxieme.loadFromFile(textureFondDeuxieme_file))
		std::cout << "erreur de chargement de texture Fond Deuxieme" << std::endl;
	m_spriteFondDeuxieme.setTexture(m_textureFondDeuxieme);

	if (!m_textureParticuleBack.loadFromFile(textureParticuleBack_file))
		std::cout << "erreur de chargement de texture Particule Back" << std::endl;
	m_spriteParticuleBack.setTexture(m_textureParticuleBack);
	
	if (!m_textureParticuleFront.loadFromFile(textureParticuleFront_file))
		std::cout << "erreur de chargement de texture Fond" << std::endl;
	m_spriteParticuleFront.setTexture(m_textureParticuleFront);
	

	srand(time(NULL));

}


void Background::apparaitreFront(int const dimension[])
{
	// ObjVolant part(rand()%dimension[0] - 5, -12, 0, 4 + rand()%5, 5, 12, &m_spriteParticuleFront);
	ObjVolant part(randInteger(dimension[0]) - 5, -12, 0, 4 + randInteger(4), 5, 12, &m_spriteParticuleFront);
	m_particulesFrontEnCours.push_back(part);
}


void Background::apparaitreBack(int const dimension[])
{
	// ObjVolant part(rand()%dimension[0] - 3, -8, 0, 2 + rand()%2, 3, 8, &m_spriteParticuleBack);
	ObjVolant part(randInteger(dimension[0]) - 3, -8, 0, 2 + randInteger(1), 3, 8, &m_spriteParticuleBack);
	m_particulesBackEnCours.push_back(part);
}


void Background::disparaitreFront(int const dimension[])
{
	std::vector<int> aSuppr;
	int i(0);

	for (std::vector<ObjVolant>::iterator obj = m_particulesFrontEnCours.begin(); obj != m_particulesFrontEnCours.end(); obj++)
	{
		obj->descendre();
		if (obj->getPosY() > dimension[1])
		{
			aSuppr.push_back(i);
		}
		i++;
	}

	for (int index = aSuppr.size() - 1; index >= 0; index--)
		m_particulesFrontEnCours.erase(m_particulesFrontEnCours.begin() + aSuppr[index]);
}


void Background::disparaitreBack(int const dimension[])
{
	std::vector<int> aSuppr;
	int i(0);

	for (std::vector<ObjVolant>::iterator obj = m_particulesBackEnCours.begin(); obj != m_particulesBackEnCours.end(); obj++)
	{
		obj->descendre();
		if (obj->getPosY() > dimension[1])
		{
			aSuppr.push_back(i);
		}
		i++;
	}

	for (int index = aSuppr.size() - 1; index >= 0; index--)
		m_particulesBackEnCours.erase(m_particulesBackEnCours.begin() + aSuppr[index]);
}

void Background::scrolling(int const dimension[])
{
	m_deuxiemeFond.descendre();
	if (m_deuxiemeFond.getPosY() > dimension[1])
		m_deuxiemeFond.setPosY(-dimension[1]);
	
	m_premierFond.descendre();
	if (m_premierFond.getPosY() > dimension[1])
		m_premierFond.setPosY(-dimension[1]);

}


void Background::afficher()
{
	m_premierFond.afficher(m_fenetre);
	m_deuxiemeFond.afficher(m_fenetre);	

	for (std::vector<ObjVolant>::iterator obj = m_particulesBackEnCours.begin(); obj != m_particulesBackEnCours.end(); obj++)
		obj->afficher(m_fenetre);

	for (std::vector<ObjVolant>::iterator obj = m_particulesFrontEnCours.begin(); obj != m_particulesFrontEnCours.end(); obj++)
		obj->afficher(m_fenetre);
}


void Background::mettreAJour(int const dimension[])
{
	Background::scrolling(dimension);
	
	if (m_particulesFrontEnCours.size() < 100 && rand()%22==0)
	{
		Background::apparaitreFront(dimension);
	}
	

	if (m_particulesBackEnCours.size() < 100 && rand()%9==0)
	{
		Background::apparaitreBack(dimension);
	}

	Background::disparaitreFront(dimension);
	Background::disparaitreBack(dimension);

	Background::afficher();
}




//------------------------------
//    METHODE DE Opening
//------------------------------

// Constructeur


Opening::Opening(std::string fichierImage, Inputs& inputs, Hud& hud, int tempsAffichage, int tempsApparition) : m_texte(hud), m_inputs(inputs), m_tempsAffichage(tempsAffichage), m_tempsApparition(tempsApparition)
{
	if (!m_imageTexture.loadFromFile(fichierImage))
		std::cout << "erreur de chargement de texture Opening" << std::endl;
	m_imageSprite.setTexture(m_imageTexture);

	m_timerAffichage.restart();
}


Opening::Opening(std::string fichierImage, Inputs& inputs, Hud& hud, int tempsAffichage) : Opening(fichierImage, inputs, hud, tempsAffichage, 1)
{

}


void Opening::executer(sf::RenderWindow& fenetre)
{
	m_timerAffichage.restart();
	double prop;

	bool passer(false);

	while (m_timerAffichage.getElapsedTime().asSeconds() < m_tempsAffichage + 2.25 * m_tempsApparition && !passer)
	{
		m_inputs.gererEvents(fenetre);
		if (m_inputs.getTouche().esc)
		{	
			passer = true;
		}

		if (m_timerAffichage.getElapsedTime().asSeconds() < 0.25 * m_tempsApparition)
		{
			m_imageSprite.setColor(sf::Color(255, 255, 255, 0));
		}

		else if (m_timerAffichage.getElapsedTime().asSeconds() < 1.25 * m_tempsApparition)
		{
			prop = (m_timerAffichage.getElapsedTime().asSeconds() - 0.25 * m_tempsApparition) / m_tempsApparition;
			m_imageSprite.setColor(sf::Color(255, 255, 255, prop * 255));
		}
		
		else if (m_timerAffichage.getElapsedTime().asSeconds() < 1.25 * m_tempsApparition + m_tempsAffichage)
		{
			m_imageSprite.setColor(sf::Color(255, 255, 255, 255));
		}

		else
		{
			prop = 1 - (m_timerAffichage.getElapsedTime().asSeconds() - 1.25 * m_tempsApparition - m_tempsAffichage) / m_tempsApparition;
			m_imageSprite.setColor(sf::Color(255, 255, 255, prop * 255));
		}
		
		fenetre.clear(sf::Color::Black);
		fenetre.draw(m_imageSprite);
		fenetre.display();
	}
}


void Opening::executer(sf::RenderWindow& fenetre, std::string texte, int tailleTexte, int const dimension[])
{
	m_timerAffichage.restart();
	double prop;
	sf::Color couleur;
	m_texte.setTaille(tailleTexte);
	m_texte.setTexte(texte);

	bool passer(false);

	while (m_timerAffichage.getElapsedTime().asSeconds() < m_tempsAffichage + 2.25 * m_tempsApparition && !passer)
	{
		m_inputs.gererEvents(fenetre);
		if (m_inputs.getTouche().esc)
		{	
			passer = true;
		}

		if (m_timerAffichage.getElapsedTime().asSeconds() < 0.25 * m_tempsApparition)
		{
			couleur = sf::Color(229, 229, 229, 0);
		}

		else if (m_timerAffichage.getElapsedTime().asSeconds() < 1.25 * m_tempsApparition)
		{
			prop = (m_timerAffichage.getElapsedTime().asSeconds() - 0.25 * m_tempsApparition) / m_tempsApparition;
			couleur = sf::Color(229, 229, 229, prop * 255);
		}

		else if (m_timerAffichage.getElapsedTime().asSeconds() < 1.25 * m_tempsApparition + m_tempsAffichage)
		{
			couleur = sf::Color(229, 229, 229, 255);
		}

		else
		{
			prop = 1 - (m_timerAffichage.getElapsedTime().asSeconds() - 1.25 * m_tempsApparition - m_tempsAffichage) / m_tempsApparition;
			couleur = sf::Color(229, 229, 229, prop * 255);
		}

		fenetre.clear(sf::Color::Black);
		m_texte.afficher((dimension[0] - m_texte.getLargeurTexte()) / 2, (dimension[1] - m_texte.getHauteurTexte()) / 2 , couleur);
		fenetre.display();
	}

}


void Opening::setTempsAffichage(int temps)
{
	m_tempsAffichage = temps;
}

