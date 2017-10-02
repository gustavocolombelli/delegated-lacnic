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

int contarQuantidadeTipoIp(char *nomeArquivo){
	
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

void indexarPorData(tpTuplaPrincipal tuplaPrincipal, FILE *indexPorData){

	tpIndexPorData tuplaIndexPorData;

	strcpy(tuplaIndexPorData.data, tuplaPrincipal.data);
	tuplaIndexPorData.posicao = tuplaPrincipal.posicaoLinha;
	
	fwrite(&tuplaIndexPorData, sizeof(tpIndexPorData), 1, indexPorData);  

}

int quantidadeRecursos(char *tipoIp, char *tipoData, char *data){
	int quantidade=0;
	FILE *arquivoIndexPorData, *arquivoPrincipal;
	tpIndexPorData tuplaPorData;
	tpTuplaPrincipal tuplaPrincipal;
	char linha[MAX_STR];

	if((arquivoIndexPorData=fopen("../data/indexadores/indexPorData.bin", "rb"))==NULL){
		printf("Erro: abertura de arquivo indexPordata.bin");
	}
	else if((arquivoPrincipal=fopen(caminhoDiretorioArquivo("principal"), "r"))==NULL){
		printf("Erro ao abrir o arquivo principal");
	}
	else{
		while(!feof(arquivoIndexPorData)){
			fseek(arquivoPrincipal, tuplaPorData.posicao, SEEK_SET);
			fread(&tuplaPorData, 1, sizeof(tpIndexPorData), arquivoIndexPorData);
				fscanf(arquivoPrincipal, "%s", linha);
				tuplaPrincipal=linhaParaStruct(linha);
			if(strcmp(tuplaPrincipal.status, "allocated")==0){
				//ano
				if(compararAno(data, tuplaPrincipal.data) && strcmp(tipoIp, tuplaPrincipal.tipoIp)==0 && strcmp(tipoData, "ano")==0){
					printaData(tuplaPrincipal.data);
					printf("    [%s]\t[%s]\t[%s]\n",tuplaPrincipal.tipoIp, tuplaPrincipal.status, tuplaPrincipal.ip);
					quantidade++;
				}

				//mes
				else if(compararMes(data, tuplaPrincipal.data) && strcmp(tipoIp, tuplaPrincipal.tipoIp)==0 && strcmp(tipoData, "mes")==0){
					printaData(tuplaPrincipal.data);
					printf("    [%s]\t[%s]\t[%s]\n",tuplaPrincipal.tipoIp, tuplaPrincipal.status, tuplaPrincipal.ip);
					quantidade++;
				}

				//ano e mes
				else if(compararMesAno(data, tuplaPrincipal.data) && strcmp(tipoIp, tuplaPrincipal.tipoIp)==0 && strcmp(tipoData, "anomes")==0){
					printaData(tuplaPrincipal.data);
					printf("    [%s]\t[%s]\t[%s]\n",tuplaPrincipal.tipoIp, tuplaPrincipal.status, tuplaPrincipal.ip);
					quantidade++;
				}
				
			}

			
		}
	}
	return quantidade;
}

//função destinada para a criação dos indices, abertura unica do arquivo principal para a criação dos demais.
void indexador(){

tpTuplaPrincipal tuplaPrincipal;

FILE *arquivoPrincipal, 
	 *arquivoIndexPais,
	 *arquivoIndexIpv4,
	 *arquivoIndexIpv6,
	 *arquivoIndexAsn,
	 *arquivoIndexPorData;

char linha[MAX_STR];
long int posicaoLinha;

	arquivoIndexPais=fopen("../data/indexadores/indexPorPais.bin", "wb");
	arquivoIndexIpv4=fopen("../data/indexadores/indexIpv4.bin", "wb");
	arquivoIndexIpv6=fopen("../data/indexadores/indexIpv6.bin", "wb");
	arquivoIndexAsn=fopen("../data/indexadores/indexAsn.bin", "wb");
	arquivoIndexPorData=fopen("../data/indexadores/indexPorData.bin", "wb");

	if((arquivoPrincipal=fopen("../data/delegated-lacnic-extended-20170903", "r"))==NULL){
		printf("Erro ao abrir o arquivo");

	}
	else{
		while(!feof(arquivoPrincipal)){
	
			//enquadramento da linha da base original para tupla
			posicaoLinha = ftell(arquivoPrincipal);
			fscanf(arquivoPrincipal, "%s", linha);
			tuplaPrincipal = linhaParaStruct(linha);
			tuplaPrincipal.posicaoLinha = posicaoLinha;

		  	//---INDEXADORES---
			indexarPorPais(tuplaPrincipal, arquivoIndexPais);
			indexarPorTipo(tuplaPrincipal, arquivoIndexIpv4, arquivoIndexIpv6, arquivoIndexAsn);
			indexarPorData(tuplaPrincipal, arquivoIndexPorData);
		}

		fclose(arquivoIndexPais);
		fclose(arquivoPrincipal);
		fclose(arquivoIndexIpv4);
		fclose(arquivoIndexIpv6);
		fclose(arquivoIndexAsn);
		fclose(arquivoIndexPorData);
	}

}

long int ipPorStatus(char * status, char * tipoIp, char *nomeArquivoIndexPorIp){
	char linha[200];
	tpIndexTipoIp tuplaIndexTipoIp;
	tpTuplaPrincipal tuplaPrincipal;
	FILE *arquivoIndexIp;
	FILE *arquivoPrincipal;
	int quantidade=0;
	long int quantidadeTotal=0;

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
				printf("[%s]\t[%s]\t[%s]  [%s]\n", tuplaPrincipal.tipoIp, tuplaPrincipal.ip , tuplaPrincipal.status, tuplaPrincipal.ipQuantity);
				quantidade++;
				if (strcmp(tipoIp, "ipv6")==0)
				{
					quantidadeTotal += quantidadeIpv6(tuplaPrincipal.ipQuantity);
				}
				else{
					quantidadeTotal += atoi(tuplaPrincipal.ipQuantity);	
				}
			}
		}

		fclose(arquivoIndexIp);
		fclose(arquivoPrincipal);
	}
	printf("[TIPO]\t[ENDEREÇOS]\t[STATUS]     [QUANTIDADE]\n");
	printf("  ''   \t   %d     \t   ''        %ld",quantidade, quantidadeTotal);
	return quantidadeTotal;
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
			printf("\n---\nQuantidade de %s correspondentes: %d\n", argv[2], contarQuantidadeTipoIp(nomeArquivoIndexPorIp));
		}

		else if(strcmp(argv[1], "quantidade")==0 && ((strcmp(argv[3], "alocados")==0)||(strcmp(argv[3], "disponiveis")==0)||(strcmp(argv[3], "reservados")==0))){
			printf("\n--\t--\t--\t--\t--\n[TOTAL DE %ld endereços %s %s]\n", ipPorStatus(argv[3], argv[2], nomeArquivoIndexPorIp),argv[2], argv[3]);
		}
	
		else{
			printf("\n[ERRO] Argumento/s invalido/s\n");
		}

}

char * buscarPorDataAlocacao(char *tipoIp, char *ip){

		FILE * arquivoIndexIp, *arquivoPrincipal;
		tpIndexTipoIp tuplaTipoIp;
		tpTuplaPrincipal tuplaPrincipal;
		char linha[MAX_STR];
		if((arquivoIndexIp=fopen(caminhoDiretorioArquivo(tipoIp), "rb"))==NULL){
			erro(407, "Abertura de arquivo");
		}
		else if((arquivoPrincipal=fopen(caminhoDiretorioArquivo("principal"), "r"))==NULL){
			erro(407, "Arquivo principal");
		}
		
		else{
			while(!feof(arquivoIndexIp)){
				fread(&tuplaTipoIp, 1, sizeof(tpIndexTipoIp), arquivoIndexIp);
				fseek(arquivoPrincipal, tuplaTipoIp.posicao, SEEK_SET);
				fscanf(arquivoPrincipal, "%s", linha);
				if(strstr(linha, ip)!=NULL)
					break;
			}
		
			fclose(arquivoPrincipal);
			fclose(arquivoIndexIp);

			tuplaPrincipal = linhaParaStruct(linha);

			if(strcmp(tuplaPrincipal.ip,ip)==0 && strcmp(tuplaPrincipal.status,"allocated")==0){
				printaData(tuplaPrincipal.data);
				return "\n";
			}
			else{
				return "Endereço não encontrado ou não é um endereço alocado\n";
				}

		}
}

void indexarPaisesOrdenados(){
	FILE *indexPorPais, *indexOrdenar;
	tpIndexPais tuplaPais[MAX_VET];

	if((indexOrdenar=fopen(caminhoDiretorioArquivo("paisesOrdenados"), "wb"))==NULL){
			erro(407, "Criação do arquivo paisesOrdenados");
	}
	else if((indexPorPais=fopen(caminhoDiretorioArquivo("indexPorPais"), "rb"))==NULL){
			erro(407, "Arquivo principal");
	}
	else{
		int i,j, tamanhoVetor, gap;
		char temp[3];

		for(i=0;!feof(indexPorPais);i++)
			fread(&tuplaPais[i], 1, sizeof(tpIndexPais), indexPorPais);
		
		tamanhoVetor = --i;
	
		for(gap=1;gap<tamanhoVetor;gap=3*gap+1)
		;

		while(gap>1){
			gap/=3;
			for (i = gap; i < tamanhoVetor; ++i)
			{
				strcpy(temp, tuplaPais[i].pais);
				j=i-gap;
				while(j>=0 && strcmp(temp, tuplaPais[j].pais)<0){
					strcpy(tuplaPais[j+gap].pais, tuplaPais[j].pais);
					j-=gap;
				}
				strcpy(tuplaPais[j+gap].pais, temp);
			}
		}

		for(i=0;i<tamanhoVetor;i++){
			fwrite(&tuplaPais[i], sizeof(tpIndexPais), 1, indexOrdenar);
		}

		fclose(indexPorPais);
		fclose(indexOrdenar);

	}	
}

void rankingPorIp(char *tipoIp){

	FILE *arquivoPrincipal, *indexOrdenados;
	tpIndexPais tuplaPaisesOrdenados;
	tpRankingPais rankingPais[50], auxVet[50], aux;
	tpTuplaPrincipal tuplaPrincipal;
	int i,j, tamanhoVetor;
	char linha[150];

	if((indexOrdenados=fopen(caminhoDiretorioArquivo("paisesOrdenados"), "rb"))==NULL){
		erro(407, "Abertura do arquivo paisesOrdenados");
	}
	else if((arquivoPrincipal=fopen(caminhoDiretorioArquivo("principal"), "r"))==NULL){
		erro(407, "Arquivo principal");
	}
	else{
		for(i=0;!feof(indexOrdenados);){
			fread(&tuplaPaisesOrdenados, 1, sizeof(tpIndexPais), indexOrdenados);	

			//Um pais de cada, em ordem alfabetica
			if(strcmp(rankingPais[i].pais, tuplaPaisesOrdenados.pais)!=0){
				strcpy(rankingPais[++i].pais, tuplaPaisesOrdenados.pais);
				rankingPais[i].quantidade = 0;
			}
		}
		tamanhoVetor = i;
		
		while(!feof(arquivoPrincipal))
		{
			
			fscanf(arquivoPrincipal, "%s", linha);
			tuplaPrincipal = linhaParaStruct(linha);
			
			for(i=1;i<=tamanhoVetor;i++){
				if((strcmp(rankingPais[i].pais, tuplaPrincipal.pais)==0) && (strcmp(tuplaPrincipal.tipoIp, tipoIp)==0)){
					if(strcmp(tipoIp, "ipv6")==0)
						rankingPais[i].quantidade+=quantidadeIpv6(tuplaPrincipal.ipQuantity);
					else{
					rankingPais[i].quantidade += atoi(tuplaPrincipal.ipQuantity);
					}
				}

			}

		}

		//Enquandrando vetor e variaveis para o algoritimo
		for(i=0;i<tamanhoVetor;i++)
			auxVet[i]=rankingPais[i+1];
		tamanhoVetor--;
		//Algoritimo de ordenacao (insertion)
		for (j = 1; j <= tamanhoVetor; ++j) {
      	aux = auxVet[j];
      		for (i = j-1; i >= 0 && auxVet[i].quantidade > aux.quantidade; --i) 
        	 	auxVet[i+1]= auxVet[i];
      	    auxVet[i+1] = aux;
   		}

		printf("[Ranking]     [Pais]\t\t[Quantidade]\n");
		for(i=tamanhoVetor, j=1;i>=0;i--,j++)
			printf("   %d°\t\t%s    \t\t%ld\n", j, auxVet[i].pais, auxVet[i].quantidade);

	}
}

void zerarTudo(){
	remove("../data/indexadores/indexIpv4.bin");
	remove("../data/indexadores/indexIpv6.bin");
	remove("../data/indexadores/indexAsn.bin");
	remove("../data/indexadores/indexPorData.bin");
	remove("../data/indexadores/paisesOrdenados.bin");
	remove("../data/indexadores/indexPorPais.bin");
	remove("../data/delegated-lacnic-extended-20170903");
}

int main(int argc, char *argv[]){

	if(argv[1]==NULL){
		erro(1, "É esperado um comando, ./ajuda para mais informações");
		return 0;
	}

	else if(strcmp(argv[1], "indexar") == 0){
		printf("\n---\n[INDEXANDO...]");
		indexador();
		indexarPaisesOrdenados();
		printf("\nTodos os indices foram criados com sucesso!\n---\n");
	}
	else if (strcmp(argv[1], "quantidade")==0)
	{
		contar(argv);
	}

	else if(strcmp(argv[1], "pordata")==0){
		printf("%s", buscarPorDataAlocacao(argv[2], argv[3]));
	}
	else if(strcmp(argv[1], "ordenarPaises")==0){
		indexarPaisesOrdenados();
	}

	else if(strcmp(argv[1], "ranking")==0){
		rankingPorIp(argv[2]);
	}
	else if(strcmp(argv[1], "recursos")==0){
		printf("[TOTAL DE: %d %s ALOCADOS EM %s]\n]", quantidadeRecursos(argv[2], argv[3], argv[4]),argv[2],argv[4]);
	}
	else if(strcmp(argv[1], "-*zerar")==0){
		char c;
		printf("[AVISO] Tem certeza que deseja apagar toda a base de dado e os índices? [s/n]: ");
		scanf("%c", &c);
		if(c=='s'){
		printf("Apagando base de dados e arquivos de índices...\n");

		zerarTudo();
		printf("Base de dados e índices apagados com sucesso!\n");
		}
		else printf("Abortado!\n.");
	}

	else{
		printf("[ERRO] Comando Inválido, ./ajuda para mais informações\n");
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
s) Mostrar a data de alocação de um ASN;
t) Mostrar a data de alocação de um bloco IPv4;
u) Mostrar a data de alocação de um bloco IPv6;
j) Imprimir o ranking de ASN por pais em ordem decrescente;
k) Imprimir o ranking da quantidade de IPv4 por pais em ordem decrescente;
l) Imprimir o ranking da quantidade de IPv6 por pais em ordem decrescente; (fórmula)
v) Mostrar a quantidade de recursos (ASN/IPv4/IPv6) alocados em um ano e/ou mês específico;

FAZENDO
z) Zerar a base de dados (e arquivos de índices);


A FAZER






*/