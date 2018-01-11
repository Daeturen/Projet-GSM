#include <wiringPi.h>
#include <wiringSerial.h>
#include <iostream>
#include <fstream>

using namespace std;

char GetEtatSys();

int main(int argc, char const *argv[])
{
	ofstream actRead;
	ifstream actWrite;


	char stateSys;

	wiringPiSetup();

	while(1)
	{
		do
		{
			stateSys = GetEtatSys();
		}
		while(stateSys != '1');
		cout << "BAnana" << "Oui";
	}

	return 0;
}

char GetEtatSys()
{

		ifstream etat("cfg/etatsys.cfg");
        char etatsys = 0;
        etat.get(etatsys);
        return etatsys;

}