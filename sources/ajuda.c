#include "ajuda.h"

int main(int argc, char const *argv[])
{
	printf("---\t---\t---\t---\t---\t---\t---\t\n");
	printf("Indexador e buscador de informacoes delegated-lacnic ips, versao 1.0\nAutor: Gustavo Colombelli Alessio\n\n");
	printf(" [COMANDO]                           |         [ARGS...]\n");	
	printf("index                                |\n");
	printf(" [indexar]                           |\n");
	printf(" [*zerar]                            |\n");
	printf(" [ordenarPaises]                     |\n");
	printf(" [backup]                            |\n");
	printf(" [quantidade, tipoIp]                | tipoIp = (ipv4, ipv6, asn)\n");
	printf(" [quantidade, tipoIp, status]        | status = (disponiveis, alocados, reservados)\n");
	printf(" [pordata, tipoIp, ip]               | \n");		
	printf(" [ranking, tipoIp]                   | tipoData = (ano, mes, anomes) \n");
	printf(" [recursos, tipoIp, tipoData, data]  | data =(mm ou aa ou mmaaaa)\n");
	printf("\n\n");

	return 0;
}