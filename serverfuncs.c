#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include "listas.h"
#include "serverfuncs.h"

/* ----------------------- FUNCOES DE OPERACOES DE DADOS AUXILIARES DO SERVIDOR ---------------------- */
void getServerIP()
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
int getServerInfo(char *ip)
{
    // config [le ip e porta do arquivo]
    FILE *config = fopen("config.txt","r");
    int port = 9000;

    if(config != NULL)
    {
        fscanf(config,"%s",ip);
        fscanf(config,"%d",&port);
        fclose(config);
    }
    PRINT2C("porta do servidor: ",CLW,CLY,"<< %d >> \n", port);
    return port;
}
void atualizaArquivo(NoPerfil *lista)
{
    remove(CADS); // deleta o arquivo existente
    FILE *cadastros = fopen(CADS,"w"); // cria um novo arquivo (será atualizado)

    // percorre a lista de perfis e escreve os dados no arquivos
    NoPerfil *at;
    for(at = lista->prox; at != NULL; at=at->prox)
    {
        // perfil atual
        Perfil *p;
        p = at->pessoa;

        fprintf(cadastros,"Email: %s\n", p->email);                    // armazena email
        fprintf(cadastros,"Nome: %s\n", p->nome);                      // armazena nome
        fprintf(cadastros,"Sobrenome: %s\n",p->sobrenome);             // armazena sobrenome
        fprintf(cadastros,"Residência: %s\n", p->cidade_residencia);   // armazena residencia
        fprintf(cadastros,"Curso: %s\n", p->formacao);                 // armazena formacao academica
        fprintf(cadastros,"Formatura: %d\n", p->ano_formatura);        // armazena ano de formatura

        // escreve lista de habilidades
        fputs("Habilidades:\n",cadastros);
        NoString *strAT;
        for(strAT = p->habilidades->prox; strAT != NULL; strAT = strAT->prox)
            fprintf(cadastros,"%s\n",strAT->str);

        // escreve lista de experiencia
        fputs("Experiência:\n",cadastros);
        for(strAT = p->experiencia->prox; strAT != NULL; strAT = strAT->prox)
            fprintf(cadastros,"%s\n",strAT->str);

        fputs("\n",cadastros);
    }

    fclose(cadastros); // fecha arquivo
}
void printFB(char *x, int code, char *val, ...)
{
    va_list ap;
    va_start(ap, val);

    char *aux = val;

    if(!strcmp(x,"SB"))
    {
        PRINTCL(CLW,"\u2192 %s ",aux);
        if(code == 0)
            PRINTCL(CLLR,"FAIL. \n"),getchar(),va_end(ap),exit(0);
        else
            PRINTCL(CLLG,"OK. \n");
    }
    else if(!strcmp(x,"OP"))
    {
        PRINTCL(CLLB,"\u2192 (%s)",aux);
        PRINTCL(CLW," operação escolhida pelo cliente: ");
        aux = va_arg(ap, char*);
        PRINTCL(CLLP,"%s\n",aux);
    }
    else if(!strcmp(x,"6"))
    {
        if (code == 0)
        {
            PRINTCL(CLLR, "\u2192 (%s)", aux);
            PRINTCL(CLW, " perfil não inserido [email duplicado]. \n");
        }
        else
        {
            PRINTCL(CLLB, "\u2192 (%s)", aux);
            PRINTCL(CLW, " perfil inserido. \n");
        }
    }
    else if(!strcmp(x,"8"))
    {
        if (code == 0)
        {
            PRINTCL(CLLR, "\u2192 (%s)", aux);
            PRINTCL(CLW, " perfil não removido [inexistente]. \n");
        }
        else
        {
            PRINTCL(CLLB, "\u2192 (%s)", aux);
            PRINTCL(CLW, " perfil removido. ");
            aux = va_arg(ap, char*);
            PRINTCL(CLLP,"[email: %s]\n",aux);
        }
    }
    va_end(ap);
}
/* ----------------------- FUNCOES DE OPERACOES DE DADOS PRINCIPAIS DO SERVIDOR ---------------------- */
NoPerfil *listarTodos()
{
    // inicializa lista, alocando memoria
    NoPerfil *listaGeral;
    listaGeral = (NoPerfil*)malloc(sizeof(NoPerfil));
    listaGeral->pessoa = NULL;
    listaGeral->prox = NULL;

    // Abre arquivo de cadastros
    FILE *cadastros = fopen(CADS,"r");

    // variaveis auxiliares
    char line[200];
    int count_data = 0;
    int flag = 0;

    Perfil *p;
    p = NULL;
    NoString *auxexp,*auxhab;

    while(fgets(line,200,cadastros))
    {
        if(count_data < 5)
        {
            // inicio da leitura de um perfil, logo aloca memoria para o perfil atual
            if(!count_data)
                p = (Perfil*)malloc(sizeof(Perfil));

            line[strlen(line)-1] = '\0';

            char *campo = malloc(sizeof(char) * (strlen(line)+1));

            if(!count_data)
                p->email = campo;
            else if(count_data == 1)
                p->nome = campo;
            else if(count_data == 2)
                p->sobrenome = campo;
            else if(count_data == 3)
                p->cidade_residencia = campo;
            else if(count_data == 4)
                p->formacao = campo;

            strcpy(campo,line);
            count_data++;
        }
        else if(count_data == 5)
        {
            line[strlen(line)-1] = '\0';
            p->ano_formatura = (int)strtol(line,NULL,10);
        }
        else if(count_data < 9)
        {
            line[strlen(line)-1] = '\0';

            if(count_data == 6 && !strcmp(line,"+=========+"))
            {
                auxhab = (NoString *)malloc(sizeof(NoString));
                auxhab->str = NULL;
                auxhab->prox = NULL;
                count_data++;
            }
            else if(count_data == 7 && !strcmp(line,"+=========+"))
            {
                auxexp = (NoString *)malloc(sizeof(NoString));
                auxexp->prox = NULL;
                auxexp->str = NULL;
                count_data++;
            }
            else if(count_data == 8 && !strcmp(line,"+=========+"))
            {
                // proximo perfil ==> reseta variaveis necessarias
                count_data = 0;
                flag = 0;

                // guarda listas de exp. e hab. no perfil
                p->experiencia = auxexp;
                p->habilidades = auxhab;

                // insere perfil na lista
                perfilListInsert(listaGeral,p);
            }
            else
            {
                line[strlen(line)-1] = '\0';
                char *campo = malloc(sizeof(char) * (strlen(line)+1));

                if(count_data == 7)
                    stringListInsert(auxhab,campo);
                else if(count_data == 8)
                    stringListInsert(auxexp,campo);
            }
        }
    }

    fclose(cadastros); // Fecha arquivo
    return listaGeral; // retorna lista criada
}
Perfil *encontrarPerfil(char *email, NoPerfil *lista)
{
    // perfil padrao para retorno
    Perfil *p;
    p = NULL;

    // percorre toda a lista em busca do perfil
    NoPerfil *at;
    for(at = lista->prox; at != NULL; at = at->prox)
    {
        if(!strcmp(at->pessoa->email,email))
        {
            p = at->pessoa;
            break;
        }
    }
    return p; // retorna perfil
}
int adicionaPerfil(Perfil *p)
{
    // cria lista com todos os perfis
    NoPerfil *lista;
    lista = listarTodos();

    // variavel de retorno
    int adicionou = 0;

    // se o email nao esta cadastrado
    if(!existePerfil(p->email,lista))
    {
        perfilListInsert(lista,p);  // insere perfil na lista
        atualizaArquivo(lista);     // atualiza o arquivo
        adicionou = 1;              // att retorno
    }
    else
        freePerfil(p);

    perfilListFree(lista); // libera memoria alocada na busca
    return adicionou;      // retorna flag
}
int removerPerfil(char *email)
{
    // cria lista com todos os perfis
    NoPerfil *lista;
    lista = listarTodos();

    // variavel de retorno
    int removeu = 0;

    // se o email esta cadastrado, o perfil existe e deve ser removido
    if(existePerfil(email,lista))
    {
        perfilListRemove(lista,email); // remove perfil da lista
        atualizaArquivo(lista);        // atualiza o arquivo
        removeu = 1;                   // att retorno
    }

    perfilListFree(lista); // libera memoria alocada na busca
    return removeu;        // retorna flag
}
NoPerfilEmailNome *listarPorFormacao(char *form)
{
    // cria lista com todos os perfis
    NoPerfil *lista;
    lista = listarTodos();

    // cria lista para retorno
    NoPerfilEmailNome *listaRet;
    listaRet = (NoPerfilEmailNome*)malloc(sizeof(NoPerfilEmailNome));
    listaRet->perfil = NULL;
    listaRet->prox = NULL;

    // percorre lista de cadastros
    NoPerfil *at;
    for(at = lista->prox; at != NULL; at = at->prox)
    {
        // se a pessoa for do curso buscado
        if(!strcmp(at->pessoa->formacao,form))
        {
            // aloca perfil aux
            PerfilEmailNome *penc;
            penc = (struct perfilEmailNome*)malloc(sizeof(PerfilEmailNome));

            // tamanhos das strings do perfil aux
            int tams[3] = {(int)strlen(at->pessoa->email),(int)strlen(at->pessoa->nome),(int)strlen(at->pessoa->sobrenome)};

            // aloca memoria para o conteudo do perfil aux
            penc->email = malloc(sizeof(char) * (tams[0]+1));
            penc->nome = malloc(sizeof(char) * (tams[1]+1));
            penc->sobrenome = malloc(sizeof(char) * (tams[2]+1));

            // atribui copia valores ao perfil aux
            strcpy(penc->email,at->pessoa->email);
            strcpy(penc->nome,at->pessoa->nome);
            strcpy(penc->sobrenome,at->pessoa->sobrenome);

            // insere estrutura na lista de retorno
            NPENListInsert(listaRet,penc);
        }
    }

    perfilListFree(lista); // libera memoria alocada na busca
    return listaRet;       // retorna lista
}
NoPerfilEmailNome *listarPorHabilidade(char *hab)
{
    // cria lista com todos os perfis
    NoPerfil *lista;
    lista = listarTodos();

    // cria lista para retorno
    NoPerfilEmailNome *listaRet;
    listaRet = ( NoPerfilEmailNome*)malloc(sizeof( NoPerfilEmailNome));
    listaRet->perfil = NULL;
    listaRet->prox = NULL;

    // percorre lista de cadastros
    NoPerfil *at;
    for(at = lista->prox; at != NULL; at = at->prox)
    {
        // se a pessoa for do curso buscado
        if(existeString(hab,at->pessoa->habilidades))
        {
            // aloca perfil aux
            PerfilEmailNome *penc;
            penc = (struct perfilEmailNome*)malloc(sizeof(PerfilEmailNome));

            // tamanhos das strings do perfil aux
            int tams[3] = {(int)strlen(at->pessoa->email),(int)strlen(at->pessoa->nome),(int)strlen(at->pessoa->sobrenome)};

            // aloca memoria para o conteudo do perfil aux
            penc->email = malloc(sizeof(char) * (tams[0]+1));
            penc->nome = malloc(sizeof(char) * (tams[1]+1));
            penc->sobrenome = malloc(sizeof(char) * (tams[2]+1));

            // atribui copia valores ao perfil aux
            strcpy(penc->email,at->pessoa->email);
            strcpy(penc->nome,at->pessoa->nome);
            strcpy(penc->sobrenome,at->pessoa->sobrenome);

            // insere estrutura na lista de retorno
            NPENListInsert(listaRet,penc);
        }
    }

    perfilListFree(lista); // libera memoria alocada na busca
    return listaRet;       // retorna lista
}
NoPerfilEmailNomeCurso *listarPorAno(int ano)
{
    // cria lista com todos os perfis
    NoPerfil *lista;
    lista = listarTodos();

    // cria lista para retorno
    NoPerfilEmailNomeCurso *listaRet;
    listaRet = (NoPerfilEmailNomeCurso*)malloc(sizeof(NoPerfilEmailNomeCurso));
    listaRet->perfil = NULL;
    listaRet->prox = NULL;

    // percorre lista de cadastros
    NoPerfil *at;
    for(at = lista->prox; at != NULL; at = at->prox)
    {
        // se a pessoa formou no ano buscado
        if(at->pessoa->ano_formatura == ano)
        {
            // aloca perfil aux
            PerfilEmailNomeCurso *penc;
            penc = (struct perfilEmailNomeCurso*)malloc(sizeof(PerfilEmailNomeCurso));

            // tamanhos das strings do perfil aux
            int tams[4] = {(int)strlen(at->pessoa->email),(int)strlen(at->pessoa->nome),(int)strlen(at->pessoa->sobrenome),(int)strlen(at->pessoa->formacao)};

            // aloca memoria para o conteudo do perfil aux
            penc->email = malloc(sizeof(char) * (tams[0]+1));
            penc->nome = malloc(sizeof(char) * (tams[1]+1));
            penc->sobrenome = malloc(sizeof(char) * (tams[2]+1));
            penc->formacao = malloc(sizeof(char) * (tams[3]+1));

            // atribui copia valores ao perfil aux
            strcpy(penc->email,at->pessoa->email);
            strcpy(penc->nome,at->pessoa->nome);
            strcpy(penc->sobrenome,at->pessoa->sobrenome);
            strcpy(penc->formacao,at->pessoa->formacao);

            // insere estrutura na lista de retorno
            NPENCListInsert(listaRet,penc);
        }
    }

    perfilListFree(lista); // libera memoria alocada na busca
    return listaRet;       // retorna lista
}
int addExperiencia(char *email, char *exp)
{
    // cria lista com todos os perfis
    NoPerfil *lista;
    lista = listarTodos();

    // acha perfil (ou nao)
    Perfil *p;
    p = encontrarPerfil(email,lista);

    // variavel de retorno
    int resultado = 0;

    if(p != NULL)
    {
        // verifica se a experiencia ja existe no perfil
        NoString *at;
        for(at = p->experiencia->prox; at != NULL; at = at->prox)
        {
            if(!strcmp(exp,at->str))
            {
                resultado = 1;
                break;
            }
        }

        if(!resultado)
        {
            char *expins = malloc(sizeof(char)* (strlen(exp)+1));
            strcpy(expins,exp);
            stringListInsert(p->experiencia,expins); // insere experiencia
            atualizaArquivo(lista);                  // atualiza o arquivo
            resultado = 2;
        }
    }
    perfilListFree(lista);  // libera memoria alocada
    return resultado;       // retorna flag
}