#pragma once
#include <string>
#include <random>

typedef unsigned short ushort;

class Question
{
public:
	Question();
	Question(std::string q, std::string* choix/*, std::string indice*/, int r);
	~Question();

	bool validate(char ans);

	std::string get();
	std::string* choix();//Toujours 4 options
	ushort reponse();
	//std::string indice();
	void randomize();

	friend std::ostream & operator << (std::ostream& s, const Question& q);
private:
	std::string _question/*, _indice*/, _choix[4];
	ushort _reponse;
};
