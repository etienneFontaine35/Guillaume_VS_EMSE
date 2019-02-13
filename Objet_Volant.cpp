#include "Objet_Volant.hpp"
#include "Tools.hpp"
#include <vector>
#include <map>
#include <cmath>
#include <ctime>
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#define PI 3.1416
#define CD_BONUS 6.0
#define VIT_EXPLOSION 30

using namespace std;

//-----------------------
// METHODES DE ObjVolant
//-----------------------

// CONSTRUCTEUR

ObjVolant::ObjVolant(int posX, int posY, int vitX, int vitY, int hitBoxX, int hitBoxY, sf::Sprite* ptSprite) : m_posX(posX), m_posY(posY), m_vitX(vitX), m_vitY(vitY), m_hitBoxX(hitBoxX), m_hitBoxY(hitBoxY), m_ptSprite(ptSprite)
{

}


// METHODES CLASSIQUES

void ObjVolant::monter()
{
	m_posY -= m_vitY;
}


void ObjVolant::descendre()
{
	m_posY += m_vitY;
}


void ObjVolant::allerGauche()
{
	m_posX -= m_vitX;
}


void ObjVolant::allerDroite()
{
	m_posX += m_vitX;
}


bool ObjVolant::collision(ObjVolant const& obj) const
{
	return obj.m_posX<(m_posX+m_hitBoxX) &&
		m_posX<(obj.m_posX+obj.m_hitBoxX) &&
		obj.m_posY<(m_posY+m_hitBoxY) &&
		m_posY<(obj.m_posY+obj.m_hitBoxY);
}


bool ObjVolant::horsMap(int const dimension[]) const
{
	return m_posX > dimension[0] || m_posX + m_hitBoxX < 0 || m_posY > dimension[1] || m_posY + m_hitBoxY < 0;	
}


int ObjVolant::getPosX() const
{
	return m_posX;
}


int ObjVolant::getPosY() const
{
	return m_posY;
}


void ObjVolant::setPosX(int x)
{
	m_posX = x;
}


void ObjVolant::setPosY(int y)
{
	m_posY = y;
}


int ObjVolant::getVitX() const
{
	return m_vitX;
}


int ObjVolant::getVitY() const
{
	return m_vitY;
}


void ObjVolant::setVitX(int x)
{
	m_vitX = x;
}


void ObjVolant::setVitY(int y)
{
	m_vitY = y;
}


int ObjVolant::getHitBoxX() const
{
	return m_hitBoxX;
}


int ObjVolant::getHitBoxY() const
{
	return m_hitBoxY;
}


int ObjVolant::centre(char a) const
{
	switch (a)
	{
		case 'x':
			return m_posX + m_hitBoxX/2;
			break;
		
		case 'y':
			return m_posY + m_hitBoxY/2;
			break;

		default:
			return -1;
			break;	
	}
}


void ObjVolant::afficher(sf::RenderWindow &fenetre)
{
	m_ptSprite->setPosition(sf::Vector2f(m_posX, m_posY));
	fenetre.draw(*m_ptSprite);
}



//-----------------------
// METHODES DE Vaisseau
//-----------------------

// CONSTRUCTEURS ET DESTRUCTEURS
Vaisseau::Vaisseau(int posX, int posY, int vitX, int vitY, int hitBoxX, int hitBoxY, sf::Sprite* ptSprite, double cooldownInvincibilite) : ObjVolant::ObjVolant(posX, posY, vitX, vitY, hitBoxX, hitBoxY, ptSprite), m_cooldownInvincibilite(cooldownInvincibilite), m_intact(true), m_armeEnCours(0)
{
	m_timerInvincibilite.restart();
}

// METHODES CLASSIQUES

void Vaisseau::tirer()
{
	if (m_armeEnCours != 0)
	{
		m_armeEnCours->tirer(this);
	}
	else
		cout << "Pas d'arme équipée !!" << endl;
}


void Vaisseau::perdreVie(int degats)
{
	m_vie -= degats;
	if (m_vie < 0)
		m_vie = 0;
}


void Vaisseau::setArme(Arme &armeObtenue)
{
	m_armeEnCours = armeObtenue.getAdresse();
}


Arme& Vaisseau::getArme()
{
	Arme& refArme = *m_armeEnCours;
	return refArme;
}


int Vaisseau::getVie() const
{
	return m_vie;
}


void Vaisseau::setIntact(bool intact)
{
	m_intact = intact;
}

bool Vaisseau::getIntact()
{
	return m_intact;
}


void Vaisseau::resetInvincibilite()
{
	m_timerInvincibilite.restart();
}

double Vaisseau::getInvinProp()
{
	double sol(m_timerInvincibilite.getElapsedTime().asSeconds()/m_cooldownInvincibilite);
	if (sol > 1)
		sol = 1;
	return sol; 
}




//-----------------------
// METHODES DE Guillaume
//-----------------------

// CONSTRUCTEURS ET DESTRUCTEURS

Guillaume::Guillaume(int posX, int posY, int vitX, int vitY, int hitBoxX, int hitBoxY, int vie, sf::Sprite* ptSprite, sf::Sprite* ptSpriteReac) : Vaisseau::Vaisseau(posX, posY, vitX, vitY, hitBoxX, hitBoxY, ptSprite, 2), m_ptSpriteReacteur(ptSpriteReac)
{
	m_vie = vie;	
}


// METHODES CLASSIQUES

void Guillaume::monter(int const dimension[])
{
	ObjVolant::monter();
	if (m_posY < 1)
		m_posY = 0;
}


void Guillaume::descendre(int const dimension[])
{
	ObjVolant::descendre();
	if (m_posY >= dimension[1] - m_hitBoxY)
		m_posY = dimension[1] - m_hitBoxY;
}


void Guillaume::allerGauche(int const dimension[])
{
	ObjVolant::allerGauche();
	if (m_posX < 1)
		m_posX = 0;
}


void Guillaume::allerDroite(int const dimension[])
{
	ObjVolant::allerDroite();
	if (m_posX >= dimension[0] - m_hitBoxX)
		m_posX = dimension[0] - m_hitBoxX;
}


void Guillaume::mettreAJour(int const dimension[], Inputs& inputs)
{
	Touche etatTouches = inputs.getTouche();
	
	if (etatTouches.haut)
		monter(dimension);

	if (etatTouches.bas)
		descendre(dimension);
	
	if (etatTouches.gauche)
		allerGauche(dimension);
	
	if (etatTouches.droite)
		allerDroite(dimension);
	
	if (etatTouches.z)
		tirer();
	
	if (!getVie())
	{
		if (getIntact())
			resetInvincibilite();	
	
		setIntact(false);
	}

}


void Guillaume::seSoigner(int soin)
{
	m_vie += soin;
	
	if (m_vie > 3)
		m_vie = 3;
}


void Guillaume::afficher(sf::RenderWindow &fenetre, int frame)
{
	if (getIntact())
	{
		m_ptSpriteReacteur->setTextureRect(sf::IntRect((frame%10 < 5)*25, 0, 25, 40));
		m_ptSpriteReacteur->setPosition(sf::Vector2f(ObjVolant::centre('x')-8, ObjVolant::getPosY()+ObjVolant::getHitBoxY()-5));
		fenetre.draw(*m_ptSpriteReacteur);
	}	
		
	int img = getIntact()*(frame%120 < 12) + (!getIntact())*(3 + int(getInvinProp()*4));
	
	m_ptSprite->setTextureRect(sf::IntRect(img*75, 0, 75, 97));
	m_ptSprite->setColor(sf::Color(255, 255, 255));

	ObjVolant::afficher(fenetre);
	
	if (frame > 180)
	{
		m_ptSprite->setTextureRect(sf::IntRect(75*2, 0, 75, 97));
		m_ptSprite->setColor(sf::Color(255, 255, 255, (1 - getInvinProp())*255));
	}
	
	ObjVolant::afficher(fenetre);
}


//-----------------------
// METHODES DE Ennemi
//-----------------------

// CONSTRUCTEURS ET DESTRUCTEURS

Ennemi::Ennemi(int posX, int posY, int vitX, int vitY, int hitBoxX, int hitBoxY, int vie, sf::Sprite* ptSprite) : Vaisseau::Vaisseau(posX, posY, vitX, vitY, hitBoxX, hitBoxY, ptSprite, 0.5), m_vitesseExplosion(VIT_EXPLOSION)
{
	m_vie = vie;	
}


Ennemi::~Ennemi()	
{

}


// METHODES CLASSIQUES

void Ennemi::monter(int const dimension[])
{
	ObjVolant::monter();
	if (m_posY < - m_hitBoxY)
		m_posY = dimension[1];
}


void Ennemi::descendre(int const dimension[])
{
	ObjVolant::descendre();
	if (m_posY > dimension[1])
		m_posY = - m_hitBoxY;
}


void Ennemi::allerGauche(int const dimension[])
{
	ObjVolant::allerGauche();
	if (m_posX < - m_hitBoxX)
		m_posX = dimension[0];
}


void Ennemi::allerDroite(int const dimension[])
{
	ObjVolant::allerDroite();
	if (m_posX > dimension[0])
		m_posX = - m_hitBoxX;
}


void Ennemi::mettreAJour(int const dimension[], Guillaume& guigui)
{
	if (getIntact())
	{
		// if (randInteger(40) == 0 || ( guigui.centre('x')-guigui.getHitBoxX() < centre('x') && centre('x') < guigui.centre('x')+guigui.getHitBoxX()))
			// tirer();

	}

	else
	{
		ObjVolant::allerDroite();
		ObjVolant::descendre();
	}
	
}



void Ennemi::afficher(sf::RenderWindow &fenetre, int frame)
{
	int img = getIntact()*(frame%58 < 29) + (!getIntact())*(3 + int(getInvinProp()*4));
	
	m_ptSprite->setColor(sf::Color(255, 255, 255));
	m_ptSprite->setTextureRect(sf::IntRect(img*75, 0, 75, 100));
	if (!getIntact())
		m_ptSprite->setRotation(5*frame);

	ObjVolant::afficher(fenetre);
	m_ptSprite->setRotation(0);

	if (frame > 180)
	{
		m_ptSprite->setTextureRect(sf::IntRect(75*2, 0, 75, 92));
		m_ptSprite->setColor(sf::Color(255, 255, 255, (1 - getInvinProp())*255));
	}
	
	ObjVolant::afficher(fenetre);

}


//--------------------------
// METHODES DE ArmeeEnnemi
//--------------------------


ArmeeEnnemie::ArmeeEnnemie(sf::Sprite* ptSpriteEnnemi, sf::SoundBuffer& bufferMort) : m_cooldownCreation(3.0), m_ptSpriteEnnemi(ptSpriteEnnemi)
{
	m_sonExplosion.setBuffer(bufferMort);
	m_sonExplosion.setVolume(25);
	m_timerCreation.restart();
}


void ArmeeEnnemie::ajouterEnnemi(int const dimension[], Arme& arme)
{
	double alea = probUniform() * 4.0 - 2.0; //distribution prob. non uniforme a revoir
	if (m_timerCreation.getElapsedTime().asSeconds() > m_cooldownCreation + alea)
	{
		Ennemi ennemi(0, 0, 5, 5, 75, 92, 2, m_ptSpriteEnnemi);
		ennemi.setPosX(randInteger(dimension[0] - ennemi.getHitBoxX()));
		ennemi.setPosY(-ennemi.getHitBoxY()*0);	
		ennemi.setArme(arme);
		m_listeEnnemis.push_back(ennemi);
		m_timerCreation.restart();
	}
}


void ArmeeEnnemie::mettreAJour(int const dimension[], Guillaume &guigui)
{
	for (vector<Ennemi>::iterator ennemi = m_listeEnnemis.begin(); ennemi != m_listeEnnemis.end(); ennemi++)
	{
		if (!ennemi->getVie())
		{
			if (ennemi->getIntact())
			{
				double angle(PI * randInteger(359) / 180);
				ennemi->setVitX(VIT_EXPLOSION*cos(angle));
				ennemi->setVitY(VIT_EXPLOSION*sin(angle));

				m_sonExplosion.play();
				ennemi->resetInvincibilite();	
			}

			ennemi->setIntact(false);
		}
		

		// FIXME: COMPORTEMENT DE L'ENNEMI
		ennemi->mettreAJour(dimension, guigui);

	}

}


void ArmeeEnnemie::supprimerEnnemis()
{
	vector<int> aSuppr;
	int i(0);

	for (vector<Ennemi>::iterator ennemi = m_listeEnnemis.begin(); ennemi != m_listeEnnemis.end(); ennemi++)
	{
		if ((!ennemi->getIntact()) && (ennemi->getInvinProp() == 1))
		{
			aSuppr.push_back(i);
		}
		i++;
	}

	for (int index = aSuppr.size() - 1; index >= 0; index--)
		m_listeEnnemis.erase(m_listeEnnemis.begin() + aSuppr[index]);
}



void ArmeeEnnemie::afficher(sf::RenderWindow& fenetre, int frame)
{
	for (vector<Ennemi>::iterator ennemi = m_listeEnnemis.begin(); ennemi != m_listeEnnemis.end(); ennemi++)
		ennemi->afficher(fenetre, frame);
}


int ArmeeEnnemie::getSizeListeEnnemis()
{
	return m_listeEnnemis.size();
}


std::vector<Ennemi>& ArmeeEnnemie::getListeEnnemis()
{
	std::vector<Ennemi>& refListe(m_listeEnnemis);
	return refListe;	
}	


//-----------------------
// METHODES DE Bonus
//-----------------------

// CONSTRUCTEURS ET DESTRUCTEURS
Bonus::Bonus(int posX, int posY, int vitX, int vitY, int hitBoxX, int hitBoxY, int probApp, sf::Sprite* ptSprite, sf::SoundBuffer& sonRecup, Arme& arme) : ObjVolant::ObjVolant(posX, posY, vitX, vitY, hitBoxX, hitBoxY, ptSprite), m_spawned(false), m_probApp(probApp), m_originX(posX), m_arme(arme)
{
	m_sonRecup.setBuffer(sonRecup);
	m_sonRecup.setVolume(25);
	nvCarac();
}


Bonus::Bonus(int posX, int posY, int vitX, int vitY, int probApp, sf::Sprite* ptSprite, sf::SoundBuffer& sonRecup, Arme& arme) : Bonus::Bonus(posX, posY, vitX, vitY, 30, 70, probApp, ptSprite, sonRecup, arme)
{
	
}


// METHODES CLASSIQUES
void Bonus::bouger(int const dimension[])
{
	Bonus::mvtVertical();
	int decalage = std::round( m_amplitude * std::sin(m_posY * PI * m_nbreNoeuds / dimension[1]) );
	Bonus::setPosX(m_originX + decalage);

	if (ObjVolant::horsMap(dimension))
	{
		Bonus::setSpawned(false);
	}
}


void Bonus::mvtLateral()
{
	m_posX += m_vitX;
}


void Bonus::mvtVertical()
{
	m_posY += m_vitY;
}


void Bonus::recup(Guillaume& guigui)
{
	if (ObjVolant::collision(guigui))
	{
		std::cout << "Erreur de résolution : méthode de Bonus de base" << std::endl;
		m_sonRecup.play();
		setSpawned(false);
	}
	
}


void Bonus::afficher(sf::RenderWindow &fenetre, int frame)
{
	int img = (frame % 60) / 10;
	m_ptSprite->setTextureRect(sf::IntRect(img*30, 0, 30, 70));

	ObjVolant::afficher(fenetre);
}


int Bonus::getProbApp()
{
	return m_probApp;
}


bool Bonus::isSpawned()
{
	return m_spawned;
}


void Bonus::setSpawned(bool state)
{
	m_spawned = state;
}


void Bonus::setAmplitude(int nvAmpl)
{
	m_amplitude = nvAmpl;
}


void Bonus::setNbreNoeuds(int nvNbre)
{
	m_nbreNoeuds = nvNbre;
}


void Bonus::setOriginX(int posX)
{
	m_originX = posX;
}


void Bonus::nvCarac()
{
	setAmplitude(200 * randInteger(-1, 1));
	setNbreNoeuds(randInteger(0, 3));
}



//-----------------------
// METHODES DE BonusErmite
//-----------------------

// CONSTRUCTEURS ET DESTRUCTEURS
BonusErmite::BonusErmite(int posX, int posY, int vitX, int vitY, int probApp, sf::Sprite* ptSprite, sf::SoundBuffer& sonRecup, Arme& arme) : Bonus(posX, posY, vitX, vitY, probApp, ptSprite, sonRecup, arme)
{
	
}


// METHODES CLASSIQUES
void BonusErmite::recup(Guillaume& guigui)
{
	if (ObjVolant::collision(guigui))
	{
		guigui.setArme(m_arme);
		guigui.getArme().vider();
		m_sonRecup.play();
		setSpawned(false);
	}
}



//-----------------------
// METHODES DE BonusRevisionVeille
//-----------------------

// CONSTRUCTEURS ET DESTRUCTEURS
BonusRevisionLaVeille::BonusRevisionLaVeille(int posX, int posY, int vitX, int vitY, int probApp, sf::Sprite* ptSprite, sf::SoundBuffer& sonRecup, Arme& arme) : Bonus(posX, posY, vitX, vitY, probApp, ptSprite, sonRecup, arme)
{
	
}


// METHODES CLASSIQUES
void BonusRevisionLaVeille::recup(Guillaume& guigui)
{
	if (ObjVolant::collision(guigui))
	{
		guigui.setArme(m_arme);
		guigui.getArme().vider();
		m_sonRecup.play();
		setSpawned(false);
	}
}



//-----------------------
// METHODES DE BonusTroisMois
//-----------------------

// CONSTRUCTEURS ET DESTRUCTEURS
BonusRevisionTrois::BonusRevisionTrois(int posX, int posY, int vitX, int vitY, int probApp, sf::Sprite* ptSprite, sf::SoundBuffer& sonRecup, Arme& arme) : Bonus(posX, posY, vitX, vitY, probApp, ptSprite, sonRecup, arme)
{
	
}


// METHODES CLASSIQUES
void BonusRevisionTrois::recup(Guillaume& guigui)
{
	if (ObjVolant::collision(guigui))
	{
		guigui.setArme(m_arme);
		guigui.getArme().vider();
		m_sonRecup.play();
		setSpawned(false);
	}
}



//-----------------------
// METHODES DE BonusSoin
//-----------------------

// CONSTRUCTEURS ET DESTRUCTEURS
BonusSoin::BonusSoin(int posX, int posY, int vitX, int vitY, int probApp, sf::Sprite* ptSprite, sf::SoundBuffer& sonRecup, Arme& arme) : Bonus(posX, posY, vitX, vitY, probApp, ptSprite, sonRecup, arme)
{
	
}


// METHODES CLASSIQUES
void BonusSoin::recup(Guillaume& guigui)
{
	if (ObjVolant::collision(guigui))
	{
		if (guigui.getVie() < 3)
			guigui.seSoigner(1);

		m_sonRecup.play();
		setSpawned(false);
	}
}



//-----------------------
// METHODES DE BonusSoin
//-----------------------

// CONSTRUCTEURS ET DESTRUCTEURS
BonusGingerXplosion::BonusGingerXplosion(int posX, int posY, int vitX, int vitY, int probApp, sf::Sprite* ptSprite, sf::SoundBuffer& sonRecup, Arme& arme) : Bonus(posX, posY, vitX, vitY, probApp, ptSprite, sonRecup, arme)
{
	
}


// METHODES CLASSIQUES
void BonusGingerXplosion::recup(Guillaume& guigui)
{
	if (ObjVolant::collision(guigui))
	{
		if (guigui.getVie() > 1)
			guigui.perdreVie(1);
			
		m_sonRecup.play();
		setSpawned(false);
	}
}


//---------------------------
// METHODES DE ListeBonus
//---------------------------

// CONSTRUCTEURS ET DESTRUCTEURS
ListeBonus::ListeBonus() : m_cooldownCreation(CD_BONUS)
{
	m_timerCreation.restart();
}

// METHODES CLASSIQUES
void ListeBonus::ajouterBonus(int const dimension[], Bonus* nvBonus)
{
	nvBonus->setPosX(randInteger(dimension[0]));
	nvBonus->setPosY(-nvBonus->getHitBoxY());
	m_listeBonus.push_back(nvBonus);
}


void ListeBonus::spawnBonus()
{
	int sommeProb(0);
	
	for (vector<Bonus*>::iterator bon = m_listeBonus.begin(); bon != m_listeBonus.end(); bon++)
	{
		sommeProb += (*bon)->getProbApp();
	}

	int choixBonus(randInteger(sommeProb));
	sommeProb = 0;

	for (vector<Bonus*>::iterator bon = m_listeBonus.begin(); bon != m_listeBonus.end() && sommeProb != -1; bon++)
	{
		sommeProb += (*bon)->getProbApp();
		if (choixBonus < sommeProb)
		{
			(*bon)->setSpawned(true);
			(*bon)->setAmplitude(50 * randInteger(-4, 4));
			(*bon)->setNbreNoeuds(randInteger(0, 8));
			sommeProb = -1;
		}
	}

}


void ListeBonus::mettreAJour(int const dimension[], Guillaume &guigui)
{
	for (vector<Bonus*>::iterator bon = m_listeBonus.begin(); bon != m_listeBonus.end(); bon++)
	{
		if ((*bon)->isSpawned())
		{
			(*bon)->bouger(dimension);		
			(*bon)->recup(guigui);
		}

		else
		{
			(*bon)->setPosX(randInteger(dimension[0]));
			(*bon)->setOriginX((*bon)->getPosX());
			(*bon)->setPosY(-(*bon)->getHitBoxY());
			(*bon)->nvCarac();
		}
	}


	if (m_timerCreation.getElapsedTime().asSeconds() > m_cooldownCreation)
	{
		ListeBonus::spawnBonus();
		m_timerCreation.restart();
	}

}


void ListeBonus::afficher(sf::RenderWindow &fenetre, int frame)
{
	for (vector<Bonus*>::iterator bon = m_listeBonus.begin(); bon != m_listeBonus.end(); bon++)
	{
		if ((*bon)->isSpawned())
			(*bon)->afficher(fenetre, frame);		
	}
}








//-----------------------
// METHODES DE Tir
//-----------------------

// CONSTRUCTEURS ET DESTRUCTEURS
Tir::Tir(int posX, int posY, int vitX, int vitY, int hitBoxX, int hitBoxY, sf::Sprite* ptSprite) : ObjVolant::ObjVolant(posX, posY, vitX, vitY, hitBoxX, hitBoxY, ptSprite)
{

}


Tir::Tir(int posX, int posY, int vitX, int vitY, sf::Sprite* ptSprite) : ObjVolant::ObjVolant(posX, posY, vitX, vitY, 10, 10, ptSprite)
{

}


// METHODES CLASSIQUES

void Tir::bouger()
{
	Tir::mvtLateral();
	Tir::mvtVertical();
}


void Tir::mvtLateral()
{
	m_posX += m_vitX;
}


void Tir::mvtVertical()
{
	m_posY += m_vitY;
}


//-----------------------
// METHODES DE Arme
//-----------------------

// CONSTRUCTEURS ET DESTRUCTEURS

Arme::Arme(double coolDown, int degats, sf::Sprite* ptSpriteTir, sf::SoundBuffer& bufferTouche, sf::SoundBuffer& bufferTir) : m_coolDown(coolDown), m_degats(degats), m_ptSpriteTir(ptSpriteTir), m_nom(L"swaggy pute") 
{
	m_sonTouche.setBuffer(bufferTouche);
	m_sonTir.setBuffer(bufferTir);
	m_instantDernierTir.restart();
}

// METHODES CLASSIQUES

Arme* Arme::getAdresse()
{
	return this;
}


vector<Tir>& Arme::getTirsEnCours()
{
	vector<Tir>& refTirs = m_tirsEnCours;
       	return refTirs;
}


void Arme::tirer(Vaisseau* tireur)
{
	cout << "erreur de résolution : méthode de tir de ARME" << endl;
}


void Arme::ballistique(int const dimension[])
{	
	vector<int> aSuppr;
	int i(0);

	for (vector<Tir>::iterator tir = m_tirsEnCours.begin(); tir != m_tirsEnCours.end(); tir++)
	{
		tir->bouger();
		if (tir->horsMap(dimension))
		{
			aSuppr.push_back(i);
		}
		i++;
	}

	for (int index = aSuppr.size() - 1; index >= 0; index--)
		m_tirsEnCours.erase(m_tirsEnCours.begin() + aSuppr[index]);
}


void Arme::viser(Vaisseau& vaisseau)
{
	vector<int> aSuppr;
	int i(0);

	for (vector<Tir>::iterator tir = m_tirsEnCours.begin(); tir != m_tirsEnCours.end(); tir++)
	{
		if (tir->collision(vaisseau))
		{
			m_sonTouche.play();
			vaisseau.resetInvincibilite();
			vaisseau.perdreVie(m_degats);
			aSuppr.push_back(i);
		}
			i++;	
	}

	for (int index = aSuppr.size() - 1; index >= 0; index--)
		m_tirsEnCours.erase(m_tirsEnCours.begin() + aSuppr[index]);
}


void Arme::viser(ArmeeEnnemie& armee)
{
	for (vector<Ennemi>::iterator ennemi = armee.m_listeEnnemis.begin(); ennemi != armee.m_listeEnnemis.end(); ennemi++)
		this->viser(*ennemi);
}


void Arme::vider()
{
	m_tirsEnCours.clear();
}


void Arme::afficher(sf::RenderWindow &fenetre)
{
	for (vector<Tir>::iterator tir = m_tirsEnCours.begin(); tir != m_tirsEnCours.end(); tir++)
	{
		tir->afficher(fenetre);
	}
}


std::wstring Arme::getNom()
{
	return m_nom;
}


// METHODES DES ARMES SPECIFIQUES

// - RevisionLaVeille

RevisionLaVeille::RevisionLaVeille(sf::Sprite* ptSpriteTir, sf::SoundBuffer& bufferTouche, sf::SoundBuffer& bufferTir) : Arme::Arme(0.25, 1, ptSpriteTir, bufferTouche, bufferTir)
{
	m_nom = L"Révision la veille";
	m_sonTir.setVolume(35);
	m_sonTouche.setVolume(55);
}


RevisionLaVeille* RevisionLaVeille::getAdresse()
{
	return this;
}


void RevisionLaVeille::tirer(Vaisseau* tireur)
{
	if (m_instantDernierTir.getElapsedTime().asSeconds() > m_coolDown)
	{
		int vit = 16;
		Tir revisionLaVeilleTir(tireur->centre('x'), tireur->centre('y') - 35, 0, -vit, m_ptSpriteTir);
		revisionLaVeilleTir.setPosX(revisionLaVeilleTir.getPosX()-revisionLaVeilleTir.getHitBoxX()/2 + 6);
		m_tirsEnCours.push_back(revisionLaVeilleTir);
		m_sonTir.play();
		m_instantDernierTir.restart();
	}
	
}


// - RevisionTrois 

RevisionTrois::RevisionTrois(sf::Sprite* ptSpriteTir, sf::SoundBuffer& bufferTouche, sf::SoundBuffer& bufferTir) : Arme::Arme(0.5, 1, ptSpriteTir, bufferTouche, bufferTir)
{
	m_nom = L"Révision trois mois en avance";
	m_sonTouche.setVolume(55);
	m_sonTir.setVolume(30);
}


RevisionTrois* RevisionTrois::getAdresse()
{
	return this;
}


void RevisionTrois::tirer(Vaisseau* tireur)
{
	if (m_instantDernierTir.getElapsedTime().asSeconds() > m_coolDown)
	{
		for (int i = -1 ; i <= 1 ; i++)
		{
			int vit = 13;
			float angle = 3.14159/11;
			Tir revisionTroisTir(tireur->centre('x'), tireur->centre('y') - 35, i*sin(angle)*vit, -vit * (1 - abs(i)*(1-cos(angle)) ), m_ptSpriteTir);
			revisionTroisTir.setPosX(revisionTroisTir.getPosX()-revisionTroisTir.getHitBoxX()/2 + 6);
			
			m_tirsEnCours.push_back(revisionTroisTir);
		}
		m_sonTir.play();	
		m_instantDernierTir.restart();
	}
	
}


// - Ermite

Ermite::Ermite(sf::Sprite* ptSpriteTir, sf::SoundBuffer& bufferTouche, sf::SoundBuffer& bufferTir, Vaisseau* tireur) : Arme::Arme(1.7, 1000, ptSpriteTir, bufferTouche, bufferTir), m_dureeLaser(0.5), m_tireur(tireur)
{
	m_nom = L"Mode Ermite dans sa chambre";
	m_sonTouche.setVolume(55);
	m_sonTir.setVolume(50);
}


Ermite* Ermite::getAdresse()
{
	return this;
}


void Ermite::tirer(Vaisseau* tireur)
{
	if ((!m_tirsEnCours.size()) && m_instantDernierTir.getElapsedTime().asSeconds() > m_coolDown)
	{
		int vit = 0;
		Tir ermiteTir(tireur->centre('x'), tireur->centre('y') - 35 - 900, 0, -vit, 20, 900, m_ptSpriteTir);
		ermiteTir.setPosX(ermiteTir.getPosX()-ermiteTir.getHitBoxX()/2 + 6);
		m_tirsEnCours.push_back(ermiteTir);
		m_sonTir.play();	
		m_instantDernierTir.restart();
	}
	
}


void Ermite::ballistique(int const dimension[])
{
	vector<int> aSuppr;
	int i(0);

	for (std::vector<Tir>::iterator laser = m_tirsEnCours.begin(); laser != m_tirsEnCours.end(); laser++)
	{
		if (m_instantDernierTir.getElapsedTime().asSeconds() > m_dureeLaser)
			aSuppr.push_back(i);
		else
		{
			laser->setPosX(m_tireur->centre('x') - laser->getHitBoxX()/2 + 6);
	       		laser->setPosY(m_tireur->centre('y') - 35 - 900);		
		}
		i++;	
	}

	for (int index = aSuppr.size() - 1; index >= 0; index--)
		m_tirsEnCours.erase(m_tirsEnCours.begin() + aSuppr[index]);
}


void Ermite::viser(Ennemi &ennemi)
{
	for (vector<Tir>::iterator laser = m_tirsEnCours.begin(); laser != m_tirsEnCours.end(); laser++)
	{
		if (laser->collision(ennemi))
		{	
			m_sonTouche.play();
			ennemi.perdreVie(m_degats);
			ennemi.resetInvincibilite();

		}
	}
}


void Ermite::viser(ArmeeEnnemie& armee)
{
	for (vector<Ennemi>::iterator ennemi = armee.m_listeEnnemis.begin(); ennemi != armee.m_listeEnnemis.end(); ennemi++)
		this->Ermite::viser(*ennemi);
}


// - Question piège

QuestionPiege::QuestionPiege(sf::Sprite* ptSpriteTir, sf::SoundBuffer& bufferTouche, sf::SoundBuffer& bufferTir) : Arme::Arme(0.3, 1, ptSpriteTir, bufferTouche, bufferTir)
{
	m_nom = L"Arf ! Il y a des questions pièges";
	m_sonTir.setVolume(20);
	m_sonTouche.setVolume(30);
}


QuestionPiege* QuestionPiege::getAdresse()
{
	return this;
}


void QuestionPiege::tirer(Vaisseau* tireur)
{
	if (m_instantDernierTir.getElapsedTime().asSeconds() > m_coolDown)
	{
		int vit = 10;
		Tir questionPiegeTir(tireur->centre('x'), tireur->centre('y') - 35, 0, vit, m_ptSpriteTir);
		questionPiegeTir.setPosX(questionPiegeTir.getPosX()-questionPiegeTir.getHitBoxX()/2 + 6);
		m_tirsEnCours.push_back(questionPiegeTir);
		m_sonTir.play();
		m_instantDernierTir.restart();
	}
	
}


void QuestionPiege::viser(Vaisseau& vaisseau)
{
	vector<int> aSuppr;
	int i(0);

	for (vector<Tir>::iterator tir = m_tirsEnCours.begin(); tir != m_tirsEnCours.end(); tir++)
	{
		if (tir->collision(vaisseau))
		{	
			if (vaisseau.getInvinProp() == 1)
			{
				m_sonTouche.play();
				vaisseau.perdreVie(m_degats);
				vaisseau.resetInvincibilite();
			}
			aSuppr.push_back(i);
		}
			i++;	
	}

	for (int index = aSuppr.size() - 1; index >= 0; index--)
		m_tirsEnCours.erase(m_tirsEnCours.begin() + aSuppr[index]);
}

