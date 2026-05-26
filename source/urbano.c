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
    Bairro *novo = NULL;
    novo = alocarBairro(int codBairro, Nome);

    if(novo == NULL){
        printf("Falhou ao alocar memoria para Bairro. . .\n");
    }

    if(*lista == NULL){
        novo->prox = novo;  //novo->prox aponta para si mesmo
        *lista = novo;  
    }                                //(*lista) sempre e o ultimo elemento da lista
    else{                            //novo->prox = (*lista)->prox; o novo aponta para o atual primeiro
        novo->prox = (*lista)->prox; //(*lista)->prox = novo; o ultimo aponta para o novo que vira o primeiro
        (*lista)->prox = novo;       //novo entra sempre no início pois é quem o ultimo (*lista) aponta
    }
}

void cadastrarSensor(Bairro *listaBairros, int codBairro, int codSensor, int tipo){

}

void cadastrarOcorrencia(Bairro *listaBairros, int codBairro, int codSensor, int codOcorrencia, int severidade, char *descricao){

}

void imprimirBairros(Bairro *lista){
    if(lista == NULL){
        printf("Lista vazia, nao ha nenhum bairro para imprimir. . .\n");
        return;
    }

    Bairro *aux = lista->prox; //aux vira o primeiro da lista
    printf("\n-Imprimindo Bairros-\n");

    do{
        printf("Bairro: %s - Codigo: %d\n", aux->nome, aux->codigo);
        aux = aux->prox;
    }while(aux != lista->prox);
}   

void imprimirSensores(Bairro *bairroEspecif){

}

void imprimirOcorrencias(Sensor *sensorEspecif){

}

Bairro *buscarBairro(Bairro *listaBairros, int codBairro){
    if(listaBairros == NULL){
        printf("Lista de bairro vazia. . .\n");
        return;
    }

    Bairro *aux = listaBairros->prox; //aux vira o primeiro no

    do{
        if(aux->codigo == codBairro){
            return aux; //se achar o no, retorna o proprio no. se nao retorna NULL
        }
        aux = aux->prox;
    }while(aux != listaBairros->prox)

    return NULL;
}   

Sensor *buscarSensor(Bairro *listaBairros, int codBairro, int codSensor){
    if(listaBairros == NULL){
        printf("Lista de bairro vazia. . .\n");
        return;
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
        return;
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