#include "../headers/indexador.h"


void indexPorPais(char * linha, long int posicaoLinha, FILE * arquivoPorPais){

	if(linha[7]=='|')
		fprintf(arquivoPorPais, "00|%ld\n", posicaoLinha);
	else{
		fprintf(arquivoPorPais, "%c%c|%ld\n",linha[7], linha[8], posicaoLinha);
	}


}

void indexTiposXdisponibilidade(char * linha, long int posicaoLinha, FILE *arquivoIpv4, FILE *arquivoIpv6, FILE *arquivoAsn){
	if (strstr(linha, "ipv4"))
	{
		fprintf(arquivoIpv4, "%ld ", posicaoLinha);
	}
	else if (strstr(linha, "ipv6"))
	{
		fprintf(arquivoIpv6, "%ld ", posicaoLinha);
	}
	else if (strstr(linha, "asn"))
	{
		fprintf(arquivoAsn, "%ld ", posicaoLinha);
	}

	else{
		printf("Erro ao fazer a indexacao dos ips por disponibilidade, favor verificar o arquivo matriz");
	}

}

void indexador(){

FILE *arquivoPrincipal, 
	 *arquivoPorPais, 
	 *arquivoIpv4Disponiveis, 
	 *arquivoIpv6Disponiveis, 
	 *arquivoAsnDisponiveis;

char linha[60];
long int posicaoLinha;

	arquivoPorPais=fopen("../data/indexadores/indexPorPais", "wt");
	arquivoIpv4Disponiveis=fopen("../data/indexadores/indexIpv4Xdisponibilidade", "wt");
	arquivoIpv6Disponiveis=fopen("../data/indexadores/indexIpv6Xdisponibilidade", "wt");
	arquivoAsnDisponiveis=fopen("../data/indexadores/indexAsnXdisponibilidade", "wt");

	if((arquivoPrincipal=fopen("../data/delegated-lacnic-extended-20170903", "r"))==NULL){
		printf("Erro ao abrir o arquivo");

	}
	else{
		while(!feof(arquivoPrincipal)){
			
			//a linha possui os dados do arquivo, posicaoLinha é a sua posição em bytes a partir do seek_set
			posicaoLinha = ftell(arquivoPrincipal);
			fscanf(arquivoPrincipal, "%s", linha);

			//funções para indexadores, sempre serão passada a linha e a posição (long int)
		    //o objetivo é usar a mesma linha para fazer todos os indexadores primarios
			indexPorPais(linha, posicaoLinha, arquivoPorPais);
			indexTiposXdisponibilidade(linha, posicaoLinha, arquivoIpv4Disponiveis, arquivoIpv6Disponiveis, arquivoAsnDisponiveis);
		}

		fclose(arquivoPorPais);

		fclose(arquivoIpv4Disponiveis);
		fclose(arquivoIpv6Disponiveis);
    	fclose(arquivoAsnDisponiveis);
	}


}



int main(int argv, char *argc[]){


	indexador();
	
	return 0;
}