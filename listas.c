#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "listas.h"

/* ------------------------------------- FUNCOES DA LISTA DE PERFIL ------------------------------------- */

/* insere no final da lista */
void perfilListInsert(NoPerfil *lista, Perfil *pessoa)
{
    NoPerfil *at;
    for(at = lista; at != NULL; at = at->prox)
    {
        if(at->prox == NULL)
        {
            NoPerfil *novo;
            novo = (NoPerfil *)malloc(sizeof(NoPerfil));
            novo->pessoa = pessoa;
            novo->prox = NULL;
            at->prox = novo;
            break;
        }
    }
}

/* remove elemento da lista */
void perfilListRemove(NoPerfil *lista, char *email)
{
    NoPerfil *at;
    for(at = lista; at != NULL; at = at->prox)
    {
        if(at->prox != NULL)
        {
            if(!strcmp(at->prox->pessoa->email,email))
            {
                NoPerfil *aux;
                aux = at->prox;
                at->prox = aux->prox;

                // libera memoria alocada para o elemento
                freePerfil(aux->pessoa);
                free(aux);
            }
        }
    }
}

/* funcao para alocar memoria de um perfil e atribuir valores */
Perfil *createPerfil(char *email, char *nome, char *sn, int ano, char *form, char *rsd, NoString *lexp, NoString *lhab)
{
    Perfil *p;
    p = (Perfil*)malloc(sizeof(Perfil));
    p->email = email;
    p->nome = nome;
    p->sobrenome = sn;
    p->ano_formatura = ano;
    p->formacao = form;
    p->cidade_residencia = rsd;
    p->experiencia = lexp;
    p->habilidades = lhab;
    return p;
}

/* libera memoria usada em um perfil */
void freePerfil(Perfil *p)
{
    // libera memória utilizada
    free(p->email);
    free(p->nome);
    free(p->sobrenome);
    free(p->cidade_residencia);
    free(p->formacao);
    stringListFree(p->experiencia);
    stringListFree(p->habilidades);
    free(p);
}

/* libera memoria usada na lista */
void perfilListFree(NoPerfil *lista)
{
    int len = perfilListLen(lista);
    NoPerfil *auxVet[len];
    NoPerfil *at;
    at = lista->prox;

    for(int i=len-1; i>=0; i--)
    {
        auxVet[i] = at;
        at = at->prox;
    }

    for(int i=0; i<len; i++)
    {
        freePerfil(auxVet[i]->pessoa);
        free(auxVet[i]);
    }
    free(lista);
}

/* retorna tamanho da lista */
int perfilListLen(NoPerfil *lista)
{
    int len = 0;
    NoPerfil *at;
    for(at = lista->prox; at != NULL; at = at->prox)
        len++;
    return len;
}

/* returna se um perfil existe na lista ou nao */
int existePerfil(char *email, NoPerfil *lista)
{
    NoPerfil *at;
    for(at = lista->prox; at != NULL; at = at->prox)
    {
        if(!strcmp(at->pessoa->email,email))
            return 1;
    }
    return 0;
}

/* ------------------------------- FUNCOES DAS LISTAS DE PERFIS AUXILIARES ------------------------------ */

/* funcao de inserir ao final da lista */
void NPENCListInsert(NoPerfilEmailNomeCurso *lista, PerfilEmailNomeCurso *info)
{
    NoPerfilEmailNomeCurso *at;
    for(at = lista; at != NULL; at = at->prox)
    {
        if(at->prox == NULL)
        {
            NoPerfilEmailNomeCurso *novo;
            novo = (NoPerfilEmailNomeCurso*)malloc(sizeof(NoPerfilEmailNomeCurso));
            novo->perfil = info;
            at->prox = novo;
            novo->prox = NULL;
            break;
        }
    }
}

/* libera toda a memoria alocada para a lista */
void NPENCListFree(NoPerfilEmailNomeCurso *lista)
{
    int len = NPENCListLen(lista);
    NoPerfilEmailNomeCurso *auxVet[len];
    NoPerfilEmailNomeCurso *at;
    at = lista->prox;

    for(int i=len-1; i>=0; i--)
    {
        auxVet[i] = at;
        at = at->prox;
    }

    for(int i=0; i<len; i++)
    {
        free(auxVet[i]->perfil->email);
        free(auxVet[i]->perfil->nome);
        free(auxVet[i]->perfil->sobrenome);
        free(auxVet[i]->perfil->formacao);
        free(auxVet[i]->perfil);
        free(auxVet[i]);
    }
    free(lista);
}

/* retorna tamanho da lista */
int NPENCListLen(NoPerfilEmailNomeCurso *lista)
{
    int len = 0;
    NoPerfilEmailNomeCurso *at;
    for(at = lista->prox; at != NULL; at = at->prox)
        len++;
    return len;
}

/* funcao de inserir ao final da lista */
void NPENListInsert(NoPerfilEmailNome *lista, PerfilEmailNome *info)
{
    NoPerfilEmailNome *at;
    for(at = lista; at != NULL; at = at->prox)
    {
        if(at->prox == NULL)
        {
            NoPerfilEmailNome *novo;
            novo = (NoPerfilEmailNome*)malloc(sizeof(NoPerfilEmailNome));
            novo->perfil = info;
            at->prox = novo;
            novo->prox = NULL;
            break;
        }
    }
}

/* libera toda a memoria alocada para a lista */
void NPENListFree(NoPerfilEmailNome *lista)
{
    int len = NPENListLen(lista);
    NoPerfilEmailNome *auxVet[len];
    NoPerfilEmailNome *at;
    at = lista->prox;

    for(int i=len-1; i>=0; i--)
    {
        auxVet[i] = at;
        at = at->prox;
    }

    for(int i=0; i<len; i++)
    {
        free(auxVet[i]->perfil->email);
        free(auxVet[i]->perfil->nome);
        free(auxVet[i]->perfil->sobrenome);
        free(auxVet[i]->perfil);
        free(auxVet[i]);
    }
    free(lista);
}

/* retorna tamanho da lista */
int NPENListLen(NoPerfilEmailNome *lista)
{
    int len = 0;
    NoPerfilEmailNome *at;
    for(at = lista->prox; at != NULL; at = at->prox)
        len++;
    return len;
}

/* ------------------------------------- FUNCOES DA LISTA DE STRING ------------------------------------- */

/* funcao de inserir ao final da lista */
void stringListInsert(NoString *lista, char *str)
{
    NoString *at;
    for(at = lista; at != NULL; at = at->prox)
    {
        if(at->prox == NULL)
        {
            NoString *novo;
            novo = (NoString*)malloc(sizeof(NoString));
            novo->str = str;
            at->prox = novo;
            novo->prox = NULL;
            break;
        }
    }
}

/* libera toda a memoria alocada para a lista */
void stringListFree(NoString *lista)
{
    int len = stringListLen(lista);
    NoString *auxVet[len];
    NoString *at;
    at = lista->prox;

    for(int i=len-1; i>=0; i--)
    {
        auxVet[i] = at;
        at = at->prox;
    }

    for(int i=0; i<len; i++)
    {
        free(auxVet[i]->str);
        free(auxVet[i]);
    }
    free(lista);
}

/* retorna tamanho da lista */
int stringListLen(NoString *lista)
{
    int len = 0;
    NoString *at;
    for(at = lista->prox; at != NULL; at = at->prox)
        len++;
    return len;
}

/* retorna se a string existe na lista */
int existeString(char *str, NoString *lista)
{
    NoString *at;
    for(at = lista; at != NULL; at = at->prox)
    {
        if(at->prox != NULL)
        {
            if(!strcmp(at->prox->str,str))
                return 1;
        }
    }
    return 0;
}