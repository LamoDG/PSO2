#include "ficheros.h"
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
		char c;
		// visualizamos los bits
		//for (i = 0; i <= sb.posUltimoBloquesDatos; i++){
		//	if (leer_bit(i) == 1){
		//		printf("1");
		//	}else{
		//		printf("0");
		//	}
		//}

	        struct tm *ts;
	        char atime[80];
	        char mtime[80];
	        char ctime[80];
	        struct inodo inodo;	
	        int in;
		// visualizamos la metainformación de los ficheros
	        for (in=0;in<sb.posPrimerInodoLibre; in++) {
	            inodo = leer_inodo(in);
			if (inodo.tipo == 'f'){
				printf("\n");
				printf("INODO %d\n",in);
				printf("\n");
				ts = localtime(&inodo.atime);
				strftime(atime, sizeof(atime), "%a %Y-%m-%d %H:%M:%S", ts);
	        		ts = localtime(&inodo.mtime);
	        		strftime(mtime, sizeof(mtime), "%a %Y-%m-%d %H:%M:%S", ts);
	        		ts = localtime(&inodo.ctime);
	        		strftime(ctime, sizeof(ctime), "%a %Y-%m-%d %H:%M:%S", ts);
	        		printf("ID: %d SIG: %d ATIME: %s MTIME: %s CTIME: %s\n",in,inodo.punterosIndirectos[0],atime,mtime,ctime);
	        		printf("DATOS:\n");
				printf("Tipo: %c\n", inodo.tipo);
				printf("Permisos: %c\n", inodo.permisos);
				printf("Cantidad de enlaces: %d\n", inodo.nlinks);
				printf("Tamaño del fichero: %d\n", inodo.tamEnBytesLog);
				printf("Número de bloques ocupados: %d\n", inodo.numBloquesOcupados);
	        	}
		}
		// visualizamos el superbloque
		printf("\n");
		printf("LOS DATOS DEL SUPERBLOQUE SON:\n");
		printf("Primer bloque del mapa de bits: %d\n", sb.posPrimerBloqueMB);
		printf("Último bloque del mapa de bits: %d\n", sb.posUltimoBloqueMB);
		printf("Primer bloque del array de inodos: %d\n", sb.posPrimerBloqueAI);
		printf("Último bloque del array de inodos: %d\n", sb.posUltimoBloqueAI);
		printf("Primer bloque de datos: %d\n", sb.posPrimerBloquesDatos);
		printf("Último bloque de datos: %d\n", sb.posUltimoBloquesDatos);
		printf("Posición del inodo raíz: %d\n", sb.posInodoRaiz);
		printf("Posición del primer inodo libre: %d\n", sb.posPrimerInodoLibre);
		printf("Número de bloques libres: %d\n", sb.cantBloquesLibres);
		printf("Número de inodos libres: %d\n", sb.cantInodosLibres);
		printf("Número de bloques: %d\n", sb.totBloques);
		printf("Número de inodos: %d\n", sb.totInodos);
		bumount();
	}	
}
