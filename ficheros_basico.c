#include "ficheros_basico.h"

// Estructura interna del sistema de ficheros (Super-bloque, mapa de bits, array de inodos y datos)





//Funcion que calcula el tamaño del mapa de bits en bloques
int tamMB(unsigned int nbloques){	

    if ((nbloques/8)%BLOCKSIZE == 0){

        return ((nbloques/8)/BLOCKSIZE);

    }else{

        return ((nbloques/8)/BLOCKSIZE + 1);

    }   
}

// Funcion para devolver el tamaño del array de inodos en bloques
int tamAI(unsigned int ninodos){

    int T_INODO = sizeof(struct inodo);		

    if (ninodos%(BLOCKSIZE/T_INODO) == 0){

        return (ninodos/(BLOCKSIZE/T_INODO));

    }else{

        return (ninodos/(BLOCKSIZE/T_INODO) + 1);

    }    
}

// Funcion que permite rellenar los datos basicos del superbloque
int initSB(unsigned int nbloques, unsigned int ninodos){
    int tam_MB = tamMB(nbloques);
    int tam_AI = tamAI(ninodos);
    struct superbloque SB;
    
    SB.posPrimerBloqueMB=1;		 
    SB.posUltimoBloqueMB=tam_MB;		
    SB.posPrimerBloqueAI=tam_MB + 1;	
    SB.posUltimoBloqueAI=tam_MB+tam_AI;	
    SB.posPrimerBloqueDatos=SB.posUltimoBloqueAI+1;		
    SB.posUltimoBloqueDatos=nbloques-1;		
    SB.posInodoRaiz=0;		
    SB.posPrimerInodoLibre=0;
    SB.cantBloquesLibres=nbloques-tam_AI-tam_MB-1;		
    SB.cantInodosLibres=ninodos;		
    SB.totBloques=nbloques;		
    SB.totInodos=ninodos;

    // Guardamos la informacion del superbloque en el sistema de ficheros
    if (bwrite(0,&SB) ==-1)return -1; 
    return 0;
}
