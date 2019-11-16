

#include "pila.h"
#include <stdlib.h>

#define TAM 20


struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad física de la pila.
};

bool pila_redimensionar( pila_t* pila , size_t TAM_NUEVO );

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t* pila_crear(void){

	pila_t* pila = malloc( sizeof(pila_t) );
	
	if ( pila == NULL ){
		return NULL;	
	}
	
	pila->datos = malloc( TAM * sizeof(void*) );
	
	if ( pila->datos == NULL) {
        free(pila);
        return NULL;
    }	 	
    
    pila->capacidad = TAM ;
    pila->cantidad = 0 ;

	return pila;
}
	
void pila_destruir(pila_t *pila){
	
		free(pila->datos);
		free(pila);	
}	

bool pila_esta_vacia(const pila_t *pila){

	return pila->cantidad == 0;
}	

bool pila_apilar(pila_t *pila, void* valor){
	
	if ( pila->capacidad == pila->cantidad ){
		if ( pila_redimensionar( pila , 2 *pila->capacidad ) == false ){
			return false ;
		}
	}
	
	pila->datos[pila->cantidad] = valor;
	pila->cantidad++;
	return true;
}	
				
void* pila_ver_tope(const pila_t *pila){
	
	if ( pila->cantidad == 0 ){//  if ( pila_esta_vacia(pila) == true ){
		return NULL;
	}else{
		return pila->datos[pila->cantidad-1];
	}
}			

void* pila_desapilar(pila_t *pila){
	
	if ( pila->cantidad == 0 ){//  if ( pila_esta_vacia(pila) == true ){
		return NULL;
	}
	 
	pila->cantidad--;
	void* valor = pila->datos[pila->cantidad];
	
	if ( pila->cantidad <= (pila->capacidad)/4 ){
		pila_redimensionar( pila , (pila->capacidad)/2 );
	}
	
	return valor;	
}

bool pila_redimensionar( pila_t* pila , size_t TAM_NUEVO ){
	
		if( TAM_NUEVO < TAM ){//Menor que el tamaño original
				return false;
		}
				
		void** datos_nuevo = realloc(pila->datos,TAM_NUEVO*sizeof(void*));
		
		if ( datos_nuevo == NULL ){
			return false;
		}else{
			pila->datos	= datos_nuevo;
			pila->capacidad = TAM_NUEVO;
			return true;
		}	
}	
	
