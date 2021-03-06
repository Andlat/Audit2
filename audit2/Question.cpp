#include "Question.h"
#include "Util.h"

using namespace std;

const std::unordered_map<std::string, Question::Level> Question::getLevel = { {"easy", Level::EASY}, {"medium", Level::MEDIUM}, {"hard", Level::HARD}, {"veryhard", Level::VERYHARD} };
const std::string Question::toString(Level lvl) {
	auto it = getLevel.begin();
	while (it != getLevel.end()) {
		if (it->second == lvl)
			return it->first;
		
		++it;
	}
	return "";
}

Question::Question()
{
}

Question::Question(string q, string* choix/*, string indice*/, int r, Level lvl)
	: _question(q)/*, _indice(indice)*/, _reponse(r), _lvl(lvl)
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
	//ramener de 0-3
	reponse = util::toUpperCase(reponse) - 65;

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

Question::Level Question::getLvl() {
	return _lvl;
}

ostream& operator << (ostream& s, const Question & q)
{
	s << q._question << endl;

	for (ushort i = 0; i < 4; i++)
		s << char(i + 65) << ". " << q._choix[i] << endl;

	s << endl;

	return s;
}