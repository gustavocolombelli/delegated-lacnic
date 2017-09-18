#include "../headers/pesquisa.h"


char * procuraLinhaArquivoPrincipal(long int posicao){

	FILE * arquivoPrincipal;

	char linha[60];

	if((arquivoPrincipal=fopen("../data/delegated-lacnic-extended-20170903", "r"))==NULL){
		printf("Erro ao abrir o arquivo");

	}
	else{
		while(!feof(arquivoPrincipal)){
			fseek(arquivoPrincipal, posicao, SEEK_SET);
			fscanf(arquivoPrincipal, "%s", linha);
		}
		fclose(arquivoPrincipal);
	}
	return linha;
}

int main(int argv, char * argc[]){


}