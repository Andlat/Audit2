#include"Save.h"

using namespace std;

Save::Save()
{

}

Save::~Save()
{

}

void Save::sauvegarde(int x)
{
	ofstream file;
	file.open("score.txt", ios_base::out);
	if (file.is_open())
	{
	
		file << "Bravo, votre score est de: " << x << " $"<< endl;
	}
	file.close();
}