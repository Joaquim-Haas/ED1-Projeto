#include "urbano.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

Bairro *alocarBairro(int codBairro, char *nomeBairro){
    Bairro *novo = NULL;
    novo = (Bairro *)malloc(sizeof(Bairro));

    if(novo == NULL){
        printf("ERRO: Falhou ao alocar memoria para Bairro. . .\n");
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

Sensor *alocarSensor(int codSensor, int tipoSensor, int statusSensor){
    Sensor *novo = NULL;
    novo = (Sensor *)malloc(sizeof(Sensor));

    if(novo == NULL){
        printf("ERRO: Falhou ao alocar memoria para Sensor. . .\n");
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

Ocorrencia *alocarOcorrencia(int codOcorrencia, int severidade, char *descricao, int statusOcorrencia){
    Ocorrencia *novo = NULL;
    novo = (Ocorrencia *)malloc(sizeof(Ocorrencia));

    if(novo == NULL){
        printf("ERRO: Falhou ao alocar memoria para Ocorrencia. . .\n");
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

void cadastrarBairro(Bairro **lista, int codBairro, char *Nome){
    if(buscarBairro(*lista, codBairro) != NULL){
        printf("ERRO: Ja existe um bairro cadastrado com o codigo [%d]. . .\n", codBairro);
        return;
    }

    Bairro *novo = alocarBairro(codBairro, Nome);

    if(novo == NULL){
        printf("ERRO: Falhou ao alocar memoria para Bairro. . .\n");
        return;
    }

    if(*lista == NULL){
        novo->prox = novo;  //novo->prox aponta para si mesmo
        *lista = novo;  
    }                               
    else{                            //novo->prox = (*lista)->prox; o novo aponta para o atual primeiro
        novo->prox = (*lista)->prox; //(*lista)->prox = novo; o ultimo aponta para o novo que vira o primeiro
        (*lista)->prox = novo;       
    }

    printf("Bairro [%s] cadastrado com sucesso. . .\n", Nome);
}

void cadastrarSensor(Bairro *listaBairros, int codBairro, int codSensor, int tipo){
    Bairro *bairroAchado = buscarBairro(listaBairros, codBairro);

    if(bairroAchado == NULL){
        printf("ERRO: Nao e possivel cadastrar o sensor, bairro com o codigo [%d] nao existe . . .\n",codBairro);
        return;
    }

    Sensor *novo = alocarSensor(codSensor, tipo, 1);

    if(novo == NULL){
        printf("ERRO: Falhou ao alocar memoria para Sensor. . .\n");
        return;
    }

    if(bairroAchado->listaSensores == NULL){
        novo->prox = novo;
        bairroAchado->listaSensores = novo; //primeiro sensor
    }
    else{
        novo->prox = bairroAchado->listaSensores->prox; //novo sensor aponta para a lista original
        bairroAchado->listaSensores->prox = novo; //novo adicionado ao inicio
    }
    printf("Sensor [%d] cadastrado com sucesso no bairro [%s]. . .\n", codSensor, bairroAchado->nome);
}

void cadastrarOcorrencia(Bairro *listaBairros, int codBairro, int codSensor, int codOcorrencia, int severidade, char *descricao){
    Sensor *sensorAchado = buscarSensor(listaBairros, codBairro, codSensor);

    if(sensorAchado == NULL){
        printf("ERRO: Nao e possivel cadastrar a ocorrencia, Sensor [%d] nao encontrado no bairro com o codigo [%d]. . .\n", codSensor, codBairro);
        return;
    }   

    Ocorrencia *novo = alocarOcorrencia(codOcorrencia, severidade, descricao, 1);

    if(novo == NULL){
        printf("ERRO: Falhou ao alocar memoria para a Ocorrencia. . .\n");
        return;
    }

    if(sensorAchado->listaOcorrencias == NULL){
        novo->prox = novo;
        sensorAchado->listaOcorrencias = novo;
    }
    else{
        novo->prox = sensorAchado->listaOcorrencias->prox;
        sensorAchado->listaOcorrencias->prox = novo;
    }

    printf("Ocorrencia [%d] vinculada com sucesso ao sensor [%d]. . .\n", codOcorrencia, codSensor);  
}

void imprimirBairros(Bairro *lista){
    if(lista == NULL){
        printf("Nao ha nenhum bairro para imprimir. . .\n");
        return;
    }

    Bairro *aux = lista->prox; 
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
        printf("Ocorrencia ID: [%d] - Severidade [%d] - Status - [%d] - Descricao [%s]\n", aux->codigo, aux->severidade, aux->status, aux->descricao);
        aux = aux->prox;
    }while(aux != sensorEspecif->listaOcorrencias->prox);
}

Bairro *buscarBairro(Bairro *listaBairros, int codBairro){
    if(listaBairros == NULL){
        printf("Lista de bairro vazia. . .\n");
        return NULL;
    }

    Bairro *aux = listaBairros->prox; //primeiro bairro

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
        printf("ERRO: Nao foi achado o bairro ou a lista esta sem sensor registrado. . .\n");
        return NULL;
    }

    Sensor *aux = bairroAchado->listaSensores->prox; //primeiro sensor

    do{
        if(aux->codigo == codSensor){
            return aux;
        }
        aux = aux->prox;
    }while(aux != bairroAchado->listaSensores->prox);
}

Ocorrencia *buscarOcorrencia(Bairro *listaBairros, int codBairro, int codSensor, int codOcorrencia){
    if(listaBairros == NULL){
        printf("Lista de bairro vazia. . .\n");
        return NULL;
    }

    Sensor *sensorAchado = buscarSensor(listaBairros, codBairro, codSensor);

    if(sensorAchado == NULL || sensorAchado->listaOcorrencias == NULL){
        printf("ERRO: Nao foi achado o sensor ou a lista esta sem ocorrencias registrada. . .\n");
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

void removerBairro(Bairro **listaBairros, int codBairro){
    if(*listaBairros == NULL){
        printf("Lista de bairros vazia, nao ha o que remover. . .\n");
        return;
    }

    Bairro *aux = (*listaBairros)->prox; //primeiro
    Bairro *ant = *listaBairros;  //ultimo
    int achou = 0;

    do{
        if(aux->codigo == codBairro){
            achou = 1;
            break;
        }
        ant = aux;
        aux = aux->prox;
    }while(aux != (*listaBairros)->prox);

    if(achou == 0){
        printf("ERRO: Nao foi possivel achar o bairro com o codigo [%d]. . .\n", codBairro);
        return;
    }

    if(aux->listaSensores != NULL){
        printf("ERRO: Bairro possui sensores cadastrados e nao pode ser removido. . .\n");
        return;
    }
    
    if(aux == ant){
        *listaBairros == NULL; //unico elemento da lista
    }
    else{
        ant->prox = aux->prox; //ant aponta para o seguinte de aux
        if(aux == *listaBairros){ //se aux for o ultimo elemento
            *listaBairros == ant;
        }
    }

    printf("Bairro [%s] removido com sucesso. . .\n", aux->nome);
    free(aux);
}

void liberarBairro(Bairro **listaBairros) {
    if (*listaBairros == NULL) {
        printf("Nao ha o que liberar, 0 uso de memoria. . .\n");
        return;
    }//remove em ordem: ocorrencias -> sensores -> bairros -> NULL e free()

    Bairro *aux = (*listaBairros)->prox;
    Bairro *prox;

    (*listaBairros)->prox = NULL; //tira a circularidade para ter fim = NULL

    while (aux != NULL) {
        prox = aux->prox;

        if (aux->listaSensores != NULL) { //enquanto houver sensores >>  bairros->sensores
            liberarSensores(&(aux->listaSensores));
        }

        free(aux);
        aux = prox;
    }
    *listaBairros = NULL;
    printf("Memoria de Bairros, Sensores e Ocorrencias totalmente liberada. . .\n");
}

void liberarSensores(Sensor **listaSensores) {
    if (*listaSensores == NULL) return;

    Sensor *aux = (*listaSensores)->prox;
    Sensor *prox;

    (*listaSensores)->prox = NULL; 

    while (aux != NULL) {
        prox = aux->prox;

        if (aux->listaOcorrencias != NULL) { //enquanto houver ocorrencias >> bairros->sensores->ocorrencias
            liberarOcorrencias(&(aux->listaOcorrencias));
        }

        free(aux);
        aux = prox;
    }
    *listaSensores = NULL;
}

void liberarOcorrencias(Ocorrencia **listaOcorr) {
    if (*listaOcorr == NULL) return;

    Ocorrencia *aux = (*listaOcorr)->prox;
    Ocorrencia *prox;

    (*listaOcorr)->prox = NULL; 

    while (aux != NULL) {
        prox = aux->prox;
        free(aux);
        aux = prox;
    }
    *listaOcorr = NULL;
}