#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>

#include "tinyxml2.h"
#include "Question.h"
#include "QuestionsParser.h"
#include "Util.h"

using namespace std;
using namespace tinyxml2;

#define FICHIER_QUESTIONS "questions.xml"

#define FACILE 5
#define MOYEN 5
#define DIFFICILE 3
#define TRES_DIFFICILE 2

constexpr unsigned NB_TOURS = FACILE + MOYEN + DIFFICILE + TRES_DIFFICILE;

#define INCREMENTATION_MONTANT 1000

bool SelectionQuestions(QuestionsParser* parser);
Question::Level SelectDiff(unsigned questionNo);

vector<Question> questions(NB_TOURS);
unsigned montant = 0;


string AfficherDiff(Question::Level diff) {
	switch (diff) {
	case Question::Level::EASY: return "FACILE";
	case Question::Level::MEDIUM: return "MOYEN";
	case Question::Level::HARD: return "DIFFICILE";
	case Question::Level::VERYHARD: return "TRES DIFFICILE";
	}
}
int main()
{
	/*************************************************************
	 ***************** CHARGEMENT DU JEUX ************************
	 *************************************************************/
	srand(int(time(NULL)));

	QuestionsParser* xmlQuestions = new QuestionsParser(FICHIER_QUESTIONS);

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

		cout << "Difficulte: " << AfficherDiff(q->getLvl()) << endl << endl;
		cout << "Montant actuel: " << montant << '$' << endl;
		cout << "Question pour " << INCREMENTATION_MONTANT << '$' << endl << endl;


		q->randomize();//change l'ordre des choix de manière aléatoire
		cout << *q;

		char reponse = cin.get();
		cin.get();//Ignorer la touche entree


		if (q->validate(reponse)) {
			montant += INCREMENTATION_MONTANT;

			cout << "BONNE REPONSE !!" << endl;
			cout << "Votre montant jusqu'a maintenant est de " << montant << '$' << endl;
			cout << "\nVoulez-vous continuer ? (O/N) ";

			char continuer = cin.get();
			
			if (util::toUpperCase(continuer) != 'O' && util::toUpperCase(continuer) != 10) {//Si selectionne O ou entrer
				break;//Quit game
			}
		}
		else {
			cout << "Mauvaise reponse... :(\nVous avez perdu\n" << endl;
			cout << "La bonne reponse etait : " << q->choix()[q->reponse()];
			break;//Lost game
		}

		cin.get();//Attendre que l'utilisateur appuie sur une touche pour continuer
		system("CLS");
	}

	cout << "\n\nBYE BYE !\nVOTRE MONTANT FINAL EST DE " << montant << '$' << endl;

	cin.get();
	return 0;
}

//Choix aleatoire des questions
bool SelectionQuestions(QuestionsParser* parser) {
	vector<int> used(NB_TOURS);
	fill(used.begin(), used.end(), -1);

	unsigned index = 0;
	Question::Level diff;

	for (unsigned i = 0; i < NB_TOURS; ++i) {

		//Select difficulty
		diff = SelectDiff(i + 1);

		try {
			//Selectionner un index qui n'a pas encore ete utilise
			do {
				index = rand() % parser->count(diff);
				index += parser->getOffset(diff);//Add the offset created by the previous difficulties
			} while (find(used.begin(), used.end(), index) != used.end());
			used.push_back(index);

			//lecture de la question selectionne dans le XML
			parser->read(questions[i], index);
		}
		catch (XMLError &err) {
			cout << XMLDocument::ErrorIDToName(err) << "\n Index: " << index << endl;
			return false;
		}
	}
	return true;
}

Question::Level SelectDiff(unsigned questionNo) {
	Question::Level diff;

	if (questionNo <= FACILE) diff = Question::Level::EASY;
	else if (questionNo <= MOYEN+FACILE) diff = Question::Level::MEDIUM;
	else if (questionNo <= DIFFICILE+MOYEN+FACILE) diff = Question::Level::HARD;
	else diff = Question::Level::VERYHARD;

	return diff;
}