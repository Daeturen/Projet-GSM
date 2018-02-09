#ifndef FONCTIONS_INC
#define FONCTIONS_INC

#include <iostream>
#include <fstream>
#include <cstdio>
#include <wiringSerial.h>
#include <wiringPi.h>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <locale>
#include <sstream>
#include <ctime>

#define TIMEOUT 200000
#define DEFAULT_PHONE "0768588348"

using namespace std;

unsigned char initGSM(int fd);

char getEtatSys();

int nbrSmsGsm(int fd);
string readSMS(int id, int fd);
bool deleteSMS(int id, int fd);
bool sendSMS(string numero, string texte, int fd);

bool file_empty(ifstream& pFile);
bool writeLog(string message);


string sendSerial(string requete, int fd);
bool setEtatSys(char etat);

#endif
