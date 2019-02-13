#ifndef OBJET_VOLANT_H
#define OBJET_VOLANT_H
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <map>
#include <vector>
#include <ctime>
#include "Tools.hpp"

//-----Prototypes des classes-----//
class ObjVolant;

class Vaisseau; //-->ObjVolant
class Guillaume; //-->Vaisseau
class Ennemi; //-->Vaisseau
class ArmeeEnnemie;

class Bonus; //-->ObjVolant
class BonusRevisionLaVeille; //-->Bonus
class BonusRevisionTrois; //-->Bonus
class BonusErmite; //-->Bonus
class BonusSoin; //-->Bonus
class BonusGingerXplosion; //-->Bonus
class ListeBonus;

class Tir; //-->ObjVolant


class Arme;
class RevisionLaVeille; //-->Arme
class RevisionTrois; //-->Arme
class Ermite; //-->Arme
class QuestionPiege; //-->Arme
//--------------------------------//


class ObjVolant
{	
	public :
		//constructeur
		ObjVolant(int posX, int posY, int vitX, int vitY, int hitBoxX, int hitBoxY, sf::Sprite *ptSprite); 

		//méthode de mouvement
		void monter();
		void descendre();
		void allerGauche();
		void allerDroite();
		
		//interactions	
		bool collision(ObjVolant const& obj) const;
		bool horsMap(int const dimension[]) const;

		//Affichage et MAJ
		void mettreAJour();
		void afficher(sf::RenderWindow &fenetre);

		//assesseurs
		int getPosX() const;
		int getPosY() const;
		void setPosX(int x);
		void setPosY(int Y);
		int getVitX() const;
		int getVitY() const;
		void setVitX(int x);
		void setVitY(int Y);
		int getHitBoxX() const;
		int getHitBoxY() const;
		int centre(char a) const; 

	protected :
		int m_posX, m_posY;
		int m_vitX, m_vitY;
		int m_hitBoxX, m_hitBoxY;
		sf::Sprite* m_ptSprite;

};


//----------------------------------------------------
//            CLASSES DE VAISSEAUX
//----------------------------------------------------


class Vaisseau : public ObjVolant
{
	public :
		//constructeur
		Vaisseau(int posX, int posY, int vitX, int vitY, int hitBoxX, int hitBoxY, sf::Sprite *ptSprite, double cooldownInvincibilite);
		
		//Méthodes de combat
		void tirer();
		void perdreVie(int degats);

		//Accesseurs
		void setArme(Arme &armeObtenue);
		int getVie() const;
		Arme& getArme();
		bool getIntact();
		void setIntact(bool intact);
		void resetInvincibilite();
		double getInvinProp();

	protected :
		int m_vie;
		sf::Clock m_timerInvincibilite;
		double m_cooldownInvincibilite;
		bool m_intact;
		Arme* m_armeEnCours;
};	



class Guillaume : public Vaisseau
{
	public :
		//constructeurs et destructeurs
		Guillaume(int posX, int posY, int vitX, int vitY, int hitBoxX, int hitBoxY, int vie, sf::Sprite *ptSprite, sf::Sprite *ptSpriteReacteur);

		//Masquage des méthodes de déplacement
		void monter(int const dimension[]);
		void descendre(int const dimension[]);
		void allerGauche(int const dimension[]);
		void allerDroite(int const dimension[]);
		

		//Méthodes de combat
		void seSoigner(int soin);

		//Méthodes de mise à jour
		void mettreAJour(int const dimension[], Inputs& inputs);
		void afficher(sf::RenderWindow &fenetre, int frame);
	
	private :
		sf::Sprite *m_ptSpriteReacteur;	

};


class Ennemi : public Vaisseau
{
	public :
		//constructeurs et desructeurs		
		Ennemi(int posX, int posY, int vitX, int vitY, int hitBoxX, int hitBoxY, int vie, sf::Sprite *ptSprite);
		~Ennemi();

		//Masquage des méthodes de déplacement
		void monter(int const dimension[]);
		void descendre(int const dimension[]);
		void allerGauche(int const dimension[]);
		void allerDroite(int const dimension[]);
		void mettreAJour(int const dimension[], Guillaume& guigui);

		void afficher(sf::RenderWindow& fenetre, int frame);	
	
	private :
		int m_vitesseExplosion;
};


//----------------------------------------------------
//            CLASSES DE BONUS
//----------------------------------------------------



class Bonus : public ObjVolant
{
	public :
		//Constructeurs et destructeurs
		Bonus(int posX, int posY, int vitX, int vitY, int hitBoxX, int hitBoxY, int probApp, sf::Sprite* ptSprite, sf::SoundBuffer& sonRecup, Arme& arme);
		Bonus(int posX, int posY, int vitX, int vitY, int probApp, sf::Sprite* ptSprite, sf::SoundBuffer& sonRecup, Arme& arme);

		//Méthodes de mouvement
		void bouger(int const dimension[]);
		void mvtLateral();
		void mvtVertical();
		virtual void recup(Guillaume& guigui);

		//Méthodes d'affichage
		virtual void afficher(sf::RenderWindow &fenetre, int frame);

		//Getters & Setters
		bool isSpawned();
		void setSpawned(bool state);
		int getProbApp();
		void setAmplitude(int nvAmpl);
		void setNbreNoeuds(int nvNbre);
		void setOriginX(int posX);
		void nvCarac();

	protected :
		sf::Sound m_sonRecup;
		bool m_spawned;
		int m_probApp;
		int m_originX;
		int m_nbreNoeuds;
		int m_amplitude;
		Arme& m_arme;

};



class BonusErmite : public Bonus
{
	public :
		//Constructeurs et destructeurs
		BonusErmite(int posX, int posY, int vitX, int vitY, int probApp, sf::Sprite* ptSprite, sf::SoundBuffer& sonRecup, Arme& arme);

	private :
		virtual void recup(Guillaume& guigui);
};



class BonusRevisionLaVeille : public Bonus
{
	public :
		//Constructeurs et destructeurs
		BonusRevisionLaVeille(int posX, int posY, int vitX, int vitY, int probApp, sf::Sprite* ptSprite, sf::SoundBuffer& sonRecup, Arme& arme);

	private :
		virtual void recup(Guillaume& guigui);
};



class BonusRevisionTrois : public Bonus
{
	public :
		//Constructeurs et destructeurs
		BonusRevisionTrois(int posX, int posY, int vitX, int vitY, int probApp, sf::Sprite* ptSprite, sf::SoundBuffer& sonRecup, Arme& arme);

	private :
		virtual void recup(Guillaume& guigui);
};



class BonusSoin : public Bonus
{
	public :
		//Constructeurs et destructeurs
		BonusSoin(int posX, int posY, int vitX, int vitY, int probApp, sf::Sprite* ptSprite, sf::SoundBuffer& sonRecup, Arme& arme);

	private :
		virtual void recup(Guillaume& guigui);
};



class BonusGingerXplosion : public Bonus
{
	public :
		//Constructeurs et destructeurs
		BonusGingerXplosion(int posX, int posY, int vitX, int vitY, int probApp, sf::Sprite* ptSprite, sf::SoundBuffer& sonRecup, Arme& arme);

	private :
		virtual void recup(Guillaume& guigui);
};



class ListeBonus
{
	public :
		//Constructeur
		ListeBonus();
		
		//Méthodes
		void ajouterBonus(int const dimension[], Bonus* nvBonus);
		void spawnBonus();
		void mettreAJour(int const dimension[], Guillaume &guigui);
		
		void supprimerBonus();
		void afficher(sf::RenderWindow& fenetre, int frame);

	private :
		double m_cooldownCreation;
		sf::Clock m_timerCreation;
		std::vector<Bonus*> m_listeBonus;
		sf::Sprite* m_ptSpriteBonus;

};


//----------------------------------------------------
//            CLASSES DE TIRS
//----------------------------------------------------


class Tir : public ObjVolant
{
	public :
		//Constructeurs et destructeurs
		Tir(int posX, int posY, int vitX, int vitY, int hitBoxX, int hitBoxY, sf::Sprite *ptSprite);
		Tir(int posX, int posY, int vitX, int vitY, sf::Sprite *ptSprite);
		
		//Méthodes de mouvement
		void bouger();
		void mvtLateral();
		void mvtVertical();
};


//----------------------------------------------------
//            CLASSES D'ARMES
//----------------------------------------------------

class Arme
{
	public :
		//Constructeur
		Arme(double coolDown, int degats, sf::Sprite* ptSpriteTir, sf::SoundBuffer& bufferTouche, sf::SoundBuffer& bufferTir);

		//Méthodes de combat
		virtual void tirer(Vaisseau* const tireur);
		virtual void ballistique(int const dimension[]);
		virtual void viser(Vaisseau &vaisseau);
		virtual void viser(ArmeeEnnemie& armee);
		virtual void vider();

		//Affichage
		void afficher(sf::RenderWindow &fenetre);

		//Accesseur
		virtual Arme* getAdresse();
		std::vector<Tir>& getTirsEnCours();
		std::wstring getNom();
	
	protected :
		double const m_coolDown;
		sf::Clock m_instantDernierTir;
		int m_degats;
		std::vector<Tir> m_tirsEnCours;
	    sf::Sprite* m_ptSpriteTir;
		sf::Sound m_sonTir;
		sf::Sound m_sonTouche;
		std::wstring m_nom;	
};


class RevisionLaVeille : public Arme
{
	public :
		//Constructeur
		RevisionLaVeille(sf::Sprite* ptSpriteTir, sf::SoundBuffer& bufferTouche, sf::SoundBuffer& bufferTir);

		//Méthodes de combat
		virtual void tirer(Vaisseau* const tireur);

		//Accesseur
		virtual RevisionLaVeille* getAdresse();
};


class RevisionTrois : public Arme
{
	public :
		//Constructeur
		RevisionTrois(sf::Sprite* ptSpriteTir, sf::SoundBuffer& bufferTouche, sf::SoundBuffer& bufferTir);

		//Méthodes de combat
		virtual void tirer(Vaisseau* const tireur);

		//Accesseur
		virtual RevisionTrois* getAdresse();
};


class Ermite : public Arme
{
	public :
		//Constructeur
		Ermite(sf::Sprite* ptSpriteTir, sf::SoundBuffer& bufferTouche, sf::SoundBuffer& bufferTir, Vaisseau* tireur);

		//Méthodes de combat
		virtual void tirer(Vaisseau* const tireur);
		virtual void ballistique(int const dimension[]);
		virtual void viser(Ennemi &ennemi);
		void viser(ArmeeEnnemie& armee);
		
		//Accesseur
		virtual Ermite* getAdresse();

	private :
		double const m_dureeLaser;
		Vaisseau* const m_tireur;
};


class QuestionPiege : public Arme
{
	public :
		//Constructeur
		QuestionPiege(sf::Sprite* ptSpriteTir, sf::SoundBuffer& bufferTouche, sf::SoundBuffer& bufferTir);
		void viser(Vaisseau& vaisseau);

		//Méthodes de combat
		virtual void tirer(Vaisseau* const tireur);

		//Accesseur
		virtual QuestionPiege* getAdresse();
};


//----------------------------------------------------
//            CLASSES DE ArmeeEnnemie
//----------------------------------------------------

class ArmeeEnnemie
{
	public :
		//Constructeur
		ArmeeEnnemie(sf::Sprite* ptSpriteEnnemi, sf::SoundBuffer& bufferMort);
		
		//Méthodes
		void ajouterEnnemi(int const dimension[], Arme& arme);
		void mettreAJour(int const dimension[], Guillaume &guigui);
		void supprimerEnnemis();
		void afficher(sf::RenderWindow& fenetre, int frame);
		friend void Arme::viser(ArmeeEnnemie& armee);
		friend void Ermite::viser(ArmeeEnnemie& armee);

		//Assesseurs
		int getSizeListeEnnemis();
		std::vector<Ennemi>& getListeEnnemis(); 

	private :
		double m_cooldownCreation;
		sf::Clock m_timerCreation;
		std::vector<Ennemi> m_listeEnnemis;
		sf::Sprite* m_ptSpriteEnnemi;
		sf::Sound m_sonExplosion;


};

#endif
