#pragma once
#include <string>
#include <fstream>
#include <random>
#include "tinyxml2.h"
#include <ostream>

#ifndef XMLCheckResult
	#define XMLCheckResult(a_eResult) if (a_eResult != XMLError::XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

#ifndef XMLCheckPointer
	#define XMLCheckPointer(ptr) if (ptr == nullptr) {return XMLError::XML_ERROR_PARSING_ELEMENT;}
#endif


typedef unsigned short ushort;

class Question
{
public:
	Question();
	Question(std::string q, std::string* choix, std::string indice, int r);
	~Question();

	bool validate(char ans);

	std::string get();
	std::string* choix();//Toujours 4 options
	ushort reponse();
	std::string indice();
	void randomize();

	friend std::ostream & operator << (std::ostream& s, const Question& q);
private:
	std::string _question, _indice, _choix[4];
	ushort _reponse;
};

class Questions
{
public:
	Questions();
	Questions(std::string path);
	~Questions();

	void setPath(std::string path);
	std::string getPath();

	tinyxml2::XMLError load();
	tinyxml2::XMLError load(std::string path);

	tinyxml2::XMLError read(Question& q);

private:


	std::string _path;
	tinyxml2::XMLDocument _doc;
	tinyxml2::XMLNode* _root;
	tinyxml2::XMLElement* _currentChild;
	
};