#ifndef OPERACOES_H
#define OPERACOES_H

#include "urbano.h" 

typedef struct Equipe {
    int id;
    char nome[50];
    char especialidade[30]; 
    struct Equipe *prox;
    struct Equipe *ant;
} TEquipe;

typedef struct Chamado {
    int id;
    int idSensor;     
    int idEquipe;     
    char status[20];  
    struct Chamado *prox;
    struct Chamado *ant;
} TChamado;

void cadastrarEquipe(TEquipe **topo, int id, char *nome, char *especialidade);

#endif