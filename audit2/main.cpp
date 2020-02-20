#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "tinyxml2.h"
#include <time.h>
#include "Question.h"

using namespace std;
using namespace tinyxml2;

int main()
{
	srand(int(time(NULL)));

	//question est la question et questions est la structure de gestion du XML
	Question question;
	Questions questions("questions.xml");

	//chargement du fichier XML en mémoire
	XMLError load = questions.load();

	cout << XMLDocument::ErrorIDToName(load) << endl;

	//lecture de la première question du XML (question est passé par référence)
	XMLError state = questions.read(question);

	if (state != XML_SUCCESS)
	{
		cout << XMLDocument::ErrorIDToName(state) << endl;
		return -1;
	}

	do {

		cout << "***********************************************************\n"
			<< "************ WHO WANTS TO BE A MILLIONAIRE ??? ************\n"
			<< "***********************************************************\n"
			<< endl;

		//change l'ordre des choix de manière aléatoire
		question.randomize();

		cout << question;

		char reponse = cin.get();
		cin.get();//Ignorer la touche entree


		if (question.validate(reponse)) {
			cout << "BONNE REPONSE !!" << endl;
		}
		else {
			cout << "Mauvaise reponse... :(\nVous avez perdu" << endl;
			cout << "La bonne reponse etait : " << question.choix()[question.reponse()];
			break;
		}

		cin.get();//Attendre que l'tilisateur appuie sur une touche pour continuer
		system("CLS");

		state = questions.read(question);

	} while (state == XML_SUCCESS);
	
	if (state != XML_SUCCESS)
	{
		cout << XMLDocument::ErrorIDToName(state) << endl;
		return -1;
	}

	cin.get();
	return 0;
}