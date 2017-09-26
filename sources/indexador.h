#ifndef INDEXADOR_FILE
#define INDEXADOR_FILE

#include <stdio.h>
#include <string.h>


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

tpTuplaPrincipal linhaParaStruct(char * linha);
void printaTuplaPrincipal(tpTuplaPrincipal tuplaPrincipal);

void indexPorPais(tpTuplaPrincipal tupla, FILE * indexXpais);
void indexTiposXdisponibilidade(char * linha, long int posicaoLinha, FILE *arquivoIpv4, FILE *arquivoIpv6, FILE *arquivoAsn);

//reune todos os index para fazer os indexadores
void indexador();




int main(int argv, char *argc[]);

#endif