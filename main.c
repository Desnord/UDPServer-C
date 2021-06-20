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

#define TEMP01 "tempfile01.txt"
#define TEMP02 "tempfile02.txt"

/* -------------------------- funcoes de recebimento e envio de informacoes -------------------------- */

void recebeMSG(int socketFD) // recebimento da mensagem do cliente (gera arquivo temporario com as informacoes)
{
    SockAddr_in client;
    int len = sizeof(client);
    FILE *temp = fopen(TEMP01, "a");
    for(;;)
    {
			char RBuffer[200];
			memset(RBuffer,'\0',200);
      recvfrom(socketFD, RBuffer, 200, 0, (SockAddr*)&client, &len);

      if (strcmp(RBuffer, "EOF") == 0)
          break;
			fputs(RBuffer,temp);
    }
    fclose(temp);
}

char *interpretaMSG() // interpreta mensagem recebida
{
    FILE *temp = fopen(TEMP01, "r+");
    char *opcao = malloc(10*sizeof(char));
		memset(opcao,'\0',10);

    fgets(opcao,10,temp);
    opcao[strlen(opcao)-1] = '\0';

    if(!strcmp(opcao,"1") || !strcmp(opcao,"2"))
    {
        // curso ou habilidade
        char str[100];
        fgets(str,100,temp);
        str[strlen(str)-1] = '\0';

        NoPerfilEmailNome *lista;

        if(!strcmp(opcao,"1"))
            lista = listarPorFormacao(str);
        else
            lista = listarPorHabilidade(str);

        FILE *temp2 = fopen(TEMP02, "w");

        if(lista->prox == NULL)
            fprintf(temp2,"%s\n", "NULL");

        for(NoPerfilEmailNome *at = lista->prox; at != NULL; at = at->prox)
        {
            fprintf(temp2,"%s\n", at->perfil->email);     // armazena email
            fprintf(temp2,"%s\n", at->perfil->nome);      // armazena nome
            fprintf(temp2,"%s\n", at->perfil->sobrenome); // armazena sobrenome
            fprintf(temp2,"%s\n", "+=====+");
        }
        fclose(temp2);
        NPENListFree(lista);
    }
    else if(!strcmp(opcao,"3"))
    {
        char str[100];
        fgets(str,100,temp);
        str[strlen(str)-1] = '\0';

        int ano = (int)strtol(str,NULL,10);

        NoPerfilEmailNomeCurso *lista = listarPorAno(ano);

        FILE *temp2 = fopen(TEMP02, "w");

        if(lista->prox == NULL)
            fprintf(temp2,"%s\n", "NULL");

        for(NoPerfilEmailNomeCurso *at = lista->prox; at != NULL; at = at->prox)
        {
            fprintf(temp2,"%s\n", at->perfil->email);     // armazena email
            fprintf(temp2,"%s\n", at->perfil->nome);      // armazena nome
            fprintf(temp2,"%s\n", at->perfil->sobrenome); // armazena sobrenome
            fprintf(temp2,"%s\n", at->perfil->formacao);  // armazena curso
            fprintf(temp2,"%s\n", "+=====+");
        }
        fclose(temp2);
        NPENCListFree(lista);
    }
    else if(!strcmp(opcao,"4"))
    {
        NoPerfil *lista = listarTodos();

        FILE *temp2 = fopen(TEMP02, "w");

        if(lista->prox == NULL)
            fprintf(temp2,"%s\n", "NULL");

        for(NoPerfil *at = lista->prox; at != NULL; at = at->prox)
        {
            fprintf(temp2,"%s\n", at->pessoa->email);              // armazena email
            fprintf(temp2,"%s\n", at->pessoa->nome);               // armazena nome
            fprintf(temp2,"%s\n", at->pessoa->sobrenome);          // armazena sobrenome
            fprintf(temp2,"%s\n", at->pessoa->cidade_residencia);  // armazena residencia
            fprintf(temp2,"%s\n", at->pessoa->formacao);           // armazena formacao academica
            fprintf(temp2,"%d\n", at->pessoa->ano_formatura);      // armazena ano de formatura
            fprintf(temp2,"%s\n", "+=====+");
            for(NoString *at2 = at->pessoa->habilidades->prox; at2 != NULL; at2 = at2->prox)
                fprintf(temp2,"%s\n", at2->str);
            fprintf(temp2,"%s\n", "+=====+");
            for(NoString *at2 = at->pessoa->experiencia->prox; at2 != NULL; at2 = at2->prox)
                fprintf(temp2,"%s\n", at2->str);
            fprintf(temp2,"%s\n", "+=====+");
        }
        fclose(temp2);
        perfilListFree(lista);
    }
    else if(!strcmp(opcao,"5"))
    {
        // email
        char str[100];
        fgets(str,100,temp);
        str[strlen(str)-1] = '\0';

        NoPerfil *lista = listarTodos();
        Perfil *p = encontrarPerfil(str,lista);

        FILE *temp2 = fopen(TEMP02,"w");
        if(p != NULL)
        {
            fprintf(temp2,"%s\n", p->email);                // armazena email
            fprintf(temp2,"%s\n", p->nome);                 // armazena nome
            fprintf(temp2,"%s\n", p->sobrenome);            // armazena sobrenome
            fprintf(temp2,"%s\n", p->cidade_residencia);    // armazena residencia
            fprintf(temp2,"%s\n", p->formacao);             // armazena formacao academica
            fprintf(temp2,"%d\n", p->ano_formatura);        // armazena ano de formatura
            fprintf(temp2,"%s\n", "+=====+");

            for(NoString *at = p->habilidades->prox; at != NULL; at = at->prox)
                fprintf(temp2,"%s\n", at->str);
            fprintf(temp2,"%s\n", "+=====+");
            for(NoString *at = p->experiencia->prox; at != NULL; at = at->prox)
                fprintf(temp2,"%s\n", at->str);
            fprintf(temp2,"%s\n", "+=====+");
        }
        else
        {
            fprintf(temp2,"%s\n", "NULL");
        }

        perfilListFree(lista);
        fclose(temp2);
    }
    else if(!strcmp(opcao,"6"))
    {
        Perfil *p = (Perfil*)malloc(sizeof(Perfil));
        p->experiencia = (NoString *)malloc(sizeof(NoString));
        p->habilidades = (NoString *)malloc(sizeof(NoString));
        p->habilidades->str = NULL;
        p->habilidades->prox = NULL;
        p->experiencia->str = NULL;
        p->experiencia->prox = NULL;

        int ctd = 0;
        char line[200];
        memset(line, '\0', 200);
        while(fgets(line,200,temp))
        {
            line[(int)strlen(line)-1] = '\0';
            if(ctd < 5)
            {
                char *campo = malloc(sizeof(char) * ((int)strlen(line)+1));

                if(!ctd)
                    p->email = campo;
                else if(ctd == 1)
                    p->nome = campo;
                else if(ctd == 2)
                    p->sobrenome = campo;
                else if(ctd == 3)
                    p->cidade_residencia = campo;
                else if(ctd == 4)
                    p->formacao = campo;

                strcpy(campo,line);
                ctd++;
            }
            else if(ctd == 5)
            {
                p->ano_formatura = (int)strtol(line,NULL,10);
                ctd++;
            }
            else if(ctd < 8)
            {
                NoString *STRlist =  p->habilidades;
                if(!strcmp(line,"+=====+"))
                    ctd++;
                else
                {
                    char *strAUX = malloc(sizeof(char) * ((int)strlen(line)+1));
                    strcpy(strAUX, line);

                    // insere na lista de habilidades/experiencia
                    stringListInsert(STRlist, strAUX);
                }
            }
            else
            {
                NoString *STRlist = p->experiencia;
                if(!strcmp(line,"+=====+"))
                    ctd++;
                else
                {
                    char *strAUX = malloc(sizeof(char) * ((int)strlen(line)+1));
                    strcpy(strAUX, line);

                    // insere na lista de habilidades/experiencia
                    stringListInsert(STRlist, strAUX);
                }
            }
        }
        int res = addPerfil(p);
        FILE *temp2 = fopen(TEMP02,"w");
        fprintf(temp2,"%d\n", res);
        fclose(temp2);
    }
    else if(!strcmp(opcao,"7"))
    {
        // email
        char eml[100];
        fgets(eml,100,temp);
				eml[(int)strlen(eml)-1] = '\0';

        // experiencia
        char exp[100];
        fgets(exp,100,temp);
				exp[(int)strlen(exp)-1] = '\0';

        // tenta adicionar experiencia.
        int res = addExperiencia(eml,exp);

        FILE *temp2 = fopen(TEMP02,"w");
        fprintf(temp2,"%d\n", res);
        fclose(temp2);
    }
    else if(!strcmp(opcao,"8"))
    {
        // email
        char str[100];
        fgets(str,100,temp);
        str[strlen(str)-1] = '\0';

        // tenta remover o perfil.
        int res = removerPerfil(str);

        FILE *temp2 = fopen(TEMP02,"w");
        fprintf(temp2,"%d\n", res);
        fclose(temp2);
    }

    fclose(temp);
    remove(TEMP01);
    return opcao;
}

// envio dos dados do arquivo temporario ao cliente
void enviaMSG(int socketFD)
{
    SockAddr_in client;

    FILE *temp = fopen(TEMP02, "r");
    char line[200];
		memset(line,'\0',200);

    while(fgets( line, 200, temp) != NULL)
		{
			//printf("%s",line);
      sendto(socketFD, line, 200, 0, (SockAddr*)&client, sizeof(client));
		}

    strcpy(line,"EOF");
    sendto(socketFD,  line, 200, 0, (SockAddr*)&client, sizeof(client));

    fclose(temp);
    remove(TEMP02);
}

// escreve o que ocorreu no log do servidor
void logMSG()
{

}

/* funcao de comunicação entre cliente-servidor [gerencia as 8 operações] */
void comunicacao(int socketFD)
{
    for (;;)
    {
			recebeMSG(socketFD);
			char *opt = interpretaMSG();

			if(!strcmp(opt,"ENCERRAR"))
      {
				free(opt);
				break;
			}

			free(opt);
			enviaMSG(socketFD);

			//logMSG();
			//printFB("OP", 0, inet_ntoa(client.sin_addr), RBuffer);
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
	socketLOG("SB", socketFD, "socket ", ""); 		   // verifica criação do socket

	/* atribui valores à estrutura do socket */
	server.sin_family = AF_INET;            // AF_INET é a familia de protocolos do IPV4
	server.sin_addr.s_addr = inet_addr(ip); // 0.0.0.0 (qualquer ip na rede pode enviar ao servidor)
	server.sin_port = htons(port);          // porta do servidor

	int do_bind = bind(socketFD, (SockAddr*)&server, sizeof(server)); // vincula um nome ao socket
	socketLOG("SB", do_bind, "bind   ", ""); 			  // verifica bind
	comunicacao(socketFD);

  return 0;
}
