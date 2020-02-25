#include "Question.h"

using namespace std;

Question::Question()
{
}

Question::Question(string q, string* choix/*, string indice*/, int r)
	: _question(q)/*, _indice(indice)*/, _reponse(r)
{
	for (ushort i = 0; i < 4; ++i) {
		_choix[i] = choix[i];
	}
}


Question::~Question()
{
}

bool Question::validate(char reponse)
{
	//Convertir la reponse en lettre majuscule
	if (reponse >= 97)//a = 97, A=65 en ASCII
		reponse -= 32;

	//ramener de 0-3
	reponse -= 65;

	return reponse == _reponse;
}


void Question::randomize()
{
	ushort i[2];

	for (ushort j = 0; j < 6; j++)
	{
		i[0] = ushort(rand() % 4);
		i[1] = ushort(rand() % 4);
		while (i[0] == i[1])
			i[1] = ushort(rand() % 3);
		
		if (_reponse == i[0])
			_reponse = i[1];
		else if (_reponse == i[1])
			_reponse = i[0];
		swap(_choix[i[0]], _choix[i[1]]);
	}
}

string Question::get()
{
	return _question;
}

string * Question::choix()
{
	return _choix;
}

ushort Question::reponse()
{
	return _reponse;
}

/*
string Question::indice()
{
	return _indice;
}
*/

ostream& operator << (ostream& s, const Question & q)
{
	s << q._question << endl;

	for (ushort i = 0; i < 4; i++)
		s << char(i + 65) << ". " << q._choix[i] << endl;

	s << endl;

	return s;
}