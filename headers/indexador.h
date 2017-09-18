#ifndef INDEXADOR_FILE
#define INDEXADOR_FILE

#include <stdio.h>
#include <string.h>

void indexPorPais(char * linha, long int posicaoLinha, FILE *arquivoPorPais);
void indexTiposXdisponibilidade(char * linha, long int posicaoLinha, FILE *arquivoIpv4, FILE *arquivoIpv6, FILE *arquivoAsn);

//reune todos os index para fazer os indexadores
void indexador();




int main(int argv, char *argc[]);

#endif