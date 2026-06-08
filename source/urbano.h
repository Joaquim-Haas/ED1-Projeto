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

Bairro *alocarBairro(int codBairro, char *nomeBairro);
Sensor *alocarSensor(int codSensor, int tipoSensor, int statusSensor);
Ocorrencia *alocarOcorrencia(int codOcorrencia, int severidade, char *descricao, int statusOcorrencia);

void cadastrarBairro(Bairro **listaBairros, int codBairro, char *Nome);
void cadastrarSensor(Bairro *listaBairros, int codBairro, int codSensor, int tipo);
void registrarOcorrencia(Bairro *listaBairros, int codBairro, int codSensor, int codOcorrencia, int severidade, char *descricao);

void listarBairros(Bairro *listaBairros);
void listarSensoresBairro(Bairro *bairroEspecif);
void listarOcorrencias(Sensor *sensorEspecif);

Bairro *buscarBairro(Bairro *listaBairros, int codBairro);
Sensor *buscarSensor(Bairro *listaBairros, int codBairro, int codSensor);
Ocorrencia *buscarOcorrencia(Bairro *listaBairros, int codBairro, int codSensor, int codOcorrencia);
Ocorrencia *buscarOcorrenciaPorCodigo(Bairro *listaBairros, int codOcorrencia);

int alterarStatusSensor(Bairro *listaBairros, int codSensor, int codBairro, int novoStatus);

void salvarBairrosArquivo(Bairro *listaBairros);
void salvarSensoresArquivo(Bairro *listaBairros);
void salvarOcorrenciasArquivo(Bairro *listaBairros);

void carregarBairrosArquivo(Bairro **listaBairros);
void carregarSensoresArquivo(Bairro *listaBairros);
void carregarOcorrenciasArquivo(Bairro *listaBairros);

void removerBairro(Bairro **listaBairros, int codBairro);
void liberarBairro(Bairro **listaBairros);
void liberarOcorrencias(Ocorrencia **listaOcorr);
void liberarSensores(Sensor **listaSensores);

#endif
