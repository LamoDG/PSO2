#include "bloques.h"

int main (int argc, char **argv){
    // argv[0]   =  mi_mkfs
    // argv[1]   =  nombre del fichero
    // argv[2]   =  nbloques

    int cantidad_bloques, nbloques, ninodos;

    if(argc!=3)
    { 
        //Gestión de errores
        fprintf(stderr,"SINTAXIS INCORRECTA: './mi_mkfs <nombre_fichero> <cantidad_bloques>'\n");
    }
    else
    {
        nbloques = atoi(argv[2]);
        // montamos el fichero que usaremos como dispositivo
        bmount(argv[1]);
        unsigned char buffer[BLOCKSIZE];     
        // inicializamos el buffer a 0  
        memset(buffer,0,BLOCKSIZE);
        // mientras queden bloques por escribir...
        for(cantidad_bloques = 0 ; cantidad_bloques<nbloques; cantidad_bloques++) 
        {
            // escribimos el buffer en el fichero
            bwrite(cantidad_bloques,buffer);            
        }
        // indica la máxima cantidad de directorios y ficheros que pueden llegar a existir
        ninodos = nbloques / 4;
		// inicializacion del SB
		initSB(nbloques, ninodos); 
		// inicializacion del MB
		initMB(); 
		// inicializacion del array de inodos
		initAI(); 
        bumount();
    }
}