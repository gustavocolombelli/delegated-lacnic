#ifndef FERRAMENTAS_FILE
#define FERRAMENTAS_FILE

#include <stdio.h>
#include <string.h>


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

}

#endif