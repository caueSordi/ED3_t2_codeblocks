#ifndef CCOMANDOS_H
    #define COMANDOS_H

#include "registro.h"
#include "cabecalho.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


//comandos basicos
void CREATE_TABLE(char *nomeCSV, char *nomearqbin, Cabecalho *c);
void SELECT_TABLE(char *nomearqbin);
int SELECT_WHERE(char *nome, char *campo, int n);
int INSERT_INDICE(char *binario, char *indice, Cabecalho *c);
void SEARCH_INDICE(char *binario, char *indice, long int campo);
//Abertura de arquivo
FILE* abertura_arqBin(char *nome, char *tipo);
#endif
