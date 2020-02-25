#include "QuestionsParser.h"

using namespace std;
using namespace tinyxml2;

QuestionsParser::QuestionsParser()
{
	_currentChild = nullptr;
	_root = nullptr;
}

QuestionsParser::QuestionsParser(std::string path) : _path(path)
{
}

QuestionsParser::~QuestionsParser()
{
}

void QuestionsParser::setPath(string path)
{
	_path = path;
}

string QuestionsParser::getPath()
{
	return _path;
}

void QuestionsParser::load() /* throws XMLError */
{
	load(_path);
}

void QuestionsParser::load(string path) /* throws XMLError */
{
	_path = path;

	//Open xml document
	FILE * doc;
	int err = fopen_s(&doc, _path.c_str(), "rb");
	if (err != 0 || doc == nullptr)
		throw XMLError::XML_ERROR_FILE_NOT_FOUND;


	//Parse xml
	XMLError error;
	if ((error = _doc.LoadFile(doc)) != XMLError::XML_SUCCESS)
		throw error;

	fclose(doc);

	//Get root node
	_root = _doc.FirstChildElement("root");
	XMLCheckPointer(_root);

	//Get first question
	_currentChild = _root->FirstChildElement("question");
	XMLCheckPointer(_currentChild);
}

void QuestionsParser::read(Question &q) /* throws XMLError */
{
	if (_currentChild == nullptr)
		throw XML_NO_TEXT_NODE;
	XMLCheckPointer(_currentChild)


	string question, indice;
	string choix[4];

	XMLElement* statement, *answer, *hint;


	//Lire la question
	statement = _currentChild->FirstChildElement("statement");
	XMLCheckPointer(statement);
	question = statement->GetText();

	//Lire l'indice. NOUVEAU FORMAT XML NE SUPPORTE PAS LES INDICES
	/*
	hint = _currentChild->FirstChildElement("hint");
	XMLCheckPointer(hint);
	indice = hint->GetText();
	*/

	//Lire les choix
	answer = _currentChild->FirstChildElement("answers");
	XMLCheckPointer(answer);

	//Lire la reponse
	answer = answer->FirstChildElement("correct");
	XMLCheckPointer(answer);
	choix[0] = answer->GetText();

	//Lire les choix incorrects
	for (ushort i = 1; i < 4; i++){
		answer = answer->NextSiblingElement("wrong");
		XMLCheckPointer(answer);

		choix[i] = answer->GetText();
	}

	q = Question(question, choix, 0);

	this->nextQuestion();
}

void QuestionsParser::nextQuestion() { /* throws XMLError */
	_currentChild = _currentChild->NextSiblingElement("question");
}