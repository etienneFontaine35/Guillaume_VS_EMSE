#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <ctime>
#include "Objet_Volant.hpp"
#include "Tools.hpp"

class Background
{
	public :
		Background(std::string textureFondPremier_file, std::string textureFondDeuxieme_file, std::string textureParticuleBack_file, std::string textureParticuleFront_file, sf::RenderWindow& fenetre, int const dimension[]);
		
		void apparaitreFront(int const dimension[]);
		void apparaitreBack(int const dimension[]);
		void disparaitreFront(int const dimension[]);
		void disparaitreBack(int const dimension[]);
		void scrolling(int const dimension[]);
		void afficher();
		void mettreAJour(int const dimension[]);

	private :
		sf::Texture m_textureFondPremier;
		sf::Sprite m_spriteFondPremier;
		
		sf::Texture m_textureFondDeuxieme;
		sf::Sprite m_spriteFondDeuxieme;

		sf::Texture m_textureParticuleBack;
		sf::Sprite m_spriteParticuleBack;
		
		sf::Texture m_textureParticuleFront;
		sf::Sprite m_spriteParticuleFront;

		std::vector<ObjVolant> m_particulesFrontEnCours;
		std::vector<ObjVolant> m_particulesBackEnCours;
		ObjVolant m_premierFond;
		ObjVolant m_deuxiemeFond;

		sf::RenderWindow& m_fenetre;

};


class Opening
{
	public :
		Opening(std::string fichierImage, Inputs& inputs, Hud& hud, int tempsAffichage, int tempsApparition);
		Opening(std::string fichierImage, Inputs& inputs, Hud& hud, int tempsAffichage);
		
		void executer(sf::RenderWindow& fenetre);
		void executer(sf::RenderWindow& fenetre, std::string texte, int tailleTexte, int const dimension[]);

		void setTempsAffichage(int temps);


	private :
		Hud& m_texte;
		Inputs& m_inputs;

		int m_tempsAffichage;
		int m_tempsApparition;
		
		sf::Texture m_imageTexture;
		sf::Sprite m_imageSprite;

		sf::Clock m_timerAffichage;

};
