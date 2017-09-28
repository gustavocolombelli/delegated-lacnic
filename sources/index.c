#include "index.h"

tpTuplaPrincipal linhaParaStruct(char* linha){

	tpTuplaPrincipal tuplaPrincipal;

	char *token;
	token = strtok(linha, "|");//dado irrelevante
    
		if(linha[7]=='|'){
			strcpy(tuplaPrincipal.pais, "00");	
		}
       else{
       		token = strtok(NULL, "|");
      	    strcpy(tuplaPrincipal.pais, token);	
       }

       token = strtok(NULL, "|");
       strcpy(tuplaPrincipal.tipoIp, token);
       

       token = strtok(NULL, "|");
       strcpy(tuplaPrincipal.ip, token);
       


       token = strtok(NULL, "|");
       strcpy(tuplaPrincipal.ipQuantity, token);
       

      	
       	token = strtok(NULL, "|");

       if(strstr(token, "available")){
       		strcpy(tuplaPrincipal.data, "00000000");
       		strcpy(tuplaPrincipal.status, "available");
       
       }
       else if(strstr(token, "reserved")){
       		strcpy(tuplaPrincipal.data, "00000000");
       		strcpy(tuplaPrincipal.status, "reserved");
       		
       }
       else{
       		strcpy(tuplaPrincipal.data, token);
       		token = strtok(NULL, "|");
       		strcpy(tuplaPrincipal.status, token);
       		
       }
       return tuplaPrincipal; 
}

tpTuplaPrincipal buscaLinha(long int posicao){
	
	FILE* arquivoPrincipal;
	char linha[MAX_STR];
	if((arquivoPrincipal=fopen("../data/delegated-lacnic-extended-20170903", "r"))==NULL){
		printf("Erro ao abrir o arquivo\n");
	}
	else{
		 if(fseek(arquivoPrincipal, posicao, SEEK_SET)==0){
			fscanf(arquivoPrincipal, "%s", linha);
			return linhaParaStruct(linha);
		}
		else{
			printf("[ERRO] posicao inválida, buscaL");
		}
	}

}

void printaTuplaPrincipal(tpTuplaPrincipal tuplaPrincipal){
	printf("[Pais:%s] ", tuplaPrincipal.pais);
	printf("[Tipo ip:%s] ", tuplaPrincipal.tipoIp);
	printf("[Ip:%s] ", tuplaPrincipal.ip);
	printf("[Ip quantity:%s] ", tuplaPrincipal.ipQuantity);
	printf("[data: %s] ", tuplaPrincipal.data);	
    printf("[status: %s] ", tuplaPrincipal.status);
    printf("[Pos: %ld]\n", tuplaPrincipal.posicaoLinha);

}

void indexarPorPais(tpTuplaPrincipal tuplaPrincipal, FILE * indexXpais){

	tpIndexPais tuplaPais;
	
	//passando parametros necessarios para a struct do index indexXpais
	strcpy(tuplaPais.pais, tuplaPrincipal.pais);
	tuplaPais.posicaoIndexPrincipal = tuplaPrincipal.posicaoLinha;
	
	fwrite(&tuplaPais, sizeof(tpIndexPais), 1, indexXpais);  

}

void indexarPorTipo(tpTuplaPrincipal tuplaPrincipal, FILE * arquivoIndexIpv4, FILE * arquivoIndexIpv6, FILE * arquivoIndexAsn){

	tpIndexTipoIp tuplaIndexTipoIp;
	strcpy(tuplaIndexTipoIp.tipoIp, tuplaPrincipal.tipoIp);
	tuplaIndexTipoIp.posicao = tuplaPrincipal.posicaoLinha;

	if(strcmp(tuplaPrincipal.tipoIp, "ipv4")==0){
		fwrite(&tuplaIndexTipoIp, sizeof(tpIndexTipoIp), 1, arquivoIndexIpv4);
		return;
	}
	else if(strcmp(tuplaPrincipal.tipoIp, "ipv6")==0){
		fwrite(&tuplaIndexTipoIp, sizeof(tpIndexTipoIp), 1, arquivoIndexIpv6);
		return;
	}
	else if(strcmp(tuplaPrincipal.tipoIp, "asn")==0){
		fwrite(&tuplaIndexTipoIp, sizeof(tpIndexTipoIp), 1, arquivoIndexAsn);
		return;
	}
	else{
		printf("[ERRO] funcao indexarPorTipo, tipoIp nao encontrado");
		return;
	}


}

void buscaIndexPorPais(){

	tpIndexPais tuplaPais;
	tpTuplaPrincipal tuplaPrincipal;
	
	long int posicaoFimArquivo;

	char linha[MAX_STR];
	
	FILE * indexXpais,
		 * arquivoPrincipal;

	if((indexXpais=fopen("../data/indexadores/indexPorPais.bin", "rb"))==NULL){
		printf("Erro: abertura de arquivo indexPorPais.bin");
	}
	else if((arquivoPrincipal=fopen("../data/delegated-lacnic-extended-20170903", "r"))==NULL){
		printf("Erro: abertura do arquivo delegated-lacnic-extended-20170903");
	}
	else{

		while(!feof(arquivoPrincipal)){

			fread(&tuplaPais, 1, sizeof(tpIndexPais), indexXpais);
			fseek(arquivoPrincipal, tuplaPais.posicaoIndexPrincipal, SEEK_SET);
			
			fscanf(arquivoPrincipal, "%s", linha);
			tuplaPrincipal = linhaParaStruct(linha);
			tuplaPrincipal.posicaoLinha = tuplaPais.posicaoIndexPrincipal;
			
			printaTuplaPrincipal(tuplaPrincipal);

		}
		fclose(arquivoPrincipal);
		fclose(indexXpais);
	}
}

int contarQuantidadeTipoIp(char * tipoIp, char *nomeArquivo){
	
	FILE * indexTipoIp;
	long int tamanho;

	if((indexTipoIp=fopen(nomeArquivo, "rb"))==NULL){
		printf("Erro: abertura de arquivo indexPorPais.bin");
		return -1;
	}
	else{
		fseek(indexTipoIp, 0, SEEK_END);
		tamanho = ftell(indexTipoIp);
		fclose(indexTipoIp);
		return tamanho/(sizeof(tpIndexTipoIp)); //tpIndexTipoIp é a estrutura de inxex por tipo de ip
	}
}

//função destinada para a criação dos indices, abertura unica do arquivo principal para a criação dos demais.
void indexador(){

tpTuplaPrincipal tuplaPrincipal;

FILE *arquivoPrincipal, 
	 *arquivoIndexPais,
	 *arquivoIndexIpv4,
	 *arquivoIndexIpv6,
	 *arquivoIndexAsn;


char linha[MAX_STR];
long int posicaoLinha;

	arquivoIndexPais=fopen("../data/indexadores/indexPorPais.bin", "wb");
	arquivoIndexIpv4=fopen("../data/indexadores/indexIpv4.bin", "wb");
	arquivoIndexIpv6=fopen("../data/indexadores/indexIpv6.bin", "wb");
	arquivoIndexAsn=fopen("../data/indexadores/indexAsn.bin", "wb");

	if((arquivoPrincipal=fopen("../data/delegated-lacnic-extended-20170903", "r"))==NULL){
		printf("Erro ao abrir o arquivo");

	}
	else{
		while(!feof(arquivoPrincipal)){
	
			//enquadramento da linha para tupla principal
			posicaoLinha = ftell(arquivoPrincipal);
			fscanf(arquivoPrincipal, "%s", linha);
			tuplaPrincipal = linhaParaStruct(linha);
			tuplaPrincipal.posicaoLinha = posicaoLinha;

		  	//---INDEXADORES---
			//indexarPorPais(tuplaPrincipal, arquivoIndexPais);
			indexarPorTipo(tuplaPrincipal, arquivoIndexIpv4, arquivoIndexIpv6, arquivoIndexAsn);
		}

		fclose(arquivoIndexPais);
		fclose(arquivoPrincipal);
		fclose(arquivoIndexIpv4);
		fclose(arquivoIndexIpv6);
		fclose(arquivoIndexAsn);
	}


}



int ipPorStatus(char * status, char * tipoIp, char *nomeArquivoIndexPorIp){
	char linha[200];
	tpIndexTipoIp tuplaIndexTipoIp;
	tpTuplaPrincipal tuplaPrincipal;
	FILE *arquivoIndexIp;
	FILE *arquivoPrincipal;
	int quantidade=0;

	if((arquivoIndexIp=fopen(nomeArquivoIndexPorIp, "r"))==NULL){
		printf("[Erro] Abertura do arquivo %s, funcao ipPorStatus", nomeArquivoIndexPorIp);

	}
	else if((arquivoPrincipal=fopen("../data/delegated-lacnic-extended-20170903", "r"))==NULL){
		printf("Erro ao abrir o arquivo principal");

	}

	else{
		while(!feof(arquivoPrincipal)){

			fscanf(arquivoPrincipal, "%s", linha);
			
			tuplaPrincipal = linhaParaStruct(linha);

			if(strcmp(tuplaPrincipal.tipoIp, tipoIp)==0 && (strcmp(tuplaPrincipal.status, traduzir(status))==0)){
				printf("[%s] [%s] [%s]\n", tuplaPrincipal.tipoIp, tuplaPrincipal.ip , tuplaPrincipal.status);
				quantidade++;
			}
		}

		fclose(arquivoIndexIp);
		fclose(arquivoPrincipal);
	}



	return quantidade;
}

void contar(char *argv[]){
	char nomeArquivoIndexPorIp[60];

		if (strcmp(argv[2],"ipv4")==0){
			strcpy(nomeArquivoIndexPorIp, "../data/indexadores/indexIpv4.bin");
			
		}
		else if(strcmp(argv[2],"ipv6")==0){
			strcpy(nomeArquivoIndexPorIp, "../data/indexadores/indexIpv6.bin");
		}
		else if(strcmp(argv[2],"asn")==0){
			strcpy(nomeArquivoIndexPorIp, "../data/indexadores/indexAsn.bin");
		}
		else{
			printf("[ERRO] Tipo de ip invalido\n");
			return;
		}

		if(strcmp(argv[1], "quantidade")==0 && argv[3]==NULL){
			printf("\n---\nQuantidade de %s correspondentes: %d\n", argv[2], contarQuantidadeTipoIp(argv[2], nomeArquivoIndexPorIp));
		}
		else if(strcmp(argv[1], "quantidade")==0 && ((strcmp(argv[3], "alocados")==0)||(strcmp(argv[3], "disponiveis")==0)||(strcmp(argv[3], "reservados")==0))){
			printf("\n--\t--\t--\t--\t--\n[TOTAL DE %d %s %s]\n", ipPorStatus(argv[3], argv[2], nomeArquivoIndexPorIp),argv[2], argv[3]);
			}

		else{
			printf("\n[ERRO] Argumento/s invalido/s\n");
		}

}




int main(int argc, char *argv[]){

	if(strcmp(argv[1], "-indexar") == 0){
		printf("\n---\n[INDEXADO COM SUCESSO]");
		indexador();
		printf("\nTodos os indices foram criados com sucesso!\n---\n");
	}
	else if (strcmp(argv[1], "quantidade")==0)
	{
		contar(argv);
	}

	else{
		printf("[ERRO] Comando Inválido\n\n");
	}



	return 0;
}



/*
FEITOS
a) Importar o arquivo-texto, e gerar arquivo(s) estruturado(s) indexado(s)
g) Contar a quantidade de IPv4 de um país;
h) Contar a quantidade de IPv6 de um país; (fórmula)
i) Contar a quantidade de ASN de um país;
m) Imprimir a quantidade de endereços IPv4 alocados;
n) Imprimir a quantidade de endereços IPv4 disponíveis;
o) Imprimir a quantidade de endereços IPv4 reservados;
p) Imprimir a quantidade de endereços IPv6 alocados; (fórmula)
q) Imprimir a quantidade de endereços IPv6 disponíveis; (fórmula)
r) Imprimir a quantidade de endereços IPv6 reservados; (fórmula)
y) Imprimir os ASN disponíveis;
w) Imprimir os blocos IPv4 disponíveis;
x) Imprimir os blocos IPv6 disponíveis;

FAZENDO
j) Imprimir o ranking de ASN por pais em ordem decrescente;
k) Imprimir o ranking da quantidade de IPv4 por pais em ordem decrescente;
l) Imprimir o ranking da quantidade de IPv6 por pais em ordem decrescente; (fórmula)

A FAZER

s) Mostrar a data de alocação de um ASN;
t) Mostrar a data de alocação de um bloco IPv4;
u) Mostrar a data de alocação de um bloco IPv6;
v) Mostrar a quantidade de recursos (ASN/IPv4/IPv6) alocados em um ano e/ou mês específico;
z) Zerar a base de dados (e arquivos de índices);







*/