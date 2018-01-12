#include <wiringPi.h>
#include <wiringSerial.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#define ACTIONVALIDE "act"
#define NUMERO "+33"
#define ACTION_1_ON	"1 ON"
#define ACTION_2_ON	"2 ON"
#define ACTION_3_ON	"3 ON"
#define ACTION_4_ON	"4 ON"
#define ACTION_5_ON	"5 ON"
#define ACTION_6_ON	"6 ON"
#define ACTION_7_ON	"7 ON"
#define ACTION_1_OFF "1 OFF"
#define ACTION_2_OFF "2 OFF"
#define ACTION_3_OFF "3 OFF"
#define ACTION_4_OFF "4 OFF"
#define ACTION_5_OFF "5 OFF"
#define ACTION_6_OFF "6 OFF"
#define ACTION_7_OFF "7 OFF"

using namespace std;

char GetEtatSys();
bool file_empty(ifstream& pFile);

int main(int argc, char const *argv[])
{
	ifstream actRead;
	ofstream actWrite;


	char stateSys,actFound;
	string ligne, action, numTel,repond;

	wiringPiSetup();
	pinMode (0, OUTPUT);
	pinMode (1, OUTPUT);
	pinMode (2, OUTPUT);
	pinMode (3, OUTPUT);
	pinMode (4, OUTPUT);
	pinMode (5, OUTPUT);
	pinMode (6, OUTPUT);
	while(1)
	{
		do
		{
			do
			{
				stateSys = GetEtatSys();
			}
			while(stateSys != '1');
			
			actRead.open("cfg/sms.recept");

			if (!actRead)
			{
	    		cout << "ERREUR Ouverture sms.recept";
			}
		}
		while(file_empty(actRead));

		while(getline(actRead, ligne))
     	 {
     	 	if(ligne.find(NUMERO))
     	 	{
     	 		numTel = ligne;
     	 		break;
     	 	}
     	 }

     	 // TODO : Numero valide ? (trouver comment extraire le numero du string numTel)

     	 while(getline(actRead, ligne))
     	 {
     	 	transform(ligne.begin(), ligne.end(), ligne.begin(), ::tolower); //met ligne en minuscule
     	 	if(ligne.find(ACTIONVALIDE) != string::npos)
     	 	{
     	 		action = ligne;
     	 		break;
     	 	}
     	 }

     	if(ligne.find(ACTIONVALIDE) != string::npos) //Code a ameliorer
     	{
     	 	if(ligne.find(ACTION_1_ON) != string::npos)
     	 	{
     	 		digitalWrite(0 , HIGH);
     	 		repond = ACTION_1_ON;
     	 	}
     	 	if(ligne.find(ACTION_1_OFF) != string::npos)
     	 	{
     	 		digitalWrite(0 , LOW);
     	 		repond = ACTION_1_OFF;
     	 	}
     	 	if(ligne.find(ACTION_2_ON) != string::npos)
     	 	{
     	 		digitalWrite(1 , HIGH);
     	 		repond = ACTION_2_ON;
     	 	}
     	 	if(ligne.find(ACTION_2_OFF) != string::npos)
     	 	{
     	 		digitalWrite(1 , LOW);
     	 		repond = ACTION_2_OFF;
     	 	}
     	 	if(ligne.find(ACTION_3_ON) != string::npos)
     	 	{
     	 		digitalWrite(2 , HIGH);
     	 		repond = ACTION_3_ON;
     	 	}
     	 	if(ligne.find(ACTION_3_OFF) != string::npos)
     	 	{
     	 		digitalWrite(2 , LOW);
     	 		repond = ACTION_3_OFF;
     	 	}
     	 	if(ligne.find(ACTION_4_ON) != string::npos)
     	 	{
     	 		digitalWrite(3 , HIGH);
     	 		repond = ACTION_4_ON;
     	 	}
     	 	if(ligne.find(ACTION_4_OFF) != string::npos)
     	 	{
     	 		digitalWrite(3 , LOW);
     	 		repond = ACTION_4_OFF;
     	 	}
     	 	if(ligne.find(ACTION_5_ON) != string::npos)
     	 	{
     	 		digitalWrite(4 , HIGH);
     	 		repond = ACTION_5_ON;
     	 	}
     	 	if(ligne.find(ACTION_5_OFF) != string::npos)
     	 	{
     	 		digitalWrite(4 , LOW);
     	 		repond = ACTION_5_OFF;
     	 	}
     	 	if(ligne.find(ACTION_6_ON) != string::npos)
     	 	{
     	 		digitalWrite(5 , HIGH);
     	 		repond = ACTION_6_ON;
     	 	}
     	 	if(ligne.find(ACTION_6_OFF) != string::npos)
     	 	{
     	 		digitalWrite(5 , LOW);
     	 		repond = ACTION_6_OFF;
     	 	}
     	 	if(ligne.find(ACTION_7_ON) != string::npos)
     	 	{
     	 		digitalWrite(6 , HIGH);
     	 		repond = ACTION_7_ON;
     	 	}
     	 	if(ligne.find(ACTION_7_OFF) != string::npos)
     	 	{
     	 		digitalWrite(6 , LOW);
     	 		repond = ACTION_7_OFF;
     	 	}
     	}
		else
		{
			repond = "There is an error (Syntax non respéctée)";
		}

     	 actRead.close();
		 actWrite("cfg/sms.recept");  //clear du fichier action.act
		if (!actWrite)
		{
	    	cout << "ERREUR clear sms.recept";
		}
		 actWrite.close();
		 
		 actWrite.open("cfg/sms.envoi");
		if (actWrite)
		{
			actWrite << "Action " << repond << "éfféctuée" << end1;
		}
		else
		{
			cout << "ERREUR Ouverture sms.envoi";
		}
		actWrite.close();

     	 delay(5000);

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

bool file_empty(ifstream& pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}

/*string first_numberstring(string const & str)
{
  size_t const n = str.find_first_of("0123456789");
  if (n != string::npos)
  {
    std::size_t const m = str.find_first_not_of("0123456789", n);
    return str.substr(n, m != string::npos ? m-n : m);
  }
  return std::string();
}*/
