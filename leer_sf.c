#include "ficheros_basico.h"


//muestra la metainformación del sistema de ficheros (SB, mapa de bits, array de inodos)
int main(int argc, char **argv){

	if (argc != 2) {
		printf ("Error en el número de argumentos\n");
	}else{
		int i;
		int file = bmount(argv[1]);
		struct superbloque SB;
		if (bread(0, &SB) == -1){
			printf("Error al leer el superbloque (initMB)");
			return -1;
		}
	
		// visualizamos el superbloque
		printf("\n");
		printf("DATOS DEL SUPERBLOQUE:\n");
		printf("posPrimerBloqueMB: %d\n", SB.posPrimerBloqueMB);
		printf("Último bloque del mapa de bits: %d\n", SB.posUltimoBloqueMB);
		printf("Primer bloque del array de inodos: %d\n", SB.posPrimerBloqueAI);
		printf("Último bloque del array de inodos: %d\n", SB.posUltimoBloqueAI);
		printf("Primer bloque de datos: %d\n", SB.posPrimerBloqueDatos);
		printf("Último bloque de datos: %d\n", SB.posUltimoBloqueDatos);
		printf("Posición del inodo raíz: %d\n", SB.posInodoRaiz);
		printf("Posición del primer inodo libre: %d\n", SB.posPrimerInodoLibre);
		printf("Número de bloques libres: %d\n", SB.cantBloquesLibres);
		printf("Número de inodos libres: %d\n", SB.cantInodosLibres);
		printf("Número de bloques: %d\n", SB.totBloques);
		printf("Número de inodos: %d\n", SB.totInodos);
		bumount();
	}	
}
