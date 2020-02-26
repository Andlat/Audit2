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
#include "Util.h"

using namespace std;
using namespace tinyxml2;

#define NB_TOURS 15
#define INCREMENTATION_MONTANT 1000

bool SelectionQuestions(QuestionsParser* parser);

vector<Question> questions(NB_TOURS);
unsigned montant = 0;

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
			montant += INCREMENTATION_MONTANT;

			cout << "BONNE REPONSE !!" << endl;
			cout << "Votre montant jusqu'a maintenant est de " << montant << '$' << endl;
			cout << "\nVoulez-vous continuer ? (y/n) ";

			char continuer = cin.get();

			if (util::toUpperCase(continuer) != 'Y') {
				break;//Quit game
			}
		}
		else {
			cout << "Mauvaise reponse... :(\nVous avez perdu\n" << endl;
			cout << "La bonne reponse etait : " << q->choix()[q->reponse()];
			break;//Lost game
		}

		cin.get();//Attendre que l'tilisateur appuie sur une touche pour continuer
		system("CLS");
	}

	cout << "\n\nBYE BYE !\nVOTRE MONTANT FINAL EST DE " << montant << '$' << endl;

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
			index = rand() % parser->count() -1;
		} while (find(used.begin(), used.end(), index) != used.end());
		used.push_back(index);
		
		//lecture de la question selectionne dans le XML
		try {
			parser->read(questions[i], index);
		}
		catch (XMLError &err) {
			cout << XMLDocument::ErrorIDToName(err) << "\n Index: " << index << endl;
			return false;
		}
	}
	return true;
}