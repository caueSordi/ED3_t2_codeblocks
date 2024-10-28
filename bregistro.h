#ifndef BREGISTRO_H
    #define BREGISTRO_H

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdbool.h>
    #include <stdlib.h>
    #include "bcabecalho.h"

    #define BREGISTRO_SIZE 93
    #define REGISTRO_FILL '$'
    #define REGISTRO_DELIMITADOR '#'
    #define MAXPAGE 93
    #define ORDEM 5
    #define CPR_SIZE 4
    #define REGISTRO_REMOVIDO_TRUE '1'
    #define REGISTRO_REMOVIDO_FALSE '0'

    typedef struct CPR{
        long int C;
        long int PR;
    } CPR;

    typedef struct PCPR{
        CPR corpo;
        int P;
    } PCPR;

    typedef struct {
        char folha;               // 1 byte para indicar se é nó folha (0 ou 1)
        int nroChavesIndexadas;   // 4 bytes para o número de chaves indexadas
        int RRNdoNo;              // 4 bytes para o RRN do nó
        int P[ORDEM];             // 4 bytes para cada ponteiro (P1 a P5), total de 5 ponteiros
        CPR CPRs[ORDEM - 1];      // 4 bytes para cada chave (C1 a C4), total de 4 chaves // 4 bytes para cada ponteiro de registro (PR1 a PR4), total de 4
        int lotacao;
    } NoArvore;


typedef struct _noPos
{
    NoArvore no;        // No em que foi encontrado / deveria estar
    NoArvore noAnt;     // No anterior para insercao com overflow
    int pos;            // Posição i do no em que foi encontrado
    int posInsercao;    // Posição i do no que deveria estar

} NoPos;
    NoArvore *no_criar(bool folha);
    void no_print(NoArvore *no);
    NoArvore *no_readbin(FILE* entrada);

    void no_writebin(FILE *nomebin, NoArvore *no);
    PCPR no_tenta_inserir(NoArvore *no,CPR valor_inserir,FILE *arquivo);
    PCPR no_inserir_recursivo(FILE *nomebin,NoArvore *atual,CPR valor_inserir, CabecalhoArvore *c,int level);

   int buscando_chave(FILE *arquivo_indice,NoArvore *atual, long int campo);
   void Arvore_empty(char *arquivobin, CPR info_cpt);

#endif

