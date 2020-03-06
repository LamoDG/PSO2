#include "ficheros_basico.h"


//muestra la metainformación del sistema de ficheros (SB, mapa de bits, array de inodos)
int main(int argc, char **argv){

	if (argc != 2) {
		printf ("Error en el número de argumentos\n");
	}else{
		int i;
		int file = bmount(argv[1]);
		struct superbloque sb;
		if (bread(0, &sb) == -1){
			printf("Error al leer el superbloque (initMB)");
			return -1;
		}
	
		// visualizamos el superbloque
		printf("\n");
		printf("LOS DATOS DEL SUPERBLOQUE SON:\n");
		printf("Primer bloque del mapa de bits: %d\n", sb.posPrimerBloqueMB);
		printf("Último bloque del mapa de bits: %d\n", sb.posUltimoBloqueMB);
		printf("Primer bloque del array de inodos: %d\n", sb.posPrimerBloqueAI);
		printf("Último bloque del array de inodos: %d\n", sb.posUltimoBloqueAI);
		printf("Primer bloque de datos: %d\n", sb.posPrimerBloqueDatos);
		printf("Último bloque de datos: %d\n", sb.posUltimoBloqueDatos);
		printf("Posición del inodo raíz: %d\n", sb.posInodoRaiz);
		printf("Posición del primer inodo libre: %d\n", sb.posPrimerInodoLibre);
		printf("Número de bloques libres: %d\n", sb.cantBloquesLibres);
		printf("Número de inodos libres: %d\n", sb.cantInodosLibres);
		printf("Número de bloques: %d\n", sb.totBloques);
		printf("Número de inodos: %d\n", sb.totInodos);
		bumount();
	}	
}
