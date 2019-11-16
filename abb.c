#include "abb.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "pila.h"
#define DER  -1
#define IZQ  1


typedef struct nodo{
    struct nodo* izq;
    struct nodo* der;
	char* clave;
    void* dato;
}nodo_t;

struct abb{
    size_t cantidad;
    nodo_t* raiz;
    abb_comparar_clave_t comparar;
    abb_destruir_dato_t dato_destruir;
};
/*****************************************************************
 *                         PRIMITIVAS NODO                       *
 * ***************************************************************/

char *copiar_clave(const char *clave)
{
    char *copia = malloc(strlen(clave)+1);
    if (copia == NULL){
        return NULL;
	}
    strcpy(copia, clave);
    return copia;
}

nodo_t* nodo_crear(const char* clave, void* dato){

    nodo_t* nodo = malloc(sizeof(nodo_t));
    if (!nodo)return NULL;
    char* copia = copiar_clave(clave);
	if (!copia){
		free (nodo);
		return NULL;
	}
    nodo->clave = copia;
    nodo->dato = dato;
    nodo->der = NULL;
    nodo->izq = NULL;
	return nodo;
}
void nodo_destruir(abb_destruir_dato_t destruir_dato, nodo_t* nodo){
	if(destruir_dato != NULL){
		destruir_dato(nodo->dato);
	}
	free(nodo->clave);
    free(nodo);
}

/********************************************************************
 *                     FUNCIONES AUXLIARES ABB                      *
 * ******************************************************************/

/* abb_buscar: devuelve el nodo con la clave correspondiente
o en caso de no existir devuelve su posible padre
Devuelve null si el arbol esta vacio*/

nodo_t* _padre(const abb_t* arbol, nodo_t *nodo, const char* clave){
	if(arbol->comparar(nodo->clave, clave) < 0){
		if(!nodo->der || arbol->comparar(nodo->der->clave,clave) == 0)return nodo;
        return _padre(arbol,nodo->der, clave);
    }else{
		if(!nodo->izq || arbol->comparar(nodo->izq->clave,clave) == 0)return nodo;
        return _padre(arbol,nodo->izq, clave);
	}
}
nodo_t* padre(const abb_t* arbol,const char* clave){
    if(!arbol->raiz || arbol->comparar(arbol->raiz->clave,clave) == 0)return NULL;
    return _padre(arbol,arbol->raiz,clave);
}

bool sin_hijos(const nodo_t *nodo){
	if(!nodo->der && !nodo->izq)return true;
	return false;
}

nodo_t* abb_buscar(const abb_t* arbol,const char* clave){
    if(arbol->raiz==NULL)return NULL;
    if(arbol->comparar(arbol->raiz->clave,clave) == 0)return arbol->raiz;
	nodo_t *buscado = padre(arbol,clave);
	if(sin_hijos(buscado))return buscado;
	if(arbol->comparar(buscado->clave,clave) < 0 && buscado->der!= NULL)return buscado->der;
	if(arbol->comparar(buscado->clave,clave) > 0 && buscado->izq!= NULL)return buscado->izq;
	return buscado;
}


void borrar_un_hijo(abb_t* arbol, nodo_t *a_borrar, nodo_t* papa,int borrar_pos, int hijo_pos ){
	
	if (borrar_pos == DER){
		if (hijo_pos == DER){
			papa->der = a_borrar->der;
		}else{
			papa->der = a_borrar->izq;
		}	
	}else if(borrar_pos == IZQ){
		if (hijo_pos == IZQ){
			papa->izq = a_borrar->izq;
		}else{
			papa->izq = a_borrar->der;
		}
	}else{
		if(hijo_pos == IZQ){
			arbol->raiz = a_borrar->izq;
		}else{
			arbol->raiz = a_borrar->der;
		}
	}
	nodo_destruir(NULL,a_borrar);
}

nodo_t* buscar_remplazante(nodo_t* a_borrar){
	nodo_t* act = a_borrar->der;
	while(act->izq){
		act = act->izq;
	}
	return act;
}

void borrar_dos_hijos(abb_t* arbol, nodo_t* a_borrar){
	nodo_t* reemplazo = buscar_remplazante(a_borrar);
	nodo_t* ant = padre(arbol,reemplazo->clave);
	free(a_borrar->clave);
	a_borrar->clave = copiar_clave(reemplazo->clave);
	a_borrar->dato = reemplazo->dato;
	if(!reemplazo->der){
		if(ant != a_borrar){
			ant->izq = NULL;
		}else{
			ant->der = NULL;
		}
		nodo_destruir(NULL,reemplazo);
	}else{
		if(ant != a_borrar){
			borrar_un_hijo(arbol,reemplazo,ant,IZQ,DER);
		}else{
			a_borrar->der = reemplazo->der;
			nodo_destruir(NULL,reemplazo);
		}
	}
}	

void borrar_raiz(abb_t *arbol){
	if(arbol->cantidad == 1){
		nodo_destruir(NULL,arbol->raiz);
		arbol->raiz = NULL;
	}else if(arbol->raiz->der && arbol->raiz->izq){
		borrar_dos_hijos(arbol,arbol->raiz);
	}else if(arbol->raiz->izq){
		borrar_un_hijo(arbol,arbol->raiz,NULL,0,IZQ);
	}else{
		borrar_un_hijo(arbol,arbol->raiz,NULL,0,DER);
	}
}

void reemplazar(abb_t *abb, nodo_t *nuevo, nodo_t *viejo){
	nuevo->der = viejo->der;
	nuevo->izq = viejo->izq;
	nodo_destruir(abb->dato_destruir,viejo);
	abb->cantidad--;
}

void subarbol_destruir_post(abb_t *arbol,nodo_t *nodo){
	if(!nodo)return;	
	subarbol_destruir_post(arbol,nodo->izq);
	subarbol_destruir_post(arbol,nodo->der);
	nodo_destruir(arbol->dato_destruir,nodo);		
	return;
}

/*****************************************************************
 *                         PRIMITIVAS ABB                        *
 * ***************************************************************/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    
    abb_t* abb = malloc(sizeof(abb_t));
    if (!abb)return NULL;
    abb->cantidad = 0;
    abb->raiz = NULL;
    abb->dato_destruir = destruir_dato;
    abb->comparar = cmp;
	return abb;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
	if(clave == NULL)return false;
	
    nodo_t* nodo = nodo_crear(clave,dato);
    if (!nodo)return false;
 
    nodo_t* papa = padre(arbol, clave);
    
    if(!papa){
		if(arbol->raiz != NULL){
			reemplazar(arbol,nodo,arbol->raiz);
		}
		arbol->raiz = nodo;
    }else{
		if (arbol->comparar(papa->clave, clave) < 0){
			if(papa->der){
				reemplazar(arbol,nodo,papa->der);
			}
			papa->der = nodo;
		}else if (arbol->comparar(papa->clave, clave) > 0){
			if(papa->izq != NULL){
				reemplazar(arbol,nodo,papa->izq);
			}
			papa->izq = nodo;
		}	
	}
    arbol->cantidad++;
    return true;
}

void *abb_borrar(abb_t *arbol, const char *clave){
	if(!arbol->raiz || clave ==  NULL)return NULL;
	nodo_t *a_borrar;
	void *dato;
	nodo_t *papa = padre(arbol,clave);
	if(!papa){
		if(arbol->comparar(arbol->raiz->clave,clave) != 0)return NULL;
		dato = arbol->raiz->dato;
		borrar_raiz(arbol);
	}else if(sin_hijos(papa)){
		return NULL;
	}else if(arbol->comparar(papa->clave,clave) > 0){
		if(!papa->izq)return NULL;	
		a_borrar = papa->izq;
		dato = a_borrar->dato;
		if(sin_hijos(a_borrar)){
			nodo_destruir(NULL,a_borrar);
			papa->izq = NULL;
		}else if(a_borrar->izq && a_borrar->der){
			borrar_dos_hijos(arbol, a_borrar);
		}else if (a_borrar->izq){
			borrar_un_hijo(arbol,a_borrar,papa,IZQ,IZQ);
		}else{
			borrar_un_hijo(arbol,a_borrar,papa,IZQ,DER);
		}
	}else{
		if(!papa->der)return NULL;
		a_borrar = papa->der;
		dato = a_borrar->dato;
		if(sin_hijos(a_borrar)){
			nodo_destruir(NULL,a_borrar);
			papa->der= NULL;
		}else if(a_borrar->izq && a_borrar->der){
			borrar_dos_hijos(arbol,a_borrar);
		}else if (a_borrar->izq){
				borrar_un_hijo(arbol,a_borrar,papa,DER,IZQ);
		}else{
				borrar_un_hijo(arbol,a_borrar,papa,DER,DER);
		}
	}	
	arbol->cantidad--;
	return dato;
}

void *abb_obtener(const abb_t *arbol, const char *clave){
	if(clave == NULL)return NULL;
	nodo_t *buscado = abb_buscar(arbol,clave);
	if(!buscado || arbol->comparar(clave,buscado->clave) != 0)return NULL;
	return buscado->dato;	
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
	if(clave == NULL)return false;
	nodo_t* nodo = abb_buscar(arbol,clave);
	if( !nodo || arbol->comparar(clave,nodo->clave)!=0)return false;
	return true;
}

size_t abb_cantidad(abb_t *arbol){
	return arbol->cantidad;
}

void abb_destruir(abb_t *arbol){
	subarbol_destruir_post(arbol,arbol->raiz);
	free(arbol);
}

 /****************************************************************
 *               PRIMITIVA DEL ITERADOR INTERNO                  *
 * ***************************************************************/
 
void _abb_in_order(nodo_t* nodo, bool visitar(const char *, void *, void *),void* extra, bool *corte){
	
	if (!nodo)return;
	_abb_in_order(nodo->izq,visitar,extra,corte);
	if(*corte)return;
	if(!visitar(nodo->clave,nodo->dato,extra)){
		*corte = true;
	}
	_abb_in_order(nodo->der,visitar,extra,corte);
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
	
	if (!arbol->raiz)return;
	bool corte = false;
	_abb_in_order(arbol->raiz,visitar,extra,&corte);
	
}

/*****************************************************************
 *           FUNCION AUXILIAR DEL ITERADOR EXTERNO               *
 * ***************************************************************/

void apilar_hijos_izq(nodo_t *nodo , pila_t *pila){
	if(!nodo)return;
	pila_apilar(pila,nodo);
	nodo_t *hijo = nodo->izq;
	while(hijo){
		pila_apilar(pila,hijo);
		hijo = hijo->izq;
	}	
}

/*****************************************************************
 *               PRIMITIVAS DEL ITERADOR EXTERNO                 *
 * ***************************************************************/
 
struct abb_iter{
	const abb_t *abb;
	pila_t *pila;
};

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
	
	abb_iter_t *iter = malloc(sizeof(abb_iter_t));
	if(!iter)return NULL;
	
	iter->pila = pila_crear();
	if(!iter->pila){
		free(iter);
		return NULL;
	}
	apilar_hijos_izq(arbol->raiz,iter->pila);
	
	iter->abb = arbol;
	return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
	if(abb_iter_in_al_final(iter))return false;
	
	nodo_t *desapilado = (nodo_t*)pila_desapilar(iter->pila);
	if(desapilado->der){
		apilar_hijos_izq(desapilado->der,iter->pila);
	}
	return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
	nodo_t* act = (nodo_t*)pila_ver_tope(iter->pila);
	if(act){
		return act->clave;
	}
	return NULL;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
	return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter){
	pila_destruir(iter->pila);
	free(iter);
}
