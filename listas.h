#ifndef LISTAS_H
#define LISTAS_H

/* ------------------------------- STRUCT DE LISTA DE STRING ------------------------------- */
typedef struct noString
{
    struct noString *prox;
    char *str;
} NoString;

/* ------------------------------- FUNCOES DA LISTA DE STRING ------------------------------ */
void stringListInsert(NoString *atual, char *str);
void stringListFree(NoString *atual);
int stringListLen(NoString *lista);
int existeString(char *hab, NoString *lista);

/* ----------------------------- STRUCTS DE PERFIS AUXILIARES ------------------------------ */
/* struct auxiliar (perfil reduzido) */
typedef struct perfilEmailNome
{
    char  *email;
    char  *nome;
    char  *sobrenome;
}PerfilEmailNome;
/* segunda struct auxiliar (perfil reduzido) */
typedef struct perfilEmailNomeCurso
{
    char  *email;
    char  *nome;
    char  *sobrenome;
    char  *formacao;
}PerfilEmailNomeCurso;

/* ------------------------- STRUCTS DE LISTAS DE PERFIS AUXILIARES ------------------------ */
/* no de struct auxiliar (perfil reduzido) */
typedef struct noPerfilEmailNome
{
    PerfilEmailNome *perfil;
    struct noPerfilEmailNome *prox;
} NoPerfilEmailNome;
/* no de segunda struct auxiliar (perfil reduzido) */
typedef struct noPerfilEmailNomeCurso
{
    PerfilEmailNomeCurso *perfil;
    struct noPerfilEmailNomeCurso *prox;
} NoPerfilEmailNomeCurso;

/* ------------------------ FUNCOES DAS LISTAS DE PERFIS AUXILIARES ------------------------ */
void NPENCListInsert(NoPerfilEmailNomeCurso *atual, PerfilEmailNomeCurso *info);
void NPENCListFree(NoPerfilEmailNomeCurso *atual);
int NPENCListLen(NoPerfilEmailNomeCurso *lista);

void NPENListInsert(NoPerfilEmailNome *atual, PerfilEmailNome *info);
void NPENListFree(NoPerfilEmailNome *atual);
int NPENListLen(NoPerfilEmailNome *lista);

/* ------------------------------------ STRUCT DE PERFIL ----------------------------------- */
typedef struct perfil
{
    char  *email;
    char  *nome;
    char  *sobrenome;
    char  *cidade_residencia;
    char  *formacao;

    int ano_formatura;

    NoString *experiencia;
    NoString *habilidades;
} Perfil;

/* ------------------------------- STRUCT DE LISTA DE PERFIL ------------------------------- */
typedef struct noPerfil
{
    Perfil *pessoa;
    struct noPerfil *prox;
} NoPerfil;

/* ------------------------------- FUNCOES DE LISTA DE PERFIL ------------------------------ */
void perfilListInsert(NoPerfil *atual, Perfil *pessoa);
void perfilListRemove(NoPerfil *atual, char *email);
Perfil *createPerfil(char *email, char *nome, char *sn, int ano, char *form, char *rsd, NoString *lexp, NoString *lhab);
void freePerfil(Perfil *p);
void perfilListFree(NoPerfil *atual);
int perfilListLen(NoPerfil *lista);
int existePerfil(char *email, NoPerfil *lista);

#endif
