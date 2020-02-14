#include "Question.h"

using namespace std;

Question::Question()
{
}

Question::Question(string q, string* choix, string indice, char r)
	: _question(q), _indice(indice), _reponse(r)
{
	for (ushort i = 0; i < 4; ++i) {
		_choix[0] = choix[0];
	}
}


Question::~Question()
{
}

void Question::lire(ifstream& qns)
{
	getline(qns, _question);//Lire la question

	//Lire les choix
	for (ushort i = 0; i < 4; ++i) {
		getline(qns, _choix[i]);
	}

	_reponse = qns.get();//Lire la reponse
	qns.ignore();//Sauter le caractere de nouvelle ligne

	getline(qns, _indice);//Lire l'indice
}

string Question::get()
{
	return _question;
}

string * Question::choix()
{
	return _choix;
}

char Question::reponse()
{
	return _reponse;
}

string Question::indice()
{
	return _indice;
}
