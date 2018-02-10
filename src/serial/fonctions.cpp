#include "fonctions.h"


char getEtatSys()
{
	#ifdef DEBUG
    cout << "\nAppel fonction getEtatSys()";
    #endif
    
	ifstream etat("cfg/etatsys.cfg");
    char etatsys = 0;
    etat.get(etatsys);
    return etatsys;
}

string getPin()
{
	#ifdef DEBUG
	cout << "\nAppel fonction getPin()";
	#endif
	
	ifstream f_pin("cfg/pin.cfg");
    string pin = "";
    getline(f_pin, pin);
    f_pin.close();
    return pin;
}

bool checkPhone(string numero)
{
	bool res = 0;
	string listeNums(getPhones());
	numero.erase(0,3);
	if(listeNums.find(numero) != string::npos)
	{
		res = 1;
	}
	else
	{
		res = 0;
	}
	return res;
}

string getPhones()
{
	ifstream f_nums("cfg/numauto.cfg");
    string nums(static_cast<stringstream const&>(stringstream() << f_nums.rdbuf()).str());
    f_nums.close();
    return nums;
}

bool setEtatSys(char etat)
{
	bool res = true;
	FILE* file_etat_sys = NULL;
	
	file_etat_sys = fopen("cfg/etatsys.cfg", "w+"); //ouverture en ecriture + supp contenu fichier
    if(file_etat_sys!=NULL)
    {
        fputc(etat, file_etat_sys);
        fclose(file_etat_sys);
        res = false;
    }
	return res;
}


int nbrSmsGsm(int fd)
{
	#ifdef DEBUG
    cout << "\nAppel fonction nbrSmsGsm()";
    #endif
    
    string reponse("");
    int nbr_sms = 0;
    reponse = sendSerial("AT+CPMS=\"MT\"", fd);
    ///analyse reponse provisoire
    nbr_sms=atoi(reponse.c_str());
    return nbr_sms;
}

string readSMS(int id, int fd)
{
	#ifdef DEBUG
    cout << "\nAppel fonction readSMS()";
    #endif
    
    string commande("AT+CMGR=");
    string reponse("");        
	commande += static_cast<ostringstream*>( &(ostringstream() << id) )->str();
    reponse = sendSerial(commande, fd);
    return reponse;
}

bool deleteSMS(int id, int fd)
{
	bool result = true;
	
	#ifdef DEBUG
    cout << "\nAppel fonction deleteSMS()";
    #endif
    
    string commande("AT+CMGD=");
    string reponse("");
	commande += static_cast<ostringstream*>( &(ostringstream() << id) )->str();
    reponse = sendSerial(commande, fd);
    if(reponse.find("OK") != string::npos)
    {
        result = false;
    }
    else
    {
        result = true;
    }
    
    return result;
}

bool sendSMS(string numero, string texte,int fd)
{
	bool result = true;
	
	#ifdef DEBUG
    cout << "\nAppel fonction sendSMS()";
    #endif
    
    
    string commande("AT+CMGS=\"");
    string reponse("");
    int cptserial = 0;
    commande += numero + "\"";
    /// ON ENVOIE LE NUMERO DU SMS
    reponse = sendSerial(commande, fd);
    texte += 0x1A; // on rajoute ^Z a la fin (datasheet)
    /// ON ENVOIE LE TEXTE DU SMS
    reponse = sendSerial(texte, fd);
    if(reponse.find("CMGS") != string::npos)
    {
        result = false;
    }
    else
    {
        result = true;
    }
    
    return result;
}

bool file_empty(ifstream& pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}


unsigned char initGSM(int fd)
{
    string reponse("");
    string code_pin = getPin();
    unsigned char res = -1, err=0;

    ///INIT MODEM///
    reponse = sendSerial("AT&F", fd);
    //TEST REPONSE GSM
    if(reponse.find("OK") == string::npos)
    {
        err++;
    }
    
    ///FORMAT NUMERIQUE PR REPONSE MODEM///
    reponse = sendSerial("AT+CMEE=1", fd);
    //TEST REPONSE GSM
    if(reponse.find("OK") == string::npos)
    {
        err++; //erreur
        writeLog("erreur selection FORMAT NUMERIQUE PR REPONSE MODEM", true);
    }

    ///CODE PIN
    reponse = sendSerial("AT+CPIN=" + code_pin, fd);
    //TEST REPONSE GSM
    if(reponse.find("OK") == string::npos)
    {
        err++; //erreur
        writeLog("erreur code PIN", true);
    }

    if(err!=0)
    {
        res = 1;
    }
    else
    {
        res = 0;
    }
    return res;
}

string sendSerial(string requete, int fd)
{
	int cptserial = 0;
	string reponse = "";
	
	serialPuts (fd, requete.c_str());
    serialPutchar(fd, 0x0D); //CR
    serialPutchar(fd, 0x0A); //LF
    while(((serialDataAvail (fd)) <= 0)&&(cptserial<TIMEOUT)) // on attend qu'il y ait des data
    {
		delayMicroseconds(83);
	}
    while (serialDataAvail (fd))
    {
      reponse += serialGetchar(fd);
      delayMicroseconds(83);
    }
    
    if(reponse=="")
    {
		reponse = "Timeout ou pas de reponse.";
	}
    
	return reponse;
}

bool writeLog(string message, bool print)
{
	#ifdef DEBUG
	if(print)
	{
		cout << endl << message;
	}
	#endif
	
	ofstream logfile;
	time_t now = time(0);
	string date = ctime(&now);
	date.erase(date.size() - 1);
    string log = "\n" + date  + " : " + message;
	bool success = true;
	
	logfile.open("cfg/log.txt", std::ios_base::app);
    logfile << log;
    logfile.close();
    success = false;
	
	return success;
}


string actionneuse(string action)
{
    string repond = "";
    if(action.find(ACTIONVALIDE) != string::npos) //Code a ameliorer
     	{
     	    if(action.find(ACTION_SHUTDOWN) != string::npos)
     	    {
     	        if(setEtatSys('0'))
     	        {
     	            repond += "Erreur du shutdown";
     	        }
     	        else
     	        {
     	            repond += "Shutdown";
     	        }
     	    }
     	 	if(action.find(ACTION_1_ON) != string::npos)
     	 	{
     	 		digitalWrite(0 , HIGH);
     	 		repond += ACTION_1_ON;
     	 	}
     	 	if(action.find(ACTION_1_OFF) != string::npos)
     	 	{
     	 		digitalWrite(0 , LOW);
     	 		repond += ACTION_1_OFF;
     	 	}
     	 	if(action.find(ACTION_2_ON) != string::npos)
     	 	{
     	 		digitalWrite(1 , HIGH);
     	 		repond += ACTION_2_ON;
     	 	}
     	 	if(action.find(ACTION_2_OFF) != string::npos)
     	 	{
     	 		digitalWrite(1 , LOW);
     	 		repond += ACTION_2_OFF;
     	 	}
     	 	if(action.find(ACTION_3_ON) != string::npos)
     	 	{
     	 		digitalWrite(2 , HIGH);
     	 		repond += ACTION_3_ON;
     	 	}
     	 	if(action.find(ACTION_3_OFF) != string::npos)
     	 	{
     	 		digitalWrite(2 , LOW);
     	 		repond += ACTION_3_OFF;
     	 	}
     	 	if(action.find(ACTION_4_ON) != string::npos)
     	 	{
     	 		digitalWrite(3 , HIGH);
     	 		repond += ACTION_4_ON;
     	 	}
     	 	if(action.find(ACTION_4_OFF) != string::npos)
     	 	{
     	 		digitalWrite(3 , LOW);
     	 		repond += ACTION_4_OFF;
     	 	}
     	 	if(action.find(ACTION_5_ON) != string::npos)
     	 	{
     	 		digitalWrite(4 , HIGH);
     	 		repond += ACTION_5_ON;
     	 	}
     	 	if(action.find(ACTION_5_OFF) != string::npos)
     	 	{
     	 		digitalWrite(4 , LOW);
     	 		repond += ACTION_5_OFF;
     	 	}
     	 	if(action.find(ACTION_6_ON) != string::npos)
     	 	{
     	 		digitalWrite(5 , HIGH);
     	 		repond += ACTION_6_ON;
     	 	}
     	 	if(action.find(ACTION_6_OFF) != string::npos)
     	 	{
     	 		digitalWrite(5 , LOW);
     	 		repond += ACTION_6_OFF;
     	 	}
     	 	if(action.find(ACTION_7_ON) != string::npos)
     	 	{
     	 		digitalWrite(6 , HIGH);
     	 		repond += ACTION_7_ON;
     	 	}
     	 	if(action.find(ACTION_7_OFF) != string::npos)
     	 	{
     	 		digitalWrite(6 , LOW);
     	 		repond += ACTION_7_OFF;
     	 	}
     	}
		else
		{
			repond = "There is an error (Syntax non respectee)";
		}
		return repond;
}

void initOutput()
{
	wiringPiSetup();
	pinMode (0, OUTPUT);
	pinMode (1, OUTPUT);
	pinMode (2, OUTPUT);
	pinMode (3, OUTPUT);
	pinMode (4, OUTPUT);
	pinMode (5, OUTPUT);
	pinMode (6, OUTPUT);
}
