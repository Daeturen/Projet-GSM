using namespace std;

int main(int argc, char const *argv[])
{
	ifstream actRead;
	ofstream actWrite;


	char stateSys,actFound;
	string ligne, action1, numTel,repond1;

	
	while(1)
	{

			do
			{
				stateSys = GetEtatSys();
			}
			while(stateSys != '1');

			actRead.open("cfg/sms.recept");


     	 while(getline(actRead, ligne))
     	 {

     	 	if(ligne.find(ACTIONVALIDE) != string::npos)
     	 	{
     	 		action1 = ligne;
     	 	}
     	 }

        repond1 = actionneuse(action1);

     	 actRead.close();
		 actWrite.open("cfg/sms.recept");  //clear du fichier action.act

		 actWrite.close();

		 actWrite.open("cfg/sms.envoi");
		if (actWrite)
		{
			actWrite << "Action Effectuee " << repond1 << endl;
		}
		else
		{
			cout << "ERREUR Ouverture sms.envoi";
		}
		actWrite.close();

     	 delay(15000);

	}


	return 0;
}
