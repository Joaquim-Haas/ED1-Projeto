#ifndef URBANO_H
#define URBANO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Ocorrencia {
    int id;
    char descricao[100];
    char gravidade[20]; 
    char dataHora[20];  
    struct Ocorrencia *prox;
    struct Ocorrencia *ant;
} TOcorrencia;

typedef struct Sensor {
    int id;
    char tipo[30];      
    char status[15];    
    TOcorrencia *listaOcorrencias; 
    struct Sensor *prox;
    struct Sensor *ant;
} TSensor;

typedef struct Bairro {
    int id;
    char nome[50];
    TSensor *listaSensores; /
    struct Bairro *prox;
    struct Bairro *ant;
} TBairro;

void cadastrarBairro(TBairro **topo, int id, char *nome);
void cadastrarSensor(TBairro *listaBairros, int idBairro, int idSensor, char *tipo);

#endif