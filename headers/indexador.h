#ifndef INDEXADOR_FILE
#define INDEXADOR_FILE

#include <stdio.h>
#include <string.h>

void indexPorPais(char * linha, long int posicaoLinha, FILE *arquivoPorPais);

//reune todos os index para fazer os indexadores
void indexador();
int main(int argv, char *argc[]);

#endif