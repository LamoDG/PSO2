#include "bloques.h"



int main (int argc, char **argv){
    //argv[0]=mi_mkfs
    //argv[1] = nombre del fichero
    //argv[2]= cantidad de bloques

    if(argc!=3)
    {//Gestión de errores
        fprintf(stderr,"SINTAXIS INCORRECTA: './mi_mkfs <nombre_fichero> <cantidad_bloques>'\n");
    }else
    {
        int cantidadBloques = atoi(argv[2]);
        bmount(argv[1]);//montamos el fichero que usaremos como dispositivo

        int  buffer[BLOCKSIZE];

           
        memset(buf,0,BLOCKSIZE);//Llenamos el buffer declarado anteriormente de 0

        for( cBloquesAux = 0 ; cBloquesAux<cantidadBloques; cBloquesAux++) //Mientras queden bloques por escribir...
        {
            bwrite(cBloquesAux,buffer);//Escribimos el buffer en el fichero            
        }

        bumount();
    }

}