#include "bloques.h"

static int descriptor = 0;

// montamos el fichero que usaremos como dispositivo virtual
int bmount(const char *camino)
{
    // abrimos el fichero con todos los permisos
    descriptor = open(camino,O_RDWR|O_CREAT,0666);
    if(descriptor == -1){
        perror("ERROR 0001 BMOUNT ");
        return -1;
    }
    printf("Montamos el fichero\n");
    return descriptor;
}

int bumount()
{
    if(close(descriptor)==-1){
        perror("ERROR 0002 BUMOUNT ");
        return -1;
    }    
    printf("Desmontamos el fichero\n");
    return 0;
}

int bwrite(unsigned int nbloque, const void *buf)
{
    int desplazamiento = nbloque * BLOCKSIZE;
    // off_t lseek(descriptor, desplazamiento, SEEK_SET);
    lseek(descriptor, desplazamiento, SEEK_SET);
    int bytesEscritos = write(descriptor, buf, BLOCKSIZE);
    if (bytesEscritos != BLOCKSIZE){
	    perror("ERROR 0003 BWRITE");
        return -1;
    }
    // se ha escrito correctamente
    // printf("Hemos escrito %i bytes", bytesEscritos);
    return bytesEscritos; 
}

int bread(unsigned int nbloque, void *buf)
{
    int desplazamiento = nbloque * BLOCKSIZE;
    // off_t lseek(descriptor, desplazamiento, SEEK_SET);
    lseek(descriptor, desplazamiento, SEEK_SET);
    int bytesLeidos = read(descriptor, buf, BLOCKSIZE);
    if (bytesLeidos < 0){
	    perror("ERROR 0004 BREAD");
        return -1;
    }
    // se ha leido correctamente
    // printf("Hemos leido %i bytes", bytesLeidos);
    return bytesLeidos; 
}