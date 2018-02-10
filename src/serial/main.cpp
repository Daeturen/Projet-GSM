#include "fonctions.h"


int main()
{
    char etat_sys = '2';
    char res_init_gsm = -1;
    int fd_serie = -3, err_init=0, nbr_sms_recu=0;
    string bufferSMS("");
    string bufferSMSenvoi("");
    ofstream file_smsrecept;
    ifstream file_smsenvoi;
    
    
    ////TEMP////
    ifstream actRead;
	ofstream actWrite;
	char stateSys,actFound;
	string ligne, action1, numTel,repond1;


    initOutput(); //INIT WIRINGPI
    // Initialisation port serie
    writeLog("Initialisation port serie... ", true);
    fd_serie = serialOpen ("/dev/ttyUSB0", 9600);
    if(fd_serie<0)
    {
        err_init++;
        if(!setEtatSys('0'))
        {
            
            writeLog("Erreur init serie.", true);
        }
        else
        {
           
             writeLog("Erreur ouverture etatsys.cfg et erreur init serie", true);
            err_init++;
        }
    }
    else
    {
        cout << "OK.";
    }

    // Initialisation module GSM
    writeLog("Initialisation module GSM... ", true);
    res_init_gsm = initGSM(fd_serie);
    if(res_init_gsm!=0)
    {
        err_init++;
        if(!setEtatSys('0'))
        {
            writeLog("Erreur init module GSM.",true);
        }
        else
        {
            writeLog("Erreur ouverture etatsys.cfg et erreur init module GSM", true);
            err_init++;
        }
    }
    else
    {
        cout << "OK.";
    }
    etat_sys = getEtatSys();
    #ifdef DEBUG
    cout << "\nEtat du systeme :" << etat_sys;
    #endif

    // BOUCLE PRINCIPALE //
    if(err_init==0) // Pas d'erreurs != systeme eteint
    {
        while(1)
        {
            if(etat_sys=='1') //Si le systeme actif
            {

                /// PARTIE LECTURE SMS
                nbr_sms_recu = nbrSmsGsm(fd_serie);
                while(nbr_sms_recu!=0)
                {
					#ifdef DEBUG
                    cout << endl << nbr_sms_recu << " SMS recus.";
                    #endif
                    
                    bufferSMS = readSMS(nbr_sms_recu, fd_serie);

                    ///ECRITURE FICHIER///
                    file_smsrecept.open("cfg/sms.recept", std::ios_base::app);
                    file_smsrecept << bufferSMS << endl;
                    file_smsrecept.close();
                    bufferSMS = "";

                    if(deleteSMS(nbr_sms_recu,fd_serie)==0)
                    {
						actRead.open("cfg/sms.recept");
						 while(getline(actRead, ligne))
						 {

							if(ligne.find(ACTIONVALIDE) != string::npos)
							{
								action1 = ligne;
							}
						 }

						repond1 = actionneuse(action1); //on effectue l'action en consequence 

						 actRead.close();
						 actWrite.open("cfg/sms.recept"); 

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
                        nbr_sms_recu -= 1;
                    }
                    else
                    {
                        writeLog("Erreur suppression SMS.",true);
                    }
                }
                


                /// PARTIE ENVOI SMS
                file_smsenvoi.open("cfg/sms.envoi");
                if(!file_empty(file_smsenvoi))
                {
                    getline(file_smsenvoi, bufferSMSenvoi);
                    sendSMS(DEFAULT_PHONE, bufferSMSenvoi, fd_serie);
                    bufferSMSenvoi = "";

                }
                else
                {
					#ifdef DEBUG
                    cout << "\nAucun SMS a envoyer.";
                    #endif
                }
                file_smsenvoi.close();
                file_smsrecept.open("cfg/sms.envoi", ios::trunc);
				file_smsrecept.close();
                
                
                
            }
            else
            {
					#ifdef DEBUG
                cout << "\nSysteme inactif.";
					#endif
            }
            //On reteste si le systeme est actif
            etat_sys = getEtatSys();
            cout << "\nEtat du systeme :" << etat_sys;
            delay(5000);
        }
    }
    else //Erreurs d'initialisation
    {
        writeLog(err_init + " erreurs a l'initialisation.", true);
    }
    return 0;
}
