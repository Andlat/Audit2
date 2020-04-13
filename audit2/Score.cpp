#include "Score.h"
#include "Question.h"
#include <string.h>
using namespace std;

Score::Score()
{

}

Score::~Score()
{

}

void Score::resultat()
{
	int liste[20] = { 500,1000,2000,3000,5000,7.500,10000,12.500,15000,25000,50000,100000,250000,500000,1000000};
	Question question;
	if (question.reponse())
	{
		static int i = 0;
		gain = liste[i];
		cout << liste[i];
		i++;
		if (i =='\0')
		{
			cout << " Fin du jeu  " << endl;
		
		}
	}
}