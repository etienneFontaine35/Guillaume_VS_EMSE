#ifndef TOOLS_H
#define TOOLS_H
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include <map>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>



struct Touche
{ 
	bool gauche, droite, haut, bas, espace, esc, z, leftMouse, rightMouse, backSpace, entree;
	int mouseX, mouseY;
};


class Inputs
{
	public :
		//Constructeur
		Inputs();
		
		//Méthodes
		void gererEvents(sf::RenderWindow &fenetre);
 
		//Accesseurs
		Touche getTouche() const;
		std::string getTexte() const;
		void setTexte(std::string texte);
		void setTexte();
		void setTouche(int bouton, bool etat);
			    
	private :
		sf::Event m_event;
		Touche m_touche;
		std::string m_texte;
};


class Hud
{
	public :
		//Constructeur
		Hud(sf::RenderWindow& fenetre, std::string police_file, int taille);
		
		//Méthodes
		void afficher(int posX, int posY, int taille, sf::Color couleur);
		void afficher(int posX, int posY, sf::Color couleur);

		//Assesseur
		int getTailleStand();
		int getLargeurTexte();
		int getHauteurTexte();
		void setTexte(std::wstring texte);
		void setTexte(std::string texte);
		void setTaille(int taille);
		sf::Text getTexte();
	
	private :
		sf::RenderWindow& m_fenetre;
		sf::Font m_police;
		sf::Text m_texte;
		int m_tailleTexteStand;
};


class Save
{
	
	public :
		Save(int max_saves);
		void ajouterEntree(std::string pseudo, int score);
		std::pair<std::string, int> plusPetitScore();
		int getNbreEntrees();
		int pseudo2Score(std::string pseudo);
		std::map<std::string, int> getListe();
	
	private :
		friend class boost::serialization::access;

        template<class Archive>
        void serialize(Archive & ar, const unsigned int version)
		{
                ar & m_maxSaves;
                ar & m_listeSaves;
        }

		unsigned int m_maxSaves;
		std::map<std::string, int> m_listeSaves;

};



// FONCTIONS POUR L'Aleatoire

double probUniform();

int randInteger(int a, int b);

int randInteger(int b);

int randInteger();
	

#endif
