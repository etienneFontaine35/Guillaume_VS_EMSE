#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <map>
#include "Objet_Volant.hpp"
#include "Tools.hpp"
#include "Background.hpp"
#include "Menu.hpp"
#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/map.hpp>


#define SAVE_FILE "score.save" 
#define MAX_SAVES 10
#define LONGUEUR 1600
#define HAUTEUR 900
#define LONG_PSEUDO 8


using namespace std;

// TODO: supprimer les buts et mplémenter un déplacement comme les bonus
// TODO: son des bonus (Ginger OK) !!! puis régler le bon taux d'apparition et la probabilité de chaque bonus
// TODO: rajouter 2 boutons sur le menus : pour couper la musique et les bruitages en jeu
// TODO: changer la musique du menu par qqch de bcp plus dank façon RC de rue
// TODO: rajouter "Jackson knew ..." en freezant les animations au moment de la mort
// TODO: rajouter "John Cena" à chaque passage d'année avec animation blingbling
// TODO: à leur mort, les ennemis partent dans tous les sens façon shooting star avec goofy yell
// TODO: regler tous les hyperparametres (duree d'une annee, apparition des bonusetc...)
// TODO: comportements des ennemis étranges et pas aléatoires (ils se suivent ??) ! revoir l'apparition uniforme
// TODO: uniformiser la vitesse peu importe la direction

// TODO: ++++ : implémenter une intelligence évolutive pour les ennemis

int main()
{

	//-------------------------------------------------------
	//Initialisation des objets et chargements des ressources
	//-------------------------------------------------------
	
	//--------Création de la fenetre-------------------------
	int const dimension[2] = {LONGUEUR, HAUTEUR};
	sf::RenderWindow fenetre(sf::VideoMode(dimension[0], dimension[1]), "Guillaume VS EMSE", sf::Style::Titlebar | sf::Style::Close);
	fenetre.setFramerateLimit(60);
	Inputs inputs;
	
	//--------Gestion des sauvegardes------------------------
	bool ecriture(false);
	string tmp;
	Save imageSauvegarde(MAX_SAVES);

	ifstream fichierSauvegardeLecture(SAVE_FILE, ios::in);
 
	if(fichierSauvegardeLecture)
	{       
		boost::archive::binary_iarchive archiveLecture(fichierSauvegardeLecture);
		archiveLecture >> imageSauvegarde;
		
		fichierSauvegardeLecture.close();
	}



	//--------Ressources graphiques--------------------------	
	
	// ARMES
	sf::Texture textureTirEnnemi;
	if (!textureTirEnnemi.loadFromFile("textures/sprite_tir_ennemi.bmp"))
		cout << "erreur de chargement de texture TirEnnemi" << endl;

	sf::Sprite spriteTirEnnemi(textureTirEnnemi);
	

	
	sf::Texture textureTirGuigui;
	if (!textureTirGuigui.loadFromFile("textures/sprite_tir_guigui.bmp"))
		cout << "erreur de chargement de texture TirGuigui" << endl;

	sf::Sprite spriteTirGuigui(textureTirGuigui);


	sf::Texture textureLaser;
	if (!textureLaser.loadFromFile("textures/sprite_laser.bmp"))
		cout << "erreur de chargement de texture Laser" << endl;

	sf::Sprite spriteLaser(textureLaser);



	// BONUS
	sf::Texture textureBonus;
	if (!textureBonus.loadFromFile("textures/bonus.png"))
		cout << "erreur de chargement de texture BonusTest" << endl;

	sf::Sprite spriteBonus(textureBonus);



	// VAISSEAUX
	sf::Texture textureEnnemi;
	if (!textureEnnemi.loadFromFile("textures/sprite_ennemi.bmp"))
		cout << "erreur de chargement de texture Ennemi" << endl;

	sf::Sprite spriteEnnemi(textureEnnemi);


	sf::Texture textureGuillaume;
	if (!textureGuillaume.loadFromFile("textures/sprite_guillaume.bmp"))
		cout << "erreur de chargement de texture Guigui" << endl;

	sf::Sprite spriteGuigui(textureGuillaume);

	
	sf::Texture textureReacteur;
	if (!textureReacteur.loadFromFile("textures/sprite_reacteur.bmp"))
		cout << "erreur de chargement de texture Reacteur" << endl;

	sf::Sprite spriteReacteur(textureReacteur);



	// BACKGROUND, UI & OPENING
	Background background("textures/fond_etoile_premier.bmp", "textures/fond_etoile_deuxieme.bmp", "textures/sprite_particule_back.bmp", "textures/sprite_particule_front.bmp", fenetre, dimension); // initialise aussi le random

	Hud hud(fenetre, "textures/espace.ttf", 15);

	Opening opening("textures/pegi_18.png", inputs, hud, 2, 1);


	//--------Ressources audio--------------------------	
	
	// MUSIQUES
	sf::Music musiqueMenu;
	if (!musiqueMenu.openFromFile("sons/Dance_till_youre_dead.ogg"))
		cout << "erreur de chargement de la musique DanceTill" << endl;
	musiqueMenu.setLoop(true);
	musiqueMenu.setVolume(35);
	// musiqueMenu.setVolume(0);

	sf::Music musiqueJeu;
	if (!musiqueJeu.openFromFile("sons/Shooting_stars.ogg"))
		cout << "erreur de chargement de la musique ShootingStars" << endl;
	musiqueJeu.setLoop(true);
	musiqueJeu.setVolume(13);
	// musiqueJeu.setVolume(0);
	
	sf::Music musiqueScore;
	if (!musiqueScore.openFromFile("sons/Spanish_flea.ogg"))
		cout << "erreur de chargement de la musique SpanishFlea" << endl;
	musiqueScore.setVolume(50);
	// musiqueScore.setVolume(0);

	//VAISSEAUX
	sf::SoundBuffer bufferGuiguiTouche;
	if (!bufferGuiguiTouche.loadFromFile("sons/guigui_touche.ogg"))
		cout << "erreur de chargement du son GuiguiTouche" << endl;
	
	sf::SoundBuffer bufferEnnemiTouche;
	if (!bufferEnnemiTouche.loadFromFile("sons/ennemi_touche.ogg"))
		cout << "erreur de chargement du son EnnemiTouche" << endl;

	sf::SoundBuffer bufferEnnemiExplosion;
	if (!bufferEnnemiExplosion.loadFromFile("sons/explosion_ennemi.ogg"))
		cout << "erreur de chargement du son EnnemiExplosion" << endl;
	
	sf::SoundBuffer bufferGuiguiExplosion;
	if (!bufferGuiguiExplosion.loadFromFile("sons/explosion_guigui.ogg"))
		cout << "erreur de chargement du son GuiguiExplosion" << endl;

	sf::Sound sonGuiguiExplosion;
	sonGuiguiExplosion.setBuffer(bufferGuiguiExplosion);

	//ARMES
	sf::SoundBuffer bufferRevisionVeille;
	if (!bufferRevisionVeille.loadFromFile("sons/revisionVeille.ogg"))
		cout << "erreur de chargement du son RevisionVeille" << endl;
	
	sf::SoundBuffer bufferRevisionTrois;
	if (!bufferRevisionTrois.loadFromFile("sons/revisionTrois.ogg"))
		cout << "erreur de chargement du son RevisionTrois" << endl;
	
	sf::SoundBuffer bufferErmite;
	if (!bufferErmite.loadFromFile("sons/ermite.ogg"))
		cout << "erreur de chargement du son Ermite" << endl;

	sf::SoundBuffer bufferQuestionPiege;
	if (!bufferQuestionPiege.loadFromFile("sons/questionPiege.ogg"))
		cout << "erreur de chargement du son QuestionPiege" << endl;

	//BONUS
	sf::SoundBuffer bufferVeilleBonus;
	if (!bufferVeilleBonus.loadFromFile("sons/bonus_veille.ogg"))
		cout << "erreur de chargement du son BonusVeille" << endl;
	
	sf::SoundBuffer bufferTroisBonus;
	if (!bufferTroisBonus.loadFromFile("sons/bonus_trois.ogg"))
		cout << "erreur de chargement du son BonusTrois" << endl;
	
	sf::SoundBuffer bufferErmiteBonus;
	if (!bufferErmiteBonus.loadFromFile("sons/bonus_ermite.ogg"))
		cout << "erreur de chargement du son BonusErmite" << endl;
	
	sf::SoundBuffer bufferSoinBonus;
	if (!bufferSoinBonus.loadFromFile("sons/bonus_soin.ogg"))
		cout << "erreur de chargement du son BonusSoin" << endl;
	
	sf::SoundBuffer bufferGingerBonus;
	if (!bufferGingerBonus.loadFromFile("sons/bonus_ginger.ogg"))
		cout << "erreur de chargement du son BonusGinger" << endl;
	
	//OPENING
	sf::SoundBuffer bufferPegi18;
	if (!bufferPegi18.loadFromFile("sons/pegi_18.ogg"))
		cout << "erreur de chargement du son Pegi 18" << endl;

	sf::Sound sonPegi18;
	sonPegi18.setBuffer(bufferPegi18);

	//-------Interface-----------------------------
	
	sf::Texture textureTitre;
	if (!textureTitre.loadFromFile("textures/titre.bmp"))
		cout << "erreur de chargement de texture Titre" << endl;
	sf::Sprite spriteTitre(textureTitre);
	spriteTitre.setPosition(sf::Vector2f((dimension[0] - spriteTitre.getLocalBounds().width) / 2, 30));

	sf::Texture textureBarrePseudo;
	if (!textureBarrePseudo.loadFromFile("textures/barre_pseudo.bmp"))
		cout << "erreur de chargement de texture BarrePseudo" << endl;
	sf::Sprite spriteBarrePseudo(textureBarrePseudo);

	Menu menuPrincipal;
	menuPrincipal.setActivite(true);
	Menu menuScores;
	
	Entree jouer(fenetre, L"Jouer", "textures/espace.ttf", 40, "textures/fond_entree_menu.bmp", (dimension[0]-600)/2, 350, 600, 103);
	menuPrincipal.ajouterEntree(&jouer);
			
	Entree scores(fenetre, L"Scores", "textures/espace.ttf", 40, "textures/fond_entree_menu.bmp", (dimension[0]-600)/2, 300 + 103 + 95, 600, 103);
	menuPrincipal.ajouterEntree(&scores);
	
	Entree quitter(fenetre, L"Quitter", "textures/espace.ttf", 30, "textures/fond_quitter_menu.bmp", dimension[0] - 50 - 400, dimension[1] - 50 - 103, 400, 103);
	menuPrincipal.ajouterEntree(&quitter);

	Entree retour(fenetre, L"Retour", "textures/espace.ttf", 40, "textures/fond_entree_menu.bmp", (dimension[0]-600)/2, 750, 600, 103);
	menuScores.ajouterEntree(&retour);


	//---------------------
	//      Openings
	//---------------------
		
	string studionNom("Fonfon studio presents");
	string presentationJeu("Apres avoir bu trop de sirop d'erable, Guillaume s\'est endormi \ndans son lit a Montreal. Dans son reve il se voit retourner en\npremiere annee a l\'EMSE, un peu avant Noel. Il n\'est alors\npas encore dans la celebre universite Quebequoise, mais il\nfait tout son possible pour obtenir le meilleur GPA et ainsi etre\nadmis !\n\nMalheureusement la vie n\'est pas toute rose en cette periode\nde l\'annee, et Marc Roelens ne cesse de rappeler qu\'au dela\nde 3 rattrapages toute chance d\'integrer un DD est perdue !\n\nA l\'aide de sa determination, Guillaume passe les annees tout\nen franchissant les obstacles qui deviennent de plus en plus\ndifficiles. \n\nMais Guigui n\'est pas un simple narvalo comme les autres, il\nsait qu\'une bonne methode et qu\'une bonne biere intense\npeuvent (presque) toujours arranger les choses...");
	
	opening.setTempsAffichage(2);
	sonPegi18.play();
	opening.executer(fenetre);
	sonPegi18.stop();

	opening.setTempsAffichage(2);
	opening.executer(fenetre, studionNom, 40, dimension);

	opening.setTempsAffichage(40);
	opening.executer(fenetre, presentationJeu, 30, dimension);


	while (fenetre.isOpen())
	{
		//---------------------
		//       Menus
		//---------------------
		
		//-----Menu principal---------

		menuPrincipal.resetInteraction();
		

		while (menuPrincipal.getActivite() && fenetre.isOpen())
		{
			if (musiqueMenu.getStatus() == sf::SoundStream::Stopped )
				musiqueMenu.play();

			inputs.gererEvents(fenetre);

			jouer.survoler(inputs);
			scores.survoler(inputs);
			quitter.survoler(inputs);

			if (menuPrincipal.peutInteragir())
			{
				jouer.interagir(inputs);
				quitter.interagir(inputs);
				scores.interagir(inputs);
			}

			if (jouer.getInteraction())
			{
				menuPrincipal.resetTimer();
				menuPrincipal.setActivite(false);
			}

			if (quitter.getInteraction())
			{
				ofstream fichierSauvegardeEcriture(SAVE_FILE, ios::out);
 
				if(fichierSauvegardeEcriture)
				{       
					boost::archive::binary_oarchive archiveEcriture(fichierSauvegardeEcriture);
					archiveEcriture << imageSauvegarde;
					
					fichierSauvegardeLecture.close();
				}

				fenetre.close();

			}

			if (scores.getInteraction())
			{
				menuPrincipal.resetTimer();
				menuPrincipal.setActivite(false);
				menuScores.setActivite(true);
			}
				

			fenetre.clear(sf::Color::Black);	
			background.mettreAJour(dimension);
			menuPrincipal.afficher();
			fenetre.draw(spriteTitre);
			fenetre.display();

		}

		
		//-----Menu des scores-----------

		menuScores.resetInteraction();	

		while (menuScores.getActivite())
		{
			inputs.gererEvents(fenetre);

			retour.survoler(inputs);

			if (menuScores.peutInteragir())
				retour.interagir(inputs);

			if (retour.getInteraction())
			{
				menuScores.resetTimer();

				if (musiqueScore.getStatus() == sf::SoundStream::Playing)
					musiqueScore.stop();

				menuScores.setActivite(false);
				menuPrincipal.setActivite(true);	
			}

			fenetre.clear(sf::Color::Black);	
			background.mettreAJour(dimension);
			hud.setTexte("MEILLEURS  SCORES");
			hud.setTaille(45);
			hud.afficher(378, 30, sf::Color(24, 207, 201));
			menuScores.afficher();
			
			map<string, int> listeSauvegardes(imageSauvegarde.getListe());
			int index(0);
			for (map<string, int>::iterator iter = listeSauvegardes.begin(); iter != listeSauvegardes.end(); iter++)
			{
				hud.setTexte(iter->first);
				hud.setTaille(25);
				hud.afficher(450, 150 + index*(30 + 25), sf::Color(230, 230, 230));
				hud.setTexte(to_string(iter->second));
				hud.afficher(dimension[0] - 450 - hud.getLargeurTexte(), 150 + index*(30 + 25), sf::Color(230, 230, 230));
				index++;
			}

			fenetre.display();

		}	


		//---------------------
		//   Boucle de jeu
		//---------------------

		if (!menuPrincipal.getActivite() && !menuScores.getActivite())
		{
			musiqueMenu.stop();
			
			//--------Création des entités de jeu-----------
			Guillaume guigui(0, 0, 9, 9, 75, 97, 3, &spriteGuigui, &spriteReacteur);

			RevisionLaVeille revisionLaVeille(&spriteTirGuigui, bufferEnnemiTouche, bufferRevisionVeille);
			RevisionTrois revisionTroisMois(&spriteTirGuigui, bufferEnnemiTouche, bufferRevisionTrois);
			Ermite ermite(&spriteLaser, bufferEnnemiTouche, bufferErmite, &guigui);
			QuestionPiege questionPiege(&spriteTirEnnemi, bufferGuiguiTouche, bufferQuestionPiege);

			guigui.setArme(revisionLaVeille);
			guigui.setPosX((dimension[0] - guigui.getHitBoxX())/2);
			guigui.setPosY(dimension[1] - guigui.getHitBoxY());
			
			ArmeeEnnemie armeeEnnemie(&spriteEnnemi, bufferEnnemiExplosion);	
		
			BonusRevisionLaVeille veilleBonus(dimension[0]/2, 0, 0, 3, 30, &spriteBonus, bufferVeilleBonus, revisionLaVeille);
			BonusRevisionTrois troisBonus(dimension[0]/2, 0, 0, 3, 30, &spriteBonus, bufferTroisBonus, revisionTroisMois);
			BonusErmite ermiteBonus(dimension[0]/2, 0, 0, 3, 10, &spriteBonus, bufferErmiteBonus, ermite);
			BonusSoin soinBonus(dimension[0]/2, 0, 0, 3, 20, &spriteBonus, bufferSoinBonus, questionPiege);
			BonusGingerXplosion gingerBonus(dimension[0]/2, 0, 0, 3, 10, &spriteBonus, bufferGingerBonus, questionPiege);
			ListeBonus listeBonus;
			listeBonus.ajouterBonus(dimension, &veilleBonus);
			listeBonus.ajouterBonus(dimension, &troisBonus);
			listeBonus.ajouterBonus(dimension, &ermiteBonus);
			listeBonus.ajouterBonus(dimension, &soinBonus);
			listeBonus.ajouterBonus(dimension, &gingerBonus);

			//-------Initialisation des mesures------------
			int frame(0);
			int annee(0);
			int GPA(0);
			sf::Clock timer;
			timer.restart();
			hud.setTaille(15);

			musiqueJeu.play();
			
			while (guigui.getIntact() && fenetre.isOpen())
			{
				//---Mise à jour des entités---
				
				if (timer.getElapsedTime().asSeconds() > 5 /*23.3*/)
				{
					annee++;
					GPA++;
					timer.restart();
				}
				

				if (armeeEnnemie.getSizeListeEnnemis() < annee)
				 	armeeEnnemie.ajouterEnnemi(dimension, questionPiege);

				guigui.mettreAJour(dimension, inputs);
				guigui.getArme().ballistique(dimension);
				guigui.getArme().viser(armeeEnnemie);

				listeBonus.mettreAJour(dimension, guigui);

				armeeEnnemie.mettreAJour(dimension, guigui);
				armeeEnnemie.supprimerEnnemis();
				questionPiege.ballistique(dimension);
				questionPiege.viser(guigui);

				//---Gestion des évènements---

				inputs.gererEvents(fenetre);

				if (inputs.getTouche().esc)
				{	

					fenetre.close();
				}

				
				//---Affichage des Objets volant---
				fenetre.clear(sf::Color::Black);
				background.mettreAJour(dimension);
				
				guigui.afficher(fenetre, frame);

				listeBonus.afficher(fenetre, frame);

				armeeEnnemie.afficher(fenetre, frame);			
				guigui.getArme().afficher(fenetre);
				questionPiege.afficher(fenetre);

				//---Affichage de l'HUD------------
				hud.setTexte(L"GPA : " + to_wstring(GPA));
				hud.afficher(20, 20, sf::Color(200, 200, 200));

				hud.setTexte(L"Année : " + to_wstring(annee) + L"A");
				hud.afficher(20, 20 + hud.getTailleStand() + 10, sf::Color(200, 200, 200));

				hud.setTexte(L"Rattrapage(s) : " + to_wstring(3 - guigui.getVie()));
				hud.afficher(20, dimension[1] - hud.getTailleStand()*2 - 10 - 20, sf::Color(200, 200, 200));

				hud.setTexte(L"Méthode : " + guigui.getArme().getNom());
				hud.afficher(20, dimension[1] - hud.getTailleStand() - 20, sf::Color(200, 200, 200));
				//---------------------------------
				
				fenetre.display();  //inclut la fonction de temporisation entre chaque frame
				frame++;	

			}
			
			musiqueJeu.stop();
			timer.restart();
			bool explosion(true);

			while (timer.getElapsedTime().asSeconds() < 5 && fenetre.isOpen())
			{
				if (explosion)
					sonGuiguiExplosion.play();
				
				explosion = false;
				
				fenetre.clear(sf::Color::Black);
				background.mettreAJour(dimension);
				
				guigui.afficher(fenetre, frame);
				armeeEnnemie.afficher(fenetre, frame);			
				fenetre.display();
				frame++;	

			}

			

			timer.restart();
			musiqueScore.play();


			hud.setTaille(45);
			inputs.setTexte();
			tmp.clear();

			sf::Clock doubleStrokes;
			doubleStrokes.restart();

			while (!inputs.getTouche().entree && fenetre.isOpen())
			{
				inputs.gererEvents(fenetre);
				
				tmp = inputs.getTexte();

				if (tmp.length() > LONG_PSEUDO)
					tmp.pop_back();
				
				if (inputs.getTouche().backSpace && doubleStrokes.getElapsedTime().asSeconds() > 0.1 && tmp.length() > 0)
				{
					tmp.pop_back();
					doubleStrokes.restart();
				}

				inputs.setTexte(tmp);

				fenetre.clear(sf::Color::Black);
				background.mettreAJour(dimension);
				
				int espaceX((dimension[0] - LONG_PSEUDO*(hud.getTailleStand()*1.5))/2);

				for (unsigned int i = 0; i < LONG_PSEUDO; i++)
				{
					if (i < tmp.length())
					{
						hud.setTexte(tmp.substr(i, 1));
						hud.afficher(espaceX + i*(hud.getTailleStand()*1.7), dimension[1]/2, sf::Color(220, 220, 220));
					}

					if (i == tmp.length() || (i == LONG_PSEUDO - 1 && tmp.length() == LONG_PSEUDO))
						spriteBarrePseudo.setPosition(sf::Vector2f(espaceX + i*(hud.getTailleStand()*1.7), dimension[1]/2 + 60 + 20*(frame%40 < 20)));
					
					else
						spriteBarrePseudo.setPosition(sf::Vector2f(espaceX + i*(hud.getTailleStand()*1.7), dimension[1]/2 + 60));

					fenetre.draw(spriteBarrePseudo);
				}

				hud.setTexte("PSEUDO");
				hud.afficher((dimension[0]-hud.getLargeurTexte())/2, dimension[1]/3, sf::Color(24, 207, 201));

				fenetre.display();
				frame++;
				ecriture = true;
			}


			if (ecriture)
			{
				//enregistrement des scores
				imageSauvegarde.ajouterEntree(tmp, GPA);
				ecriture = false;
			}

			
			menuScores.setActivite(true);

		}

	}
	
	return 0;
}







