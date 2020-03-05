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

// Funcion para definir una zona de memoria con todos los bits a zero
int initMB(unsigned int nbloques){
    int i;
    struct superbloque SB;
    if (bread(0,&superb) == -1) return -1;
    unsigned char buffer [BLOCKSIZE];
    memset(buffer, BLOCKSIZE, 0);	// Nos sirve para asignar un valor a todos los elementos de una array
    //Escribimos en los bloques correspondientes mediante sucesivas llamadas a bwrite
    for (i = SB.posPrimerBloqueMB; i < SB.posUltimoBloqueMB; i++){
        if (bwrite(i, buffer) == -1) return -1;
    }
    return 0;           
}

// Funcion que nos permite enlazar todos los inodos entre si
int initAI(){
    struct superbloque SB;
    if (bread(0,&superb) == -1) return -1;
    int i,j,variable_incremental;
    variable_incremental = 1;
    struct inodo inod [BLOCKSIZE/ sizeof(struct inodo)];
    for (i = SB.posPrimerBloqueAI; i <= SB.posUltimoBloqueAI; i++){
		for (j = 0; j < (BLOCKSIZE / sizeof(struct inodo)); j++){
			inod[j].tipo = 'l';
			if (variable_incremental < SB.totInodos) {
				inod[j].punterosDirectos[0] = variable_incremental;
				variable_incremental++;
			}else{
				inod[j].punterosDirectos[0] = 0;
			}
		}
		// Escribimos el vector de inodos
		if (bwrite(i, inod) == -1) return -1;
    }
    if (bwrite(0,&superb)==-1) return -1;
	return 0;
}