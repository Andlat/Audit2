#include "QuestionsParser.h"
#include <iostream>
#include <cmath>

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

	_currentDifficulty = _root->FirstChild();
	XMLCheckPointer(_currentDifficulty);

	//Get first question
	_currentChild = _currentDifficulty->FirstChildElement("question");
	XMLCheckPointer(_currentChild);
}

void QuestionsParser::read(Question &q, unsigned index) {/* throws XMLError */
	int shift = index - (stoi(_currentChild->FindAttribute("id")->Value()) -1);

	if (shift != 0) {
		int sign = shift / abs(shift);
		for (; abs(shift) > 0; shift -= sign) {
			if (sign > 0)
				nextQuestion();
			else
				prevQuestion();
		}
	}
	this->read(q);
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

	//Creer la question
	q = Question(question, choix, 0, Question::getLevel.find(_currentChild->FindAttribute("lvl")->Value())->second);

	//Aller à la prochaine question si ce n'est pas la dernière
	if(stoi(_currentChild->FindAttribute("id")->Value()) != this->countTotal())
		this->nextQuestion();
}

// TODO Verifier si a la derniere de la difficulte
void QuestionsParser::nextQuestion() { /* throws XMLError */
	try {
		_currentChild = _currentChild->NextSiblingElement("question");
		XMLCheckPointer(_currentChild);
	}catch(XMLError err){
		this->nextDifficulty();//Will throw an exception if is at the last question of the last difficulty
		_currentChild = _currentDifficulty->FirstChildElement("question");//Access the next question once the difficulty is changed
	}
}
void QuestionsParser::prevQuestion() { /* throws XMLError */
	try {
		_currentChild = _currentChild->PreviousSiblingElement("question");
		XMLCheckPointer(_currentChild);
	}catch (XMLError err) {
		this->prevDifficulty();//Will throw an exception if is at the first question of the first difficulty
		_currentChild = _currentDifficulty->FirstChildElement("question");
	}
}

void QuestionsParser::nextDifficulty() { /* throws XMLError */
	_currentDifficulty = _currentDifficulty->NextSibling();
	XMLCheckPointer(_currentDifficulty);
}
void QuestionsParser::prevDifficulty() { /* throws XMLError */
	_currentDifficulty = _currentDifficulty->PreviousSibling();
	XMLCheckPointer(_currentDifficulty);
}

unsigned QuestionsParser::count(Question::Level diff) {/* throws XMLError */
	XMLElement* node = _root->FirstChildElement(Question::toString(diff).c_str());
	XMLCheckPointer(node);

	int firstID = stoi(node->FirstChildElement("question")->FindAttribute("id")->Value());
	int lastID = stoi(node->LastChildElement("question")->FindAttribute("id")->Value());
	

	return lastID-firstID+1;
}

unsigned QuestionsParser::countTotal() {/* throws XMLError */
	XMLElement* node = _root->FirstChildElement(Question::toString(Question::Level::VERYHARD).c_str());
	XMLCheckPointer(node);

	return stoi(node->LastChildElement("question")->FindAttribute("id")->Value());
}

unsigned QuestionsParser::getOffset(Question::Level diff)
{
	XMLElement* node = _root->FirstChildElement(Question::toString(diff).c_str());
	XMLCheckPointer(node);

	return stoi(node->LastChildElement("question")->FindAttribute("id")->Value()) - this->count(diff);
}
