#ifndef INDEXAR_FILE
#define INDEXAR_FILE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STR 100

//para o carregamento do arquivo principal.
typedef struct{

	char pais[3];
	char tipoIp[5];
	char ip[25]; //ipv4, ipv6 ou asn
	char data[10];
  	char status[10];
  	char ipQuantity[8];

	long int posicaoLinha;

}tpTuplaPrincipal;

typedef struct indexPais
{
	char pais[3];
	long int posicaoIndexPrincipal;
}tpIndexPais;



tpTuplaPrincipal linhaParaStruct(char * linha);
void printaTuplaPrincipal(tpTuplaPrincipal tuplaPrincipal);
tpTuplaPrincipal buscaLinha(long int posicao);

// --- FUNÇÕES PARA CRIAÇÃO DE INDEX ---
void indexarPorPais(tpTuplaPrincipal tuplaPrincipal, FILE * indexXpais);



void buscaIndexPorPais();


//reune todos os index para fazer os indexadores
void indexador();




int main(int argv, char *argc[]);

#endif