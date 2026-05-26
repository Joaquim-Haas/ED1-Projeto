#ifndef URBANO_H
#define URBANO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ocorrencia {
    int codigo;
    int severidade;
    char descricao[100];
    int status;
    struct ocorrencia *prox;
} Ocorrencia;

typedef struct sensor {
    int codigo;
    int tipo;
    int status;
    Ocorrencia *listaOcorrencias;
    struct sensor *prox;
} Sensor;

typedef struct bairro {
    int codigo;
    char nome[50];
    Sensor *listaSensores;
    struct bairro *prox;
} Bairro;

Ocorrencia *alocarOcorrencia();
Sensor *alocarSensor();
Bairro *alocarBairro();

void cadastrarBairro(Bairro **lista, int codBairro, char *Nome);
void cadastrarSensor(Bairro *listaBairros, int codBairro, int codSensor, char *tipo);
void cadastrarOcorrencia(Bairro *listaBairros, int codBairro, int codSensor, int codOcorrencia, int severidade, char *descricao);

#endif