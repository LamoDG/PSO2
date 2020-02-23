#include "bloques.h"


static int descriptor = 0;

int bmount(const char *camino)
{
    //Abrimos el fichero con todos los permisos
    descriptor = open(camino,O_RDWR|O_CREAT,0666);
    if(descriptor == -1){
        perror("ERROR 0001 BMOUNT ");
    }
    return descriptor;
}
int bumount()
{
    if(close(descriptor)<0){
        perror("ERROR 0002 BUNMOUNT ");
        return -1;
    }
    return 1;
}
int bwrite(unsigned int nbloque, const void *buf)
{
return 0
}
int bread(unsigned int nbloque, void *buf)
{
return 0
}