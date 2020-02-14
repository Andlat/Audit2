#pragma once
#include <string>
#include <fstream>

typedef unsigned short ushort;

class Question
{
public:
	Question();
	Question(std::string q, std::string* choix, std::string indice, char r);
	~Question();

	void lire(std::ifstream& qns);

	std::string get();
	std::string* choix();//Toujours 4 options
	char reponse();
	std::string indice();

private:
	std::string _question, _indice, _choix[4];
	char _reponse;
};

