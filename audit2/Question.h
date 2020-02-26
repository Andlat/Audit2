#pragma once
#include <string>
#include <random>
#include <unordered_map>

typedef unsigned short ushort;

class Question
{
public:
	enum class Level {EASY, MEDIUM, HARD, VERYHARD};
	static const std::unordered_map<std::string, Level> getLevel;

	Question();
	Question(std::string q, std::string* choix/*, std::string indice*/, int r, Level lvl);
	~Question();

	bool validate(char ans);

	std::string get();
	std::string* choix();//Toujours 4 options
	ushort reponse();
	//std::string indice();
	Level getLvl();

	void randomize();

	friend std::ostream & operator << (std::ostream& s, const Question& q);
private:
	std::string _question/*, _indice*/, _choix[4];
	ushort _reponse;
	Level _lvl;
};
