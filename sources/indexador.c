#include "../headers/indexador.h"


void indexPorPais(char * linha, long int posicaoLinha, FILE * arquivoPorPais){
//	printf("%ld ", posicaoLinha);
//	puts(linha);
	if(linha[7]=='|')
		fprintf(arquivoPorPais, "00|%ld\n", posicaoLinha);
	else{
		fprintf(arquivoPorPais, "%c%c|%ld\n",linha[7], linha[8], posicaoLinha);
	}


}

void indexador(){

FILE * arquivoPrincipal, *arquivoPorPais;
char linha[60];
long int posicaoLinha;

	arquivoPorPais=fopen("../data/indexadores/indexPorPais", "wt");

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

		}
	}


}



int main(int argv, char *argc[]){


	indexador();
	
	return 0;
}