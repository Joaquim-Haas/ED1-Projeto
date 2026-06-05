#include "urbano.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

Ocorrencia *alocarOcorrencia(int codOcorrencia, int severidade, char *descricao, int statusOcorrencia){
    Ocorrencia *novo = NULL;
    novo = (Ocorrencia *)malloc(sizeof(Ocorrencia));

    if(novo == NULL){
        printf("Erro ao alocar memoria para Ocorrencia. . .\n");
        return NULL;
    }

    if(novo){
        novo->codigo = codOcorrencia;
        novo->severidade = severidade;
        strcpy(novo->descricao, descricao);
        novo->status = statusOcorrencia;
        novo->prox = NULL;
    }
    return novo;
}

Sensor *alocarSensor(int codSensor, int tipoSensor, int statusSensor){
    Sensor *novo = NULL;
    novo = (Sensor *)malloc(sizeof(Sensor));

    if(novo == NULL){
        printf("Erro ao alocar memoria para Sensor. . .\n");
        return NULL;
    }

    if(novo){
        novo->codigo = codSensor;
        novo->tipo = tipoSensor;
        novo->status = statusSensor;
        novo->listaOcorrencias = NULL;
        novo->prox = NULL;
    }
    return novo;
}

Bairro *alocarBairro(int codBairro, char *nomeBairro){
    Bairro *novo = NULL;
    novo = (Bairro *)malloc(sizeof(Bairro));

    if(novo == NULL){
        printf("ERRO ao alocar memoria para Bairro. . .\n");
        return NULL;
    }

    if(novo){
        novo->codigo = codBairro;
        strcpy(novo->nome, nomeBairro);
        novo->listaSensores = NULL; 
        novo->prox = NULL;
    }
    return novo;
}

void cadastrarBairro(Bairro **lista, int codBairro, char *Nome){
    if(buscarBairro(*lista, codBairro) != NULL){
        printf("Erro: Ja existe um bairro cadastrado com o codigo [%d]. . .\n", codBairro);
        return;
    }

    Bairro *novo = NULL;
    novo = alocarBairro(codBairro, Nome);

    if(novo == NULL){
        printf("Falhou ao alocar memoria para Bairro. . .\n");
    }

    if(*lista == NULL){
        novo->prox = novo;  //novo->prox aponta para si mesmo
        *lista = novo;  
    }                               
    else{                            //novo->prox = (*lista)->prox; o novo aponta para o atual primeiro
        novo->prox = (*lista)->prox; //(*lista)->prox = novo; o ultimo aponta para o novo que vira o primeiro
        (*lista)->prox = novo;       
    }
}

void cadastrarSensor(Bairro *listaBairros, int codBairro, int codSensor, int tipo){

}

void cadastrarOcorrencia(Bairro *listaBairros, int codBairro, int codSensor, int codOcorrencia, int severidade, char *descricao){

}

void imprimirBairros(Bairro *lista){
    if(lista == NULL){
        printf("Nao ha nenhum bairro para imprimir. . .\n");
        return;
    }

    Bairro *aux = lista->prox; //aux vira o primeiro da lista
    printf("\n-Imprimindo Bairros-\n");

    do{
        printf("Bairro: [%s] - Codigo: [%d]\n", aux->nome, aux->codigo);
        aux = aux->prox;
    }while(aux != lista->prox);
}   

void imprimirSensores(Bairro *bairroEspecif){
    if(bairroEspecif == NULL){
        printf("Bairro vazio ou nao especificado exatamente. . .\n");
        return;
    }

    Sensor *aux = bairroEspecif->listaSensores->prox;
    printf("\n-Imprimindo sensores do Bairro %s-\n", bairroEspecif->nome);

    do{
        printf("Sensor ID: [%d] - Tipo: [%d] - Status: [%d]\n", aux->codigo, aux->tipo, aux->status);
        aux = aux->prox;
    }while(aux != bairroEspecif->listaSensores->prox);
}

void imprimirOcorrencias(Sensor *sensorEspecif){
    if(sensorEspecif == NULL){
        printf("Sensor vazio ou nao especificado exatamente. . .\n");
        return;
    }

    Ocorrencia *aux = sensorEspecif->listaOcorrencias->prox;
    printf("\n-Imprimindo Ocorrencias do Sensor %d-\n", sensorEspecif->codigo);

    do{
        print("Ocorrencia ID: [%d] - Severidade [%d] - Status - [%d] - Descricao [%s]\n", aux->codigo, aux->severidade, aux->status, aux->descricao);
        aux = aux->prox;
    }while(aux != sensorEspecif->listaOcorrencias->prox);
}

Bairro *buscarBairro(Bairro *listaBairros, int codBairro){
    if(listaBairros == NULL){
        printf("Lista de bairro vazia. . .\n");
        return NULL;
    }

    Bairro *aux = listaBairros->prox; //aux vira o primeiro no

    do{
        if(aux->codigo == codBairro){
            return aux; 
        }
        aux = aux->prox;
    }while(aux != listaBairros->prox);

    return NULL;
}   

Sensor *buscarSensor(Bairro *listaBairros, int codBairro, int codSensor){
    if(listaBairros == NULL){
        printf("Lista de bairro vazia. . .\n");
        return NULL;
    }

    Bairro *bairroAchado = buscarBairro(listaBairros, codBairro);

    if(bairroAchado == NULL || bairroAchado->listaSensores == NULL){
        printf("Nao foi achado o bairro ou a lista esta sem sensor registrado. . .\n");
        return NULL;
    }

    Sensor *aux = bairroAchado->listaSensores->prox; // primeiro sensor

    do{
        if(aux->codigo == codSensor){
            return aux;
        }
        aux = aux->prox;
    }while(aux != bairroAchado->listaSensores->prox);
}

Ocorrencia *buscarOcorrencia(Bairro *listaBairros, int codBairro, int codSensor int codOcorrencia){
    if(listaBairros == NULL){
        printf("Lista de bairro vazia. . .\n");
        return NULL;
    }

    Sensor *sensorAchado = buscarSensor(listaBairros, codBairro, codSensor);

    if(sensorAchado == NULL || sensorAchado->listaOcorrencias == NULL){
        printf("Nao foi achado o sensor ou a lista esta sem ocorrencias registrada. . .\n");
        return NULL;
    }

    Ocorrencia *aux = sensorAchado->listaOcorrencias->prox; //primeira ocorrencia

    do{
        if(aux->codigo == codOcorrencia){
            return aux;
        }
        aux = aux->prox;
    }while(aux != sensorAchado->listaOcorrencias->prox);
}