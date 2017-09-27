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
		printf("Erro ao abrir o arquivo");

	}
	else{
		 if(fseek(arquivoPrincipal, posicao, SEEK_SET)==0){
			fscanf(arquivoPrincipal, "%s", linha);
			return linhaParaStruct(linha);
		}
		else{
			printf("Erro, posicao inválida");
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
	tpIndexTipoIp tuplaIndexTipoIp;
	int quantidadeIps;

	if((indexTipoIp=fopen(nomeArquivo, "rb"))==NULL){
		printf("Erro: abertura de arquivo indexPorPais.bin");
	}
	else{

		for(quantidadeIps=0;!feof(indexTipoIp);quantidadeIps++){

			fread(&tuplaIndexTipoIp, 1, sizeof(tpIndexTipoIp), indexTipoIp);
			printf("%s\n%ld\n\n", tuplaIndexTipoIp.tipoIp, tuplaIndexTipoIp.posicao);

		}
		printf("%d", quantidadeIps);
		return quantidadeIps;
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



int main(int argc, char *argv[]){

	if(strcmp(argv[1], "-indexar") == 0){
		printf("ok, indexando");
		indexador();
		printf("\nTodos os indices foram criados com sucesso!\n");
	}
	else if (strcmp(argv[1], "quantidade")==0)
	{
		char nomeArquivo[20];
		if (strcmp(argv[2],"ipv4")==0)
		{
			strcpy(nomeArquivo, "../data/indexadores/indexIpv4.bin");
			contarQuantidadeTipoIp(argv[2], nomeArquivo);
		}
		else if(strcmp(argv[2],"ipv6")==0){
			strcpy(nomeArquivo, "../data/indexadores/indexIpv6.bin");
			contarQuantidadeTipoIp(argv[2], nomeArquivo);
		}
		else if(strcmp(argv[2],"asn")==0){
			strcpy(nomeArquivo, "../data/indexadores/indexAsn.bin");
			contarQuantidadeTipoIp(argv[2], nomeArquivo);
		}
		else{
			printf("[ERRO] Tipo de ip invalido");
			return 1;
		}

	}
	else{
		printf("[ERRO] Comando Inválido\n\n");
	}



	return 0;
}