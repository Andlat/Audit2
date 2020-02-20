#include "Question.h"

using namespace std;
using namespace tinyxml2;

Question::Question()
{
}

Question::Question(string q, string* choix, string indice, int r)
	: _question(q), _indice(indice), _reponse(r)
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

string Question::indice()
{
	return _indice;
}


ostream& operator << (ostream& s, const Question & q)
{
	s << q._question << endl;

	for (ushort i = 0; i < 4; i++)
		s << char(i + 65) << ". " << q._choix[i] << endl;

	s << endl;

	return s;
}


//---------------------------------------------------------------------------
//                         classe questions
//---------------------------------------------------------------------------



Questions::Questions()
{
	_currentChild = nullptr;
	_root = nullptr;
}

Questions::Questions(std::string path) : _path(path)
{

}

Questions::~Questions()
{

}

void Questions::setPath(string path)
{
	_path = path;
}

string Questions::getPath()
{
	return _path;
}

XMLError Questions::load()
{
	return load(_path);
}

XMLError Questions::load(string path)
{
	_path = path;

	FILE * doc;
	
	int err = fopen_s(&doc, _path.c_str(), "rb");

	if (err != 0)
		return XMLError::XML_ERROR_FILE_NOT_FOUND;

	if (doc == nullptr)
		return XMLError::XML_ERROR_FILE_NOT_FOUND;


	XMLError error = _doc.LoadFile(doc);

	fclose(doc);

	XMLCheckResult(error);

	_root = _doc.FirstChild();

	XMLCheckPointer(_root);

	_currentChild = _root->FirstChildElement("question");

	XMLCheckPointer(_currentChild);
	
	return error;
}

XMLError Questions::read(Question &q)
{
	if (_currentChild == nullptr)
	{
		return XML_NO_TEXT_NODE;
	}

	string question, indice;
	string choix[4];

	XMLCheckPointer(_currentChild)

	XMLElement* statement, * answer, * hint;


	statement = _currentChild->FirstChildElement("statement");

	XMLCheckPointer(statement);

	question = statement->GetText();


	hint = _currentChild->FirstChildElement("hint");

	XMLCheckPointer(hint);

	indice = hint->GetText();


	answer = _currentChild->FirstChildElement("answers");

	XMLCheckPointer(answer);
		
	answer = answer->FirstChildElement("correct");

	XMLCheckPointer(answer);

	choix[0] = answer->GetText();

	for (ushort i = 1; i < 4; i++)
	{
		answer = answer->NextSiblingElement("wrong");
		
		XMLCheckPointer(answer);

		choix[i] = answer->GetText();
		
	}

	q = Question(question, choix, indice, 0);

	_currentChild = _currentChild->NextSiblingElement("question");

	return XMLError::XML_SUCCESS;
	
}


