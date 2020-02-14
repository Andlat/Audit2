#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#include "Question.h"

using namespace std;


int main()
{
	ifstream qns("questions.txt");
	if (!qns.is_open())
		return -1;

	Question question;
	while (!qns.eof()) {
		question.lire(qns);

		cout << "***********************************************************\n"
			<< "************ WHO WANTS TO BE A MILLIONAIRE ??? ************\n"
			<< "***********************************************************\n"
			<< endl;

		cout << question.get() << endl << endl;

		for (ushort i = 0; i < 4; ++i) {
			cout << question.choix()[i] << endl;
		}

		char reponse = cin.get();
		cin.get();//Ignorer la touche entree

		//Convertir la reponse en lettre majuscule
		if (reponse >= 97)//a = 97, A=65 en ASCII
			reponse -= 32;

		if (reponse == question.reponse()) {
			cout << "BONNE REPONSE !!" << endl;
		}
		else {
			cout << "Mauvaise reponse... :(\nVous avez perdu" << endl;
			break;
		}

		cin.get();//Attendre que l'tilisateur appuie sur une touche pour continuer
		system("CLS");
	}


	qns.close();

	cin.get();
	return 0;
}