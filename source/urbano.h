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

Bairro *alocarBairro(int codBairro, char *nomeBairro); //ok
Sensor *alocarSensor(int codSensor, int tipoSensor, int statusSensor); //ok
Ocorrencia *alocarOcorrencia(int codOcorrencia, int severidade, char *descricao, int statusOcorrencia); //ok

void cadastrarBairro(Bairro **listaBairros, int codBairro, char *Nome); //ok
void cadastrarSensor(Bairro *listaBairros, int codBairro, int codSensor, int tipo); //ok
void cadastrarOcorrencia(Bairro *listaBairros, int codBairro, int codSensor, int codOcorrencia, int severidade, char *descricao); //ok

void imprimirBairros(Bairro *listaBairros); //ok
void imprimirSensores(Bairro *bairroEspecif); //ok
void imprimirOcorrencias(Sensor *sensorEspecif); //ok

Bairro *buscarBairro(Bairro *listaBairros, int codBairro); //ok
Sensor *buscarSensor(Bairro *listaBairros, int codBairro, int codSensor); //ok
Ocorrencia *buscarOcorrencia(Bairro *listaBairros, int codBairro, int codSensor, int codOcorrencia); //ok

void removerBairro(Bairro **listaBairros, int codBairro); //ok
void liberarBairro(Bairro **listaBairros); //ok
void liberarOcorrencias(Ocorrencia **listaOcorr); //ok
void liberarSensores(Sensor **listaSensores); //ok

#endif