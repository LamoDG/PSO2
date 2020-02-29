#include "bloques.h"


static int descriptor = 0;

//montamos el fichero que usaremos como dispositivo virtual
int bmount(const char *camino)
{
    //Abrimos el fichero con todos los permisos
    descriptor = open(camino,O_RDWR|O_CREAT,0666);
    if(descriptor == -1){

        perror("ERROR 0001 BMOUNT ");
        return -1;
    }
    printf("Montamos el fichero");
    return descriptor;
}

int bumount()
{
    if(close(descriptor)<0){
        perror("ERROR 0002 BUNMOUNT ");
        return -1;
    }    
    printf("Desmontamos el fichero");
    return 1;
}

int bwrite(unsigned int nbloque, const void *buf)
{
    int offset = nbloque * BLOCKSIZE;//desplazamieto
    
    if(lseek(descriptor,offset,SEEK_SET)>=0)
    {
        int bytesWritten = write(descriptor,buf,BLOCKSIZE);

        if(bytesWritten>=0){
             printf("Hemos escrito %i bytes", bytesWritten);
            //se ha escrito correctamente
            return bytesWritten;
        }
        perror("ERROR 0004 BWIRTE");

    }else{
        perror("ERROR 0003 BWRITE");
    }

    return EXIT_FAILURE;
}

int bread(unsigned int nbloque, void *buf)
{
    if(lseek(descriptor, nbloque, SEEK_SET)>=0){

        int bytesRead = read(descriptor, buf, BLOCKSIZE);

        if(bytesRead>=0){
            printf("Hemos leido %i bytes", bytesWritten);
            return bytesRead;
        }
        perror("ERROR 0006 BREAD");
        
    }else{
        perror("ERROR 0005 BREAD");
    }
    return EXIT_FAILURE;
}