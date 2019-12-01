#define _POSIX_C_SOURCE 200809L
#include "tp2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "heap.h"
#include "abb.h"
#include "pila.h"
#include "lista.h"
#include "strutil.h"

const char *acciones[] = {"agregar_archivo","ver_tablero","info_vuelo","prioridad_vuelos","borrar"};
typedef enum {op_agregar,op_tablero,op_info,op_prioridad,op_borrar}op_t;

typedef enum {num,aerolinea,origen,dest,cola,prioridad,fecha,demora,tiempo,cancelados}vuelo_enum_t;


struct vuelo{
    //int num;
    char *num;
    char *aerolinea;
    char *origen;
    char *dest;
    char  *cola;
    //int prioridad;
    char *prioridad;
    char *fecha;
    char *demora;
    char *tiempo;
    char *cancelados;
    //int demora;
};


vuelo_t* vuelo_crear(char **datos){
    vuelo_t *vuelo = malloc(sizeof(vuelo_t));
    if(!vuelo)return NULL;
    //printf("num: %s,aerol: %s,prioridad: %s,fecha: %s\n",datos[0],datos[1],datos[5],datos[6]);
    
    vuelo->num = datos[num];
    vuelo->aerolinea = datos[aerolinea];
    vuelo->origen = datos[origen];
    vuelo->dest = datos[dest];
    vuelo->cola = datos[cola];
    vuelo->prioridad = /*atoi(*/datos[prioridad];
    vuelo->fecha = datos[fecha];
    vuelo->demora = /*atoi(*/datos[demora];
    vuelo->tiempo = datos[tiempo];
    vuelo->cancelados = datos[cancelados];

    //printf("num: %s,aerol: %s,prioridad: %s,fecha: %s\n",vuelo->num,vuelo->aerolinea,vuelo->prioridad,vuelo->fecha);
    return vuelo;
}

void _borrar_vuelo(vuelo_t *vuelo){
    free(vuelo->num);
    free(vuelo->aerolinea);
    free(vuelo->origen);
    free(vuelo->dest);
    free(vuelo->demora);
    free(vuelo->cola);
    free(vuelo->prioridad);
    free(vuelo->fecha);
    free(vuelo->cancelados);
    free(vuelo->tiempo);
    free(vuelo);
}

void borrar_vuelo(void *vuelo){
    _borrar_vuelo((vuelo_t*)vuelo);
}

int comparar_abb(const char *a, const char *b){
  return strcmp(a,b);
  /*  if(strcmp(a->fecha,b->fecha) == 0){
        return a->num > b->num;//return strcmp(a->num,b->num);
    }
    return strcmp(a->fecha,b->fecha);*/
/*
    return strcmp(a->fecha,b->fecha) == 0 ? strcmp(a->fecha,b->fecha) : strcmp(a->num,b->num);
*/
}


int _comparar_heap(vuelo_t* a, vuelo_t *b){
    /*int pa = atoi(a->prioridad);
    int pb = atoi(b->prioridad);
    
    if(pa == pb){//compara la prioridad, y ordena de menor a mayor
        return strcmp(a->num,b->num) ;
    }*/
    if(strcmp(a->prioridad,b->prioridad) == 0){//compara la prioridad, y ordena de menor a mayor
        return strcmp(a->num,b->num) ;
    }
    return (strcmp(a->prioridad,b->prioridad));
/*
    return strcmp(a->fecha,b->fecha) == 0 ? strcmp(a->fecha,b->fecha) : strcmp(a->num,b->num);
*/
}
 
int comparar_heap(/*(vuelo_t*)*/const void *a, /*(vuelo_t*)*/const void *b){
    return _comparar_heap((vuelo_t*)b,(vuelo_t*)a);  
   /* vuelo_t * va = (vuelo_t*)a;
    vuelo_t * vb = (vuelo_t*)b;
   // printf("cmp\n");
    if(strcmp(((vuelo_t*)a)->prioridad,((vuelo_t*)b)->prioridad) == 0){//compara la prioridad, y ordena de menor a mayor
        return strcmp(va->num,vb->num) ;
    }
    return strcmp(va->prioridad,vb->prioridad);*/
/*
    return strcmp(a->fecha,b->fecha) == 0 ? strcmp(a->fecha,b->fecha) : strcmp(a->num,b->num);
*/
}




bool agregar_archivo(const char *archivo,hash_t *hash, abb_t *abb){
    FILE* arch = fopen(archivo,"r");
    if(!arch){
        fprintf(stderr,"ERROR al abrir el archivo, en agregar_archivo\n");
        return false;
    }
    
    char* linea = NULL;
    size_t capacidad = 0; 
    ssize_t leidos = getline(&linea, &capacidad, arch);
    
    while(leidos >= 0){
       //printf("entro\n");
        char **datos = split(linea,',');
        vuelo_t *vuelo = vuelo_crear(datos);
        //printf("num: %s,aerol: %s,prioridad: %s,fecha: %s\n",vuelo->num,vuelo->aerolinea,vuelo->prioridad,vuelo->fecha);
       /* char *clave_abb = calloc(strlen(datos[fecha])+strlen(datos[num])+1,sizeof(char))
        strcpy(clave_abb,datos[fecha]);
        strcat(clave_abb,datos[num]);*/
        char *claves[3];
        claves[0] = datos[fecha];
        claves[1] = datos[num];
        claves[2] = NULL;
        char *clave_abb = join(claves,'-');
       // printf("llego\n");
        if(!hash_guardar(hash,datos[num],vuelo) || !abb_guardar(abb,clave_abb,vuelo) ){
            fprintf(stderr,"ERROR de memoria\n");
            return false;
        };
        free(clave_abb);
        free(datos);
        //printf("kknum: %s,aerol: %s,prioridad: %s,fecha: %s\n",v->num,v->aerolinea,v->prioridad,v->fecha);
        leidos = getline(&linea, &capacidad, arch);	
    }
    free(linea);
    fclose(arch);
    return true;
}

typedef struct tablero{
    long k;
    long contador;
    pila_t *pila;
    bool asc;
}tablero_t;

bool visitar(const char *clave, void *vuelo, void *extra){
    if(((tablero_t*)extra)->contador >= ((tablero_t*)extra)->k)return false;
   
    if(((tablero_t*)extra)->asc){
       fprintf(stdout,"%s - %s\n",((vuelo_t*)vuelo)->fecha,((vuelo_t*)vuelo)->num);
    }else{
        pila_apilar(((tablero_t*)extra)->pila,vuelo);
    }
    ((tablero_t*)extra)->contador++;
    return true;
}

bool ver_tablero(char **operacion, abb_t *abb){
    long k = strtol(operacion[1],NULL,10);
    if(k <= 0)return false;
    if(strcmp(operacion[2],"asc") != 0 && strcmp(operacion[2],"desc") != 0)return false;
    if(strcmp(operacion[3],operacion[4]) > 0 )return false;
    
    tablero_t *t = malloc(sizeof(tablero_t));
    if(!t)return false;
    t->contador = 0;
    t->k = k;
    if(strcmp(operacion[2],"asc") == 0){
        //printf("gneio entro al asc\n");
        t->asc = true;
        abb_in_order_finito(abb,visitar,operacion[3],operacion[4],t);
    }else{
        t->asc = false;
        t->pila = pila_crear();
        if(!t->pila){
            free(t);
            return false;
        }
        abb_in_order_finito(abb,visitar,operacion[3],operacion[4],t);
        while(!pila_esta_vacia(t->pila)){
            vuelo_t *v = (vuelo_t*)pila_desapilar(t->pila);
            fprintf(stdout,"%s - %s\n",v->fecha,v->num);
        }
        pila_destruir(t->pila);
    }
    free(t);
    return true;
}

bool info_vuelo(char **operacion, hash_t *hash){
    vuelo_t *vuelo = (vuelo_t*)hash_obtener(hash,operacion[1]);
    if(!vuelo)return false;

    fprintf(stdout,"%s %s %s %s %s %s %s %s %s %s", vuelo->num, vuelo->aerolinea, vuelo->origen, vuelo->dest, vuelo->cola, vuelo->prioridad, vuelo->fecha, vuelo->demora,vuelo->tiempo,vuelo->cancelados);
    return true;
}

bool prioridad_vuelos(char **operacion, abb_t *abb){
    heap_t *heap = heap_crear(comparar_heap);//PARA VER PRIORIDADES
    if(!heap)return false;

    abb_iter_t *iter = abb_iter_in_crear(abb);
    if(!iter){
        heap_destruir(heap,NULL);
        return false;
    }

    pila_t *res = pila_crear();
    if(!res){
        heap_destruir(heap,NULL);
        abb_iter_in_destruir(iter);
        return false;
    }

    long k = strtol(operacion[1],NULL,10);
    
    for(size_t i = 0; i<k; i++){
        heap_encolar(heap,abb_iter_in_ver_dato_actual(iter));
        abb_iter_in_avanzar(iter);
    }
    /*abb_iter_in_destruir(iter);
    abb_iter_t *iter_2 = abb_iter_in_crear(abb);
    if(!iter_2){
        heap_destruir(heap,NULL);
        return false;
    }*/

    //for(size_t i = 0; i < abb_cantidad(abb); i++){
    while(!abb_iter_in_al_final(iter)){
        //printf("entro al for\n");
        if(comparar_heap(abb_iter_in_ver_dato_actual(iter),heap_ver_max(heap)) < 0){
            //printf("entro\n");
            heap_desencolar(heap);
            heap_encolar(heap,abb_iter_in_ver_dato_actual(iter));
        }
        abb_iter_in_avanzar(iter);
    }
    abb_iter_in_destruir(iter);

    while(!heap_esta_vacio(heap)){
        pila_apilar(res,heap_desencolar(heap));
    }
    while(!pila_esta_vacia(res)){
        vuelo_t *desapilado = (vuelo_t*)pila_desapilar(res);
        fprintf(stdout,"%s - %s\n",desapilado->prioridad,desapilado->num);
    }
    pila_destruir(res);
    //fprintf(stdout,"OK\n");
    heap_destruir(heap,NULL);
    return true;
}

bool apilar(const char *clave, void *vuelo, void *pila){
    if(!pila_apilar((pila_t*)pila,vuelo))return false;
    return true;
}

bool borrar(char **operacion, hash_t *hash, abb_t *abb){
    if(strcmp(operacion[1],operacion[2]) > 0 )return false;
    
    pila_t *pila = pila_crear();
    if(!pila)return false;

    abb_in_order_finito(abb,apilar,operacion[1],operacion[2],pila);
    
    while(!pila_esta_vacia(pila)){
        vuelo_t *v = (vuelo_t*)pila_desapilar(pila);
        char *claves[3];
        claves[0] = v->fecha;
        claves[1] = v->num;
        claves[2] = NULL;
        char *clave_abb = join(claves,'-');
        hash_borrar(hash,v->num);
        abb_borrar(abb,clave_abb);
        borrar_vuelo(v);
        free(clave_abb);
    }
    pila_destruir(pila);
    return true;
}


void error(char **operacion){
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
            printf("entro a agregar\n");
            if(!agregar_archivo(operacion[1],hash,abb))return false;
			break;
		case op_tablero :
            printf("entro a ver tablero\n");
            if(!ver_tablero(operacion,abb))return false;
			break;
		case op_info :
            printf("entro a info vuelo\n");
            if(!info_vuelo(operacion,hash))return false;
			break;
		case op_prioridad :
            printf("entro a prioridades\n");
            if(!prioridad_vuelos(operacion,abb))return false;
			break;
		case op_borrar :
            printf("entro a borrar\n");
            if(!borrar(operacion,hash,abb))return false;
			break;
	}
    fprintf(stdout,"OK\n");
	return true;
}


/*IDENTIFICAR OPERACION REALIZA TODAS LAS CUENTAS NECESARIAS POR LÍNEA PARA DEVOLVER UN ÚLTIMO RESULTADO
DEFINITIVO. LA MISMA LLAMA A CALCULAR PARA CADA VEZ QUE LEE UN OPERANDO. EL MAIN LLAMA A CALCULAR
UNA VEZ PARA CADA LINEA*/
void identificar_operacion(char *linea, hash_t *hash, abb_t *abb){

    //char *l = strndup(linea,strlen(linea)-1);
    //size_t i = 0;
    /*linea[leidos-1] = '\0';
    while(linea[i] == ' ')i++; SUPONGO QUE ME INGRESAN TODO BIEN SIN ESPACIOS*/	
    linea[strlen(linea)-1] = '\0';
    char **operacion = split(linea,' ');
    //i = 0;
    bool operado = false;
    int n = 0;
    while(n <= op_borrar  && !operado){
        //printf("entro al strcmp\n");
        //printf("operacion[0]:%s\n",operacion[0]);   
        if(strcmp(operacion[0],acciones[n]) == 0){
            //printf("entro al strcmp\n");
            if(!operar(n,operacion,hash,abb)){
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
    }
	//free(l);
    free_strv(operacion);
}




int main(int argc, char *argv[]){

    hash_t *hash = hash_crear(NULL);//PARA INFO VUELOS
    if(!hash){
        fprintf(stderr,"ERROR de memoria\n");
        //return -1;
    }

    abb_t *abb = abb_crear(comparar_abb,borrar_vuelo);//PARA VER TABLERO
    if(!abb){
        hash_destruir(hash);
        fprintf(stderr,"ERROR de memoria\n");
       // return-1;
    }

    char* linea = NULL;
    size_t capacidad = 0; 
    ssize_t leidos = getline(&linea, &capacidad, stdin);
    
    while(leidos >= 0){	
        identificar_operacion(linea,hash,abb);	
        leidos = getline(&linea, &capacidad, stdin);	
    }
    free(linea);
    hash_destruir(hash);
    abb_destruir(abb);
}