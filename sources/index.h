#ifndef INDEX_FILE
#define INDEX_FILE

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

typedef struct indexTipoIp{
	char tipoIp[5];
	long int posicao;
}tpIndexTipoIp;



tpTuplaPrincipal linhaParaStruct(char * linha);
void printaTuplaPrincipal(tpTuplaPrincipal tuplaPrincipal);
tpTuplaPrincipal buscaLinha(long int posicao);

// --- FUNÇÕES PARA CRIAÇÃO DE INDEX ---
void indexarPorPais(tpTuplaPrincipal tuplaPrincipal, FILE * indexXpais);

void indexarPorTipo(tpTuplaPrincipal tuplaPrincipal, FILE * arquivoIndexIpv4, 
													 FILE * arquivoIndexIpv6, 
													 FILE * arquivoIndexAsn);

// FUNÇÕES DE PESQUISA
void buscaIndexPorPais();
int contarQuantidadeTipoIp(char * tipoIp, char *nomeArquivo); //(ipv4, ipv6, asn)
void contar(char *argv[]);
int ipPorStatus(char * acao, char * tipoIp, char * nomeArquivoIndexPorIp);

// FUNÇÕES AUXILIARES
char * traduzir(char *argv);

//reune todos os index para fazer os indexadores
void indexador();




int main(int argc, char *argv[]);

#endif