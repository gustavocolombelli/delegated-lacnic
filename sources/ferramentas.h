#ifndef FERRAMENTAS_FILE
#define FERRAMENTAS_FILE

#include <stdio.h>
#include <string.h>
#include <math.h>

char * traduzir(char *argv){
	if(strcmp(argv, "alocados")==0){
		return "allocated";
	}

	else if(strcmp(argv, "reservados")==0){
		return "reserved";
	}

	else if(strcmp(argv, "disponiveis")==0){
		return "available";
	}
}


void erro(int numErro, char *msgErro){
	switch(numErro){
		case 1: printf("[ERRO %d] Comando %s não identificado.\n", numErro, msgErro);
				break;
		case 2: printf("[ERRO %d] Argumento %s não identificado.\n", numErro, msgErro);
				break;
		case 3: printf("[ERRO %d] O arquivo %s não pode ser aberto para a leitura.\n", numErro, msgErro);
				break;
		case 4: printf("[ERRO %d] O arquivo %s não pode ser aberto para a escrita.\n", numErro, msgErro);
				break;
		case 5: printf("[ERRO %d] O arquivo %s não pode ser criado.\n", numErro, msgErro);
				break;
	}
}

char * caminhoDiretorioArquivo(char *argv){
		char nomeArquivoIndexPorIp[60];
		
		if (strcmp(argv,"ipv4")==0){
			return "../data/indexadores/indexIpv4.bin";
		}
		else if(strcmp(argv,"ipv6")==0){
			return "../data/indexadores/indexAsn.bin";
		}
		else if(strcmp(argv,"indexPorPais")==0){
			return "../data/indexadores/indexPorPais.bin";
		}
		else if(strcmp(argv,"paisesOrdenados")==0){
			return "../data/indexadores/paisesOrdenados.bin";
		}
		else if(strcmp(argv,"principal")==0){
			return "../data/delegated-lacnic-extended-20170903";
		}
		else if(strcmp(argv,"indexPorData")==0){
			return "../data/indexPorData.bin";
		}
		else{
			return NULL;
		}
	}

void printaData(char *data8dig){
	printf("%c%c/%c%c/%c%c%c%c", data8dig[6],data8dig[7],
								 data8dig[4], data8dig[5],
								 data8dig[0],data8dig[1], data8dig[2], data8dig[3]);

}

long int tamanhoArquivo(void *tpStruct){
	return 1;
}

long int quantidadeIpv6(char *ipv6){
	int quantidade = atoi(ipv6);
	return (pow(2, (64-quantidade)));
}

int compararAno(char *data, char *dataInformada){
	char ano[5];
	ano[0]=dataInformada[0];
	ano[1]=dataInformada[1];
	ano[2]=dataInformada[2];
	ano[3]=dataInformada[3];
	ano[4]= '\n';

	if(strstr(ano, data)!=NULL){
		return 1;
		}
	else return 0;
}		

int compararMes(char *data, char *dataInformada){
	char ano[3];
	ano[0]=dataInformada[4];
	ano[1]=dataInformada[5];
	ano[2]='\n';

	if(strstr(ano, data)!=NULL){
		return 1;
		}
	else return 0;
}

int compararMesAno(char *data, char *dataInformada){
	char ano[7];
	ano[0]=dataInformada[4];
	ano[1]=dataInformada[5];
	ano[2]=dataInformada[0];
	ano[3]=dataInformada[1];
	ano[4]=dataInformada[2];
	ano[5]=dataInformada[3];
	ano[6]='\n';
	if(strstr(ano, data)!=NULL){
		return 1;
		}
	else return 0;
}

#endif