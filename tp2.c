#include "tp2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "heap.h"
#include "abb.h"
#include "pila.h"
#include "strutil.h"

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

int comparar_abb(const char *a, const char *b){
  return strcmp(a,b);
  /*  if(strcmp(a->fecha,b->fecha) == 0){
        return a->num > b->num;//return strcmp(a->num,b->num);
    }
    return strcmp(a->fecha,b->fecha);*7
/*
    return strcmp(a->fecha,b->fecha) == 0 ? strcmp(a->fecha,b->fecha) : strcmp(a->num,b->num);
*/

}

int comparar_heap((vuelo_t*)const void *a, (vuelo_t*)const void *a){
    if(strcmp(a->prioridad,b->prioridad) == 0){//compara la prioridad, y ordena de menor a mayor
        return strcmp(a->num,b->num) ;
    }
    return strcmp(a->prioridad,b->prioridad);
/*
    return strcmp(a->fecha,b->fecha) == 0 ? strcmp(a->fecha,b->fecha) : strcmp(a->num,b->num);
*/
}




bool agregar_archivo(char *archivo,hash_t *hash, abb_t *abb){
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
       /* char *clave_abb = calloc(strlen(datos[fecha])+strlen(datos[num])+1,sizeof(char))
        strcpy(clave_abb,datos[fecha]);
        strcat(clave_abb,datos[num]);*/
        char *claves[2];
        char clave[0] = datos[fecha];
        char clave[1] = datos[num];
        char *clave_abb = join(clave,'');
        if(!hash_guardar(hash,datos[num],vuelo) || !abb_guardar(abb,clave_abb,vuelo) ){
            fprintf(stderr,"ERROR de memoria\n");
            return false;
        };	
        //free(clave_abb);
        leidos = getline(&linea, &capacidad, arch);	
    }
    free(linea);
    return true;
}

bool ver_tablero(char **operacion, abb_t *abb{
    long k = strtol(operacion[1],NULL,10);
    if(k <= 0)return false;
    if(strcmp(operacion[2],"asc") != 0 && strcmp(operacion[2],"desc") != 0)return false;
    if(strcmp(operacion[3],operacion[4]) > 0 )return false;
    if(strcmp(operacion[2],"asc") == 0){
        abb_iter_tp2_t *iter = abb_iter_tp2_crear()tablero ascendente;
    }else{
        tablero descendente;
    }
    pila_t *pila = pila_crear();
    if(!pila){
        fprintf(stderr,"ERROR de memoria\n");
        return;
    }

    if(strcmp(operacion[2],"asc") == 0){
        nodo_t *mayor = abb_obtener(abb,operacion[4]);
        pila_apilar(pila,mayor);
        for(long i = 0; i < strtol(operacion[1],NULL,10)-1; i++){
            pila_apilar(pila,mayor->)
        }
    }
}

bool info_vuelos(char **operacion, hash_t *hash){
    vuelo_t *vuelo = (vuelo_t*)hash_obtener(hash,operacion[1]));
    if(!vuelo)return false;

    fprintf(stdout,"%s %s %s %s %s %s %s %s\n OK\n", vuelo->num, vuelo->aerolinea, vuelo->origen, vuelo->dest, vuelo->cola, vuelo->prioridad, vuelo->fecha, vuelo-demora);
    return true;
}

bool prioridad_vuelos(abb_t *abb){
    
    heap_t *heap = heap_crear(comparar_heap);//PARA VER PRIORIDADES
    if(!heap){
        /*hash_destruir(hash);
        abb_destruir(abb);
        fprintf(stderr,"ERROR de memoria\n");*/
        return false;
    }

    abb_iter_t *iter = abb_iter_in_crear(abb);
    if(!iter){
        heap_destruir(heap,NULL);
        return false;
    }

    long k = strtol(operacion[1],NULL,10);
    
    for(size_t i = 0; i<k; i++){
        heap_encolar(heap,abb_iter_in_ver_nodo_actual(iter));
        abb_iter_in_avanzar(iter);
    }

    while(!abb_iter_in_al_final(iter)){
        if(comparar_heap(abb_iter_in_ver_nodo_actual(iter),heap_ver_max(heap)) > 0){
            heap_desencolar(heap);
            heap_encolar(heap,abb_iter_in_ver_nodo_actual(iter));
            abb_iter_in_avanzar(iter);
        }
    }

    while(!heap_esta_vacio(heap)){
        vuelo_t *desencolado = (vuelo_t*)heap_desencolar(heap);
        fprintf(stdout,"%s - %s\n",desencolado->prioridad,desencolado->num);
    }
    fprintf(stodut,"OK\n");
    heap_destruir(heap,NULL);
    abb_iter_in_destruir(iter);
    return true;
}

void borrar(){

}


void error(char **operacion,pila_t *pila){
	fprintf(stdout,"Error en comando %s\n",operacion[0]);
	free_strv(operacion);
	//pila_vaciar(pila);
	//pila_destruir(pila);
}

/*OPERAR REALIZA CADA OPERACION EN PARTICULAR Y APILA EL RESULTADO A LA PILA
 O DEVUELVE FALSE SI HUBO UN ERROR EN LA MISMA*/
bool operar(op_t op, char **operacion, hash_t *hash, abb_t *abb){
	
	switch(op){
		case op_agregar :
            if(!agregar_archivo(operacion[1],hash,abb))return false;
			break;
		case op_tablero :
            if(!ver_tablero(operacion,abb))return false;
			break;
		case op_info :
            if(!info_vuelo(operacion,hash))return false;
			break;
		case op_prioridad :
            prioridad_vuelos(operacion);
			break;
		case op_borrar :
            borrar();
			break;
	}
    fprintf(stdout,"OK\n");
	return true;
}


/*IDENTIFICAR OPERACION REALIZA TODAS LAS CUENTAS NECESARIAS POR LÍNEA PARA DEVOLVER UN ÚLTIMO RESULTADO
DEFINITIVO. LA MISMA LLAMA A CALCULAR PARA CADA VEZ QUE LEE UN OPERANDO. EL MAIN LLAMA A CALCULAR
UNA VEZ PARA CADA LINEA*/
void identificar_operacion(char *linea, hash_t *hash, abb_t *abb, heap_t *heap){

    size_t i = 0;
    /*linea[leidos-1] = '\0';
    while(linea[i] == ' ')i++; SUPONGO QUE ME INGRESAN TODO BIEN SIN ESPACIOS*/	
    char **operacion = split(linea+i,' ');
    //i = 0;
    bool operado = false;
    int n = 0;
    while(n <= op_borrar  && !operado){
        if(strcmp(operacion[0],acciones[n]) == 0){
            if(!operar(n,operacion,vuelos,hash,abb)){
                error(operacion);
                return;
            }
            operado = true;
        }
        n++;
    }
    if(!operado){
        error(operacion);
        return;
        /*char *fin;
        long num = strtol(operacion[i],&fin,10);
        if(operacion[i] == fin){
            error(operacion);
            return;
        }
        long *res = malloc(sizeof(long));*/
        
    }
    i++;		
    while(operacion[i] && strcmp(operacion[i],"") == 0){
        i++;
    }		
    free_strv(operacion);
}




int main(int argc, char *argv[]){

    hash_t *hash = hash_crear(borrar_vuelo);//PARA INFO VUELOS
    if(!hash){
        fprintf(stderr,"ERROR de memoria\n");
        return;
    }


    abb_t *abb = abb_crear(comparar_abb, borrar_vuelo);//PARA VER TABLERO
    if(!abb){
        hash_destruir(hash);
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
            identificar_operacion(linea,hash,abb);	
            leidos = getline(&linea, &capacidad, stdin);	
        }
    }
    free(linea);

    return;
}