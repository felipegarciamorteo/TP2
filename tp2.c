#include "tp2.h"
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "heap.h"
#include "abb.h"

const char *acciones[] = {"agregar_archivo","ver_tablero","info_vuelo","prioridad_vuelos","borrar"}
typedef enum op = {op_agregar,op_tablero,op_info,op_prioridad,op_borrar}op_t;

typedef enum vuelo = {num, aerolinea, origen, dest, cola, prioridad, fecha, demora,tiempo, cancelados}vuelo_t

struct fecha{

}

struct vuelo{
    int num;
    char *aerolinea;
    char *origen;
    char *dest;
    char  *cola;
    int prioridad;
    fecha_t fecha;
    int demora;
}


vuelo_t *vuelo_crear(char **datos){
    vuelo_t *vuelo = malloc(sizeof(vuelo_t));
    if(!vuelo)return NULL;

    vuelo->num = datos[0];
    vuelo->aerolinea = datos[1];
    vuelo->origen = datos[2];
    vuelo->dest = datos[3];
    vuelo->cola = datos[4];
    vuelo->prioridad = atoi(datos[5]);
    vuelo->fecha = datos[6];
    vuelo->demora = atoi(datos[7]);

    return vuelo;
}

void borrar_vuelo(vuelo_t *vuelo){
    free(vuelo);
}

bool agregar_archivo(char *archivo, FILE *vuelos,hash_t *hash, abb_t *abb, heap_t *heap){
    FILE *arch = fopen(archivo,r);
    if(!arch){
        fprintf(stderr,"ERROR al abrir el archivo\n");
        return false;
    }
    
    char* linea = NULL;
    size_t capacidad = 0; 
    ssize_t leidos = getline(&linea, &capacidad, arch);
    
    while(leidos >= 0){
        char **datos = split(linea,',');
        vuelo_t *vuelo = vuelo_crear(datos);
        if(!hash_guardar(hash,datos[num],vuelo) || !abb_guardar(abb,datos[fecha],vuelo) || !heap_encolar(heap,vuelo)){
            fprintf(stderr,"ERROR de memoria\n");
            return false;
        };	
        leidos = getline(&linea, &capacidad, arch);	
    }
    free(linea);
    return true;
}

void ver_tablero(char **operacion, heap_t *heap{

}

void info_vuelos(){

}

void prioridad_vuelos(){

}

void borrar(){

}


/*OPERAR REALIZA CADA OPERACION EN PARTICULAR Y APILA EL RESULTADO A LA PILA
 O DEVUELVE FALSE SI HUBO UN ERROR EN LA MISMA*/
bool operar(op_t op, char **operacion, FILE *vuelos, hash_t *hash, abb_t *abb, heap_t *heap){
	
	switch(op){
		case op_agregar :
            agregar_archivo(operacion[1],vuelos,hash,abb,heap);
			break;
		case op_tablero :
            ver_tablero(operacion,hash);
			break;
		case op_info :
            info_vuelo();
			break;
		case op_prioridad :
            prioridad_vuelos();
			break;
		case op_borrar :
            borrar();
			break;
	}
	return true;
}


/*IDENTIFICAR OPERACION REALIZA TODAS LAS CUENTAS NECESARIAS POR LÍNEA PARA DEVOLVER UN ÚLTIMO RESULTADO
DEFINITIVO. LA MISMA LLAMA A CALCULAR PARA CADA VEZ QUE LEE UN OPERANDO. EL MAIN LLAMA A CALCULAR
UNA VEZ PARA CADA LINEA*/
void identificar_operacion(char *linea, FILE *vuelos, hash_t *hash){

    size_t i = 0;
    /*linea[leidos-1] = '\0';
    while(linea[i] == ' ')i++; SUPONGO QUE ME INGRESAN TODO BIEN SIN ESPACIOS*/	
    char **operacion = split(linea+i,' ');
    //i = 0;
    bool operado = false;
    int n = 0;
    while(n <= op_borrar  && !operado){
        if(strcmp(operacion[0],acciones[n]) == 0){
            if(!operar(n,operacion,vuelos,hash)){
                error(operacion);
                return;
            }
            operado = true;
        }
        n++;
    }
    if(!operado){
        char *fin;
        long num = strtol(operacion[i],&fin,10);
        if(operacion[i] == fin){
            error(operacion);
            return;
        }
        long *res = malloc(sizeof(long));
        
    }
    i++;		
    while(operacion[i] && strcmp(operacion[i],"") == 0){
        i++;
    }		
    free_strv(operacion);
    free(tope);
}




int main(int argc, char *argv[]){
    FILE *vuelos = fopen("algueiza.txt",w);
    if(!vuelos){
        fprintf(stderr,"ERROR al abrir archivo vuelos\n");
        return;
    }

    hash_t *hash = hash_crear(borrar_vuelo);
    if(!hash){
        fclose(vuelos);
        fprintf(stderr,"ERROR de memoria\n");
        return;
    }


    abb_t *abb = abb_crear(comparar, borrar_vuelo);
    if(!abb){
        fclose(vuelos);
        hash_destruir(hash);
        fprintf(stderr,"ERROR de memoria\n");
        return;
    }

    heap_t *heap = heap_crear(comparar_heap),
    if(!heap){
        fclose(vuelos);
        hash_destruir(hash);
        abb_destruir(abb);
        fprintf(stderr,"ERROR de memoria\n");
        return;
    }

    char* linea = NULL;
    size_t capacidad = 0; 
    ssize_t leidos = getline(&linea, &capacidad, stdin);
    
    while(leidos >= 0){	
        if(leidos <= 1){
		    fprintf(stdout,"ERROR, faltan parametros");
	    }else{	
            identificar_operacion(linea,vuelos,hash,abb,heap);	
            leidos = getline(&linea, &capacidad, stdin);	
        }
    }
    free(linea);

    fclose(vuelos);
    return;
}