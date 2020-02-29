
#include <time.h>
#include "bloques.h"

#define INODOSIZE 128

struct superbloque{
unsigned int posPrimerBloqueMB;	//Posición del primer bloque del mapa de bits
unsigned int posUltimoBloqueMB;	//Posición del último bloque del mapa de bits
unsigned int posPrimerBloqueAI; //Posición del primer bloque del array de inodos
unsigned int posUltimoBloqueAI;	 //Posición del último bloque del array de inodos
unsigned int posPrimerBloquesDatos; //Posición del primer bloque de datos	
unsigned int posUltimoBloquesDatos; //Posición del último bloque de datos		
unsigned int posInodoRaiz;	//Posición del inodo del directorio raíz	
unsigned int posPrimerInodoLibre; //Posición del primer inodo libre	
unsigned int cantBloquesLibres;	//Cantidad de bloques libres
unsigned int cantInodosLibres;	//Cantidad de inodos libres	
unsigned int totBloques; //Cantidad total de bloques
unsigned int totInodos;	//Cantidad total de inodos
char padding [BLOCKSIZE - 12 * sizeof(unsigned int)]; //Relleno
};