#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "listas.h"
#include "serverfuncs.h"

/* SOCKET ------------------------- */
#include <netdb.h>
#include <arpa/inet.h>

typedef struct sockaddr_in SockAddr_in;
typedef struct sockaddr SockAddr;

/* -------------------------- funcoes de recebimento e envio de informacoes -------------------------- */

/* funcao de comunicação entre cliente-servidor [gerencia as 8 operações] */
void comunicacao(int socketFD)
{
	SockAddr_in client;
	int len = sizeof(client);
	
    for (;;)
    {
		char RBuffer[200];
		memset(RBuffer, 0, 200);
		recvfrom(socketFD, RBuffer, 200, 0, (SockAddr*)&client, &len);
    
		if(!strcmp(RBuffer,"ENCERRAR"))
			break;
		else
		{
			printf("mensagem: %s",RBuffer);
		}
    }
}

void getServerIP() // mostra ip da maquina (apenas para facilitar testes)
{
	system("curl --output index.txt --url https://icanhazip.com");
	system("clear");
	FILE *iptxt = fopen("index.txt","r");
	if(iptxt != NULL)
	{
		char myip[16];
		fscanf(iptxt,"%s",myip);
		fclose(iptxt);
		remove("index.txt");
		PRINT2C("IP do servidor: ",CLW,CLY,"<< %s >> \n", myip);
	}
}
int getServerInfo(char *ip) // le arquivo de configuracao do servidor
{
	// config [le ip e porta do arquivo]
	FILE *config = fopen(CONFIG,"r");
	int port = 9000;
	char str[12];

	if(config != NULL)
	{
		fscanf(config,"%s",ip);
		fscanf(config,"%s",str);
		port = (int)strtol(str,NULL,10);
		fclose(config);
	}
	PRINT2C("porta do servidor: ",CLW,CLY,"<< %d >> \n", port);
	return port;
}
/* ----------------------------------------------------- MAIN ------------------------------------------------------ */
int main()
{
	// apenas para ver o ip da maquina (servidor)
	//getServerIP();

	// le ip e porta definidos no arquivo de configuracao
	char ip[16] = "0.0.0.0";        // valor (default)
	int port = getServerInfo(ip);   // valor default é 9000
	
	// SOCKET
	int socketFD = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // cria file descriptor do socket UDP
	SockAddr_in server;												   // estrutura do socket
	socketLOG("SBL", socketFD, "socket ", ""); 		   // verifica criação do socket

	/* atribui valores à estrutura do socket */
	server.sin_family = AF_INET;            // AF_INET é a familia de protocolos do IPV4
	server.sin_addr.s_addr = inet_addr(ip); // 0.0.0.0 (qualquer ip na rede pode enviar ao servidor)
	server.sin_port = htons(port);          // porta do servidor

	int do_bind = bind(socketFD, (SockAddr*)&server, sizeof(server)); // vincula um nome ao socket
	socketLOG("SBL", do_bind, "bind   ", ""); 			  // verifica bind
	comunicacao(socketFD);
	
    return 0;
}
