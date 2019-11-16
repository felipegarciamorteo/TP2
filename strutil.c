#define _POSIX_C_SOURCE 200809L
#include "strutil.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


char *substr(const char *str, size_t n){

	if(str==NULL)return NULL;
	if(n > strlen(str)){		
		n = strlen(str);
	}
	char *substr = malloc((n+1)*sizeof(char));
	if(!substr)return NULL;
	snprintf(substr,n+1,"%s",str);
	return substr;
}


char **split(const char *str, char sep){
	size_t n;
	size_t i = 0;
	if( sep != '\0' && strchr(str,sep) != NULL ){
		char *apariciones = strchr(str,sep)+1;
		i++;
		while(strchr(apariciones,sep) != NULL){
			apariciones = strchr(apariciones,sep)+1;
			i++;
		}
	}				
	char **split = malloc((i+2)*sizeof(char*));
	if(!split)return NULL;
	char *copia = strdup(str);
	char *aux = copia;
	for(n = 0; n < i ; n++){
		size_t largo = strlen(aux)-strlen(strchr(aux,sep));
		split[n] = substr(aux,largo);//strndup(aux,largo);
		aux += largo+1;
	}
	split[n] = strdup(aux);
	free(copia);
	split[n+1] = NULL;
	return split;
}


char *join(char **strv, char sep){

	size_t i = 0;
	size_t largo = 0;	
	while(strv[i] != NULL){
		largo += strlen(strv[i]);
		i++;
	}
	
	char *join = malloc((largo+i+1)*sizeof(char));
	if(!join)return NULL;
	join[largo+i] = '\0';
	char *start = join;

	char s[1];
	s[0] = sep;
	
	if(strv[0] != NULL){
		size_t n = 0;
		strcpy(join,strv[n]);
		while(strv[n+1] != NULL){
			strcpy(join+strlen(join),s);
			n++;
			join += strlen(join);
			strcpy(join,strv[n]);
		}		
	}
	return start;
}


void free_strv(char *strv[]){
	size_t i = 0;
	while(strv[i] != NULL){	
		free(strv[i]);
		i++;
	}
	free(strv);
}
