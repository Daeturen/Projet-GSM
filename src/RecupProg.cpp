#include <iostream>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>


using namespace std;


int main ()
{
  FILE *fichier = NULL;
  string source;
  int fd ;

  if ((fd = serialOpen ("/dev/ttyAMA0", 9600)) < 0)
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return 1 ;
  }

  if (wiringPiSetup () == -1)
  {
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
    return 1 ;
  }
	printf("Nom fichier : ");
	cin >> source;
	source = "SRC/" + source + ".cpp";
	printf("\n Destination : %s",source);
    while ((serialDataAvail (fd)) <= 0){}
    while (serialDataAvail (fd))
    {

	  fichier = fopen(source, "a");
      fputc(serialGetchar (fd),fichier);
      fclose(fichier);
      fflush (stdout);
      delayMicroseconds(83);
	}
  fclose(fichier);
  printf ("\n") ;
  return 0 ;
}
