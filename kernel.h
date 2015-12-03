#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#define SIZEBLOCK 1024; //Taille d'un bloc
#define MINBLOCK 3; //Nb de block minimal dans le disk
