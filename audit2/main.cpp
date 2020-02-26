#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>

#include "tinyxml2.h"
#include "Question.h"
#include "QuestionsParser.h"

using namespace std;
using namespace tinyxml2;

#define NB_TOURS 15

vector<Question> questions(NB_TOURS);

bool SelectionQuestions(QuestionsParser* parser);

int main()
{
	/*************************************************************
	 ***************** CHARGEMENT DU JEUX ************************
	 *************************************************************/
	srand(int(time(NULL)));

	QuestionsParser* xmlQuestions = new QuestionsParser("questions_niveaux.xml");

	//chargement du fichier XML de questions en mémoire
	try{
		xmlQuestions->load();
	}catch (XMLError &err) {
		cout << XMLDocument::ErrorIDToName(err) << endl;
		return -1;
	}

	//Choix aleatoire des questions
	if (!SelectionQuestions(xmlQuestions))
		return -2;

	delete xmlQuestions;	xmlQuestions = nullptr;


	/*************************************************************
	 ********************** BOUCLE DE JEUX ***********************
	 *************************************************************/

	for(unsigned tour=1; tour <= (NB_TOURS & questions.size()); ++tour){//Le maximum est le plus petit des 2. Soit NB_TOURS ou la taille du vecteurs de questions
		cout << "***********************************************************\n"
			<< "************ WHO WANTS TO BE A MILLIONAIRE ??? ************\n"
			<< "***********************************************************\n"
			<< endl;

		Question* q = &questions[tour - 1];

		q->randomize();//change l'ordre des choix de manière aléatoire
		cout << *q;

		char reponse = cin.get();
		cin.get();//Ignorer la touche entree


		if (q->validate(reponse)) {
			cout << "BONNE REPONSE !!" << endl;
		}
		else {
			cout << "Mauvaise reponse... :(\nVous avez perdu\n" << endl;
			cout << "La bonne reponse etait : " << q->choix()[q->reponse()];
			break;
		}

		cin.get();//Attendre que l'tilisateur appuie sur une touche pour continuer
		system("CLS");
	}

	cin.get();
	return 0;

}

//TODO Choisir selon le degré de difficulté
//Choix aleatoire des questions
bool SelectionQuestions(QuestionsParser* parser) {
	vector<int> used(NB_TOURS);
	fill(used.begin(), used.end(), -1);

	unsigned index=0;

	for (unsigned i = 0; i < NB_TOURS; ++i) {
		//Selectionner un index qui n'a pas encore ete utilise
		do {
			index = rand() % parser->count();
		} while (find(used.begin(), used.end(), index) != used.end());
		used.push_back(index);
		
		//lecture de la question selectionne dans le XML
		try {
			parser->read(questions[i], index);
		}
		catch (XMLError &err) {
			cout << XMLDocument::ErrorIDToName(err) << endl;
			return false;
		}
	}
	return true;
}