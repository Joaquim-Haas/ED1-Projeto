#ifndef OPERACOES_H
#define OPERACOES_H

#include "urbano.h" 

typedef struct chamado{
    int codigo;
    int prioridade;
    int status;
    Ocorrencia *ocorrencia; //verifique se vai precisar
    struct chamado *prox;
}Chamado;

typedef struct equipe{
    int codigo;
    char nome[50];
    char especialidade[30];
    int total_atendimentos;
    Chamado *listaChamados;
    struct equipe *prox;
}Equipe;

void cadastrarEquipe(TEquipe **topo, int id, char *nome, char *especialidade);

#endif