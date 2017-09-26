#include "indexar.h"

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

void buscaIndexPorPais(){

	tpIndexPais tuplaPais;
	tpTuplaPrincipal tuplaPrincipal;
	
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

		while(!feof(indexXpais)){

			fscanf(arquivoPrincipal, "%s", linha);
			tuplaPrincipal = linhaParaStruct(linha);
			tuplaPrincipal.posicaoLinha = tuplaPais.posicaoIndexPrincipal;
			
			printaTuplaPrincipal(tuplaPrincipal);
			fread(&tuplaPais, 1, sizeof(tpIndexPais), indexXpais);
			fseek(arquivoPrincipal, tuplaPais.posicaoIndexPrincipal, SEEK_SET);


		}
	}
}

/*void indexTiposXdisponibilidade(char * linha, long int posicaoLinha, FILE *arquivoIpv4, FILE *arquivoIpv6, FILE *arquivoAsn){
	if (strstr(linha, "ipv4")) //&& available
	{
		fprintf(arquivoIpv4, "%ld ", posicaoLinha);
	}
	else if (strstr(linha, "ipv6")) //&& available
	{
		fprintf(arquivoIpv6, "%ld ", posicaoLinha);
	}
	else if (strstr(linha, "asn")) //&& available
	{
		fprintf(arquivoAsn, "%ld ", posicaoLinha);
	}

	else{
		printf("Erro ao fazer a indexacao dos ips por disponibilidade, favor verificar o arquivo matriz");
	}

}*/


//função destinada para a criação dos indices, abertura unica do arquivo principal para a criação dos demais.
void indexador(){

tpTuplaPrincipal tuplaPrincipal;

FILE *arquivoPrincipal, 
	 *arquivoPorPais;


char linha[MAX_STR];
long int posicaoLinha;

	arquivoPorPais=fopen("../data/indexadores/indexPorPais.bin", "wb");


	if((arquivoPrincipal=fopen("../data/delegated-lacnic-extended-20170903", "r"))==NULL){
		printf("Erro ao abrir o arquivo");

	}
	else{
		while(!feof(arquivoPrincipal)){
	
			//enquadramento da linha e obtenção da posição dela.

			posicaoLinha = ftell(arquivoPrincipal);
			fscanf(arquivoPrincipal, "%s", linha);
			tuplaPrincipal = linhaParaStruct(linha);
			tuplaPrincipal.posicaoLinha = posicaoLinha;

			//funções para indexadores, sempre serão passada a linha e a posição (long int)
		    //o objetivo é usar a mesma linha para fazer todos os indexadores primarios
			/*indexPorPais(linha, posicaoLinha, arquivoPorPais);
			indexTiposXdisponibilidade(linha, posicaoLinha, arquivoIpv4Disponiveis, arquivoIpv6Disponiveis, arquivoAsnDisponiveis);*/
			//printaTuplaPrincipal(tuplaPrincipal);
			indexarPorPais(tuplaPrincipal, arquivoPorPais);
		
		}

		fclose(arquivoPorPais);
		fclose(arquivoPrincipal);
	}


}



int main(int argv, char *argc[]){


	indexador();
	buscaIndexPorPais();
	//printaTuplaPrincipal(buscaLinha(-412));
	return 0;
}