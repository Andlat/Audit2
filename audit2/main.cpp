#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "tinyxml2.h"
#include <time.h>
#include "Question.h"
#include "QuestionsParser.h"

using namespace std;
using namespace tinyxml2;

#define NB_TOURS 15

int main()
{
	srand(int(time(NULL)));

	//question est la question et questions est la structure de gestion du XML
	Question question;
	QuestionsParser questions("questions.xml");

	//chargement du fichier XML de questions en mémoire
	try{
		questions.load();
	}catch (XMLError &err) {
		cout << XMLDocument::ErrorIDToName(err) << endl;
		return -1;
	}

	//Boucle de jeux
	for(unsigned tour=1; tour <= NB_TOURS; ++tour){
		cout << "***********************************************************\n"
			<< "************ WHO WANTS TO BE A MILLIONAIRE ??? ************\n"
			<< "***********************************************************\n"
			<< endl;

		try {//lecture de la prochaine question du XML 
			questions.read(question);
		}catch (XMLError &err) {
			cout << XMLDocument::ErrorIDToName(err) << endl;
			return -2;
		}

		//change l'ordre des choix de manière aléatoire
		question.randomize();

		cout << question;

		char reponse = cin.get();
		cin.get();//Ignorer la touche entree


		if (question.validate(reponse)) {
			cout << "BONNE REPONSE !!" << endl;
		}
		else {
			cout << "Mauvaise reponse... :(\nVous avez perdu\n" << endl;
			cout << "La bonne reponse etait : " << question.choix()[question.reponse()];
			break;
		}

		cin.get();//Attendre que l'tilisateur appuie sur une touche pour continuer
		system("CLS");
	}

	cin.get();
	return 0;

}