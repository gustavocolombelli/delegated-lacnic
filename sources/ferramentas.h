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
	printf("[ERRO %d] %s\n", numErro, msgErro);
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
	printf("Data da alocação: %c%c/%c%c/%c%c%c%c", data8dig[6],data8dig[7],
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


#endif