#include "ficheros_basico.h"

// Estructura interna del sistema de ficheros (Super-bloque, mapa de bits, array de inodos y datos)

// Funcion que calcula el tamaño del mapa de bits en bloques
int tamMB(unsigned int nbloques){	
    if ((nbloques/8)%BLOCKSIZE == 0){
        return ((nbloques/8)/BLOCKSIZE);
    }else{
        return ((nbloques/8)/BLOCKSIZE + 1);
    }   
}

// Funcion para devolver el tamaño del array de inodos en bloques
int tamAI(unsigned int ninodos){
    if ((ninodos * INODOSIZE) % BLOCKSIZE == 0){
        return ((ninodos * INODOSIZE) / BLOCKSIZE);
    }else{
        return ((ninodos * INODOSIZE) / BLOCKSIZE) + 1;
    }   
}

// Funcion que permite rellenar los datos basicos del superbloque
int initSB(unsigned int nbloques, unsigned int ninodos){

    struct superbloque SB;
    SB.posPrimerBloqueMB = 1; //posSB = 0, tamSB = 1
    SB.posUltimoBloqueMB = SB.posPrimerBloqueMB + tamMB(nbloques) - 1;
    SB.posPrimerBloqueAI = SB.posUltimoBloqueMB + 1;
    SB.posUltimoBloqueAI = SB.posPrimerBloqueAI + tamAI(ninodos) - 1;
    SB.posPrimerBloqueDatos = SB.posUltimoBloqueAI + 1;
    SB.posUltimoBloqueDatos = nbloques - 1;
    SB.posInodoRaiz = 0;
    SB.posPrimerInodoLibre = 0;
    SB.cantBloquesLibres = nbloques;
    SB.cantInodosLibres = ninodos;
    SB.totBloques = nbloques;
    SB.totInodos = ninodos;
    // Guardamos la informacion del superbloque en el sistema de ficheros
    if (bwrite(0,&SB) == -1)return -1; 
    return 0;
}

// Funcion para definir una zona de memoria con todos los bits a zero
//int initMB(){
    //struct superbloque SB;
    // Leemos primeramente el superbloque
    //if (bread(0,&SB) == -1) return -1;
    //unsigned char buffer [BLOCKSIZE];
    // Nos sirve para asignar un valor a todos los elementos de un array
    //memset(buffer, BLOCKSIZE, 0);	
    // Escribimos en los bloques correspondientes mediante sucesivas llamadas a bwrite
    //for (int i = SB.posPrimerBloqueMB; i < SB.posUltimoBloqueMB; i++){
        //if (bwrite(i, buffer) == -1) return -1;
    //}
    // Tenemos en cuenta los bloques ocupados para el superbloque
    //for (int i = 0;i <= SB.posUltimoBloqueMB; i++){
		//escribir_bit(i,1);
	//}
    //return 0;           
//}

// Funcion para definir una zona de memoria con todos los bits a zero
int initMB(){
    struct superbloque SB;
    // Leemos primeramente el superbloque
    if (bread(0, &SB) == -1){
        perror("Error en initMB -> bread");
        return -1;
    }
    unsigned char buffer [BLOCKSIZE];
    // Nos sirve para asignar un valor a todos los elementos de un array
    memset(buffer, BLOCKSIZE, 0);
    // Escribimos en los bloques correspondientes mediante sucesivas llamadas a bwrite
    for (int i = SB.posPrimerBloqueMB; i < SB.posUltimoBloqueMB; i++){
        if (bwrite(i, buffer) == -1) return -1;
    }
    // Tenemos en cuenta los bloques ocupados para el superbloque
    for (int i = 0; i <= SB.posUltimoBloqueAI; i++){
        if (escribir_bit(i, 1) == -1)
            return -1;
    }
    SB.cantBloquesLibres -= SB.posUltimoBloqueAI + 1;
    return 0;
}

// Funcion que nos permite enlazar todos los inodos entre si
int initAI(){
    struct superbloque SB;
    struct inodo inodos[BLOCKSIZE / INODOSIZE];
    // Leemos el superbloque para localizar array de inodos
    if (bread(0, &SB) == -1){
        perror("Error en initAI -> bread");
        return -1;
    }
    int variable_incremental = SB.posPrimerInodoLibre + 1;
    // Si hemos inicializado SB.posPrimerInodoLibre = 0
    for (int i = SB.posPrimerBloqueAI; i <= SB.posUltimoBloqueAI; i++){
        for (int j = 0; j < BLOCKSIZE / INODOSIZE; j++)
        {
            inodos[j].tipo = 'l'; // Libre
            if (variable_incremental < SB.totInodos)
            {
                // Iniciamos primer elemento de cada inodo con variable_incremental
                inodos[j].punterosDirectos[0] = variable_incremental;
                variable_incremental++;
            }
            else
            { 
                // Ultimo de la lista apunta a un numero muy grande (NULL)
                inodos[j].punterosDirectos[0] = UINT_MAX;
                // Salir del bucle, el último bloque no tiene porqué estar completo
            }
        }
        if (bwrite(i, &inodos) == -1)
            return -1;
    }
    return 0;
}

// Programacion de las funciones básicas de E/S para bits del mapa de bits
// Escribe el valor 0 (libre) o 1 (ocupado) en un determinado bit del mapa de bits
int escribir_bit(unsigned int nbloque, unsigned int bit) {
    struct superbloque SB;	
    // Leemos el superbloque
    if (bread(0, &SB) == -1)return -1;     	
    // Calculamos la posicion del byte, bit y bloque en el mapa de bits en el cual debemos leer
    unsigned int posbyte = nbloque / 8;
    unsigned int posbit = nbloque % 8;
    unsigned int nbloqueMB = (posbyte / BLOCKSIZE); 
    unsigned int nbloqueabs = nbloqueMB + SB.posPrimerBloqueMB;
    posbyte = posbyte % BLOCKSIZE;      
    unsigned char bufferMB [BLOCKSIZE];
    // Leemos el bloque que vamos a modificar
    if (bread(nbloqueabs, bufferMB) == -1) return -1;   
    unsigned char mascara = 128; 
    // Desplazamiento de bits a la derecha
    mascara >>= posbit; 
    // Miramos si tenemos que escribir un 1 o un 0
    if (bit == 1){
	bufferMB[posbyte] |= mascara;  // Operador OR para bits
    }else{
	bufferMB[posbyte] &= ~mascara; // Operadores AND y NOT para bits
    }  
    // Guarda los cambios tanto en el mapa de bits como en el superbloque 
    if (bwrite(nbloqueabs, bufferMB) == -1)return -1;
    return 0;
}

// Lee un determinado bit del mapa de bits al igual que la funcion anterior.
unsigned char leer_bit (unsigned int nbloque){
    struct superbloque SB;
    if (bread(0,&SB) == -1) return -1;
    // Guardamos en posmp el primer bloque correspodiente al mapa de bits
    int posmp = SB.posPrimerBloqueMB; 
    // Calculamos el bloque, byte y bit que está el bloque pasado por parámetro en el mapa de bits
    // Calculamos el número de bloque que contiene el bit
    int n_bloque = nbloque / (BLOCKSIZE * 8) + 1;
    // Calculamos el byte que contiene el bit
    int nbyte = nbloque % (BLOCKSIZE * 8);
    // Calculamos el bit que nos interesa
    int posbit = nbloque % 8;
    // Leemos el bloque correspondiente al bit que buscamos 
    unsigned char bufferMB[BLOCKSIZE];
    if (bread(n_bloque,bufferMB) == -1) return -1;
    unsigned char mascara = 128;
    
    mascara >>= posbit; // Desplazamiento de bits a la derecha
    mascara &= bufferMB[nbyte]; // Operador AND para bits
    mascara >>= (7 - posbit); // Desplazamiento de bits a la derecha
    return mascara;
}

// Funcion que encuentra el primer bloque libre y nos devuelve su posicion ocupandolo con la funcion escribir_bit
int reservar_bloque(){
    struct superbloque SB;
    int i;
    unsigned char bufferAux [BLOCKSIZE];
    unsigned char bufferMB [BLOCKSIZE];
    // Buscamos los bloques libres
    if (bread(0,&SB) == -1) return -1;
    if (SB.cantBloquesLibres == 0) return -1;
    memset(bufferAux, 255, BLOCKSIZE);
    int posBloqueMB = SB.posPrimerBloqueMB;
    // Leemos el primer bloque del mapa de bits
    if (bread(posBloqueMB,bufferMB) == -1) return -1;
    // Bucle para encontrar el primer bloque con un bit libre mediante la funcion memcmp
    while (memcmp(bufferMB, bufferAux, BLOCKSIZE)== 0){
        posBloqueMB++;
        if (bread(posBloqueMB,bufferMB) == -1) return -1;
    }
    int pos_byte = 0; // Posición del byte en el que tenemos el primer bit a 0
    // Bucle para encontrar el byte en el que se encuentra el bit libre ***
    while (bufferMB[pos_byte] == 255){
        pos_byte++;
    }
    // Bucle para encontrar el bit que vale 0
    unsigned char mascara = 128;
    int pos_bit = 0;
    while (bufferMB[pos_byte] & mascara) {
        pos_bit++; 
        mascara >>= 1; // Desplazamiento de la máscara
    }
    // Calculamos el número del bloque reservado
    int nbloque = ((posBloqueMB - SB.posPrimerBloqueMB) * BLOCKSIZE + pos_byte) * 8 + pos_bit;
    escribir_bit(nbloque,1);
    // Reducimos el numero de bloques libres
    SB.cantBloquesLibres--;
    // Actualizamos el superbloque
    if (bwrite(0, &SB) == -1) return -1;
    return nbloque;
}

// Funcion que nos libera un bloque determinado poniendo a 0 en el mapa de bits el bloque correspondiente y actualizamos la cantidad de bloques libres en el superbloque. No limpiamos el bloque en la zona de datos; se queda basura pero se interpreta como espacio libre. Finalmente devolvemos el bloque liberado
int liberar_bloque(unsigned int nbloque){
    struct superbloque SB;
    if (bread(0,&SB) == -1) return -1;
    if (escribir_bit(nbloque, 0) == -1) return - 1;
    SB.cantBloquesLibres++; 
    // Actualizamos el superbloque
    if (bwrite(0, &SB) == -1) return -1;
    return nbloque;
}

// Funcion para escribir el contenido de una variable del tipo INODO en un determinado inodo del array de inodos.
int escribir_inodo(unsigned int ninodo, struct inodo inodo){
    struct superbloque SB;
    // int INODOSIZE = sizeof(struct inodo);
    // Leemos el superbloque
    if (bread(0,&SB) == -1) return -1;
    // Calculo del bloque en que se encuentra el inodo
    int nbloque = SB.posPrimerBloqueAI + ninodo / (BLOCKSIZE / INODOSIZE);
    struct inodo inodos [BLOCKSIZE / INODOSIZE];
    if (bread(nbloque,inodos) == -1) return -1;
    // Escribimos el inodo pasado por parámetro en su posición
    memcpy (&inodos[ninodo % (BLOCKSIZE / INODOSIZE)], &inodo, INODOSIZE);  
    // Actualizacion del bloque de inodos  
    if (bwrite(nbloque,inodos) == -1) return -1;
    return 0;
}

// Funcion que nos lee un determinado inodo del array de inodos para volcarlo en una variable de tipo INODO
int leer_inodo(unsigned int ninodo, struct inodo *inodo){
    struct superbloque SB;
    //int INODOSIZE = sizeof(struct inodo);
    if (bread(0,&SB) == -1) return -1;
    // Calculo del bloque en el cual se encuentra el inodo
    int nbloque = SB.posPrimerBloqueAI + ninodo / (BLOCKSIZE / INODOSIZE);
    struct inodo inodos [BLOCKSIZE / INODOSIZE];
    // Leemos el bloque con los inodos
    if (bread(nbloque,inodos) == -1) return -1;
    // Devolvemos el inodo
    return 0;
}

// Encuentra el primer inodo libre, lo reserva con la funcion escribir inodo y devuelve su numero.
int reservar_inodo(unsigned char tipo, unsigned char permisos){ 
    struct superbloque SB;
    // Buscamos inodos libres
    if (bread(0,&SB) == -1) return -1;
    // Si hay algun inodo libre, reservamos uno
    if (SB.cantInodosLibres > 0){
        struct inodo inodo; 
        if (leer_inodo (SB.posPrimerInodoLibre, &inodo) == -1) return -1;
        // Inicializamos el inodo
        inodo.tipo = tipo;
	    inodo.permisos = permisos;
        inodo.nlinks = 1;
	    inodo.tamEnBytesLog = 0;
        //inodo.timestamp = time(NULL);
        inodo.atime = time(NULL);
	    inodo.mtime = time(NULL);
	    inodo.ctime = time(NULL);
	    inodo.numBloquesOcupados = 0;
        // Inicializamos los punteros directos e indirectos a 0
        for (int i = 0; i < 12; i++){
			inodo.punterosDirectos[i] = 0;
		}
		for (int j = 0; j < 3; j++){
			inodo.punterosIndirectos[j] = 0;
		}
        // Apuntamos al siguiente inodo libre
        SB.posPrimerInodoLibre = inodo.punterosDirectos[0]; 
        // La posicion del inodo a reservar es el primer inodo libre.
        int posInodoReservado = SB.posPrimerInodoLibre;
        // Escribe el inodo reservado
        if (escribir_inodo(posInodoReservado, inodo) == -1) return -1;
	    // Decrementamos el numero de inodos libres
        SB.cantInodosLibres--;
	    // Actualizamos el superbloque
        if (bwrite(0, &SB) == -1) return -1;
        return posInodoReservado;        
    }
    // Si no quedan inodos libres, devolvemos error
    else return -1;
}