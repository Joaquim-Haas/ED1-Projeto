#include "operacoes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void inserirCircularEquipe(Equipe **topo, Equipe *novo){
    if(*topo == NULL){
        novo->prox = novo;
        *topo = novo;
    }
    else{
        novo->prox = (*topo)->prox;
        (*topo)->prox = novo;
    }
}

static void inserirCircularChamado(Chamado **topo, Chamado *novo){
    if(*topo == NULL){
        novo->prox = novo;
        *topo = novo;
    }
    else{
        novo->prox = (*topo)->prox;
        (*topo)->prox = novo;
    }
}

Equipe *buscarEquipe(Equipe *listaEquipes, int codEquipe){
    if(listaEquipes == NULL){
        return NULL;
    }

    Equipe *aux = listaEquipes->prox;

    do{
        if(aux->codigo == codEquipe){
            return aux;
        }
        aux = aux->prox;
    }while(aux != listaEquipes->prox);

    return NULL;
}

Chamado *buscarChamado(Chamado *listaChamados, int codChamado){
    if(listaChamados == NULL){
        return NULL;
    }

    Chamado *aux = listaChamados->prox;

    do{
        if(aux->codigo == codChamado){
            return aux;
        }
        aux = aux->prox;
    }while(aux != listaChamados->prox);

    return NULL;
}

void cadastrarEquipe(Equipe **topo, int id, char *nome, char *especialidade){
    if(buscarEquipe(*topo, id) != NULL){
        printf("ERRO: Ja existe uma equipe cadastrada com o codigo [%d]. . .\n", id);
        return;
    }

    Equipe *novo = (Equipe *)malloc(sizeof(Equipe));
    if(novo == NULL){
        printf("ERRO: Falhou ao alocar memoria para Equipe. . .\n");
        return;
    }

    novo->codigo = id;
    strcpy(novo->nome, nome);
    snprintf(novo->especialidade, sizeof(novo->especialidade), "%s", especialidade);
    novo->total_atendimentos = 0;
    novo->listaChamados = NULL;
    novo->prox = NULL;

    inserirCircularEquipe(topo, novo);
    printf("Equipe [%s] cadastrada com sucesso. . .\n", nome);
}

int gerarChamado(Chamado **listaChamados, Bairro *listaBairros, int codChamado, int codOcorrencia, int prioridade, int status){
    if(buscarChamado(*listaChamados, codChamado) != NULL){
        printf("ERRO: Ja existe um chamado com o codigo [%d]. . .\n", codChamado);
        return 0;
    }

    Ocorrencia *ocorrencia = buscarOcorrenciaPorCodigo(listaBairros, codOcorrencia);
    if(ocorrencia == NULL){
        printf("ERRO: Ocorrencia [%d] nao encontrada para gerar chamado. . .\n", codOcorrencia);
        return 0;
    }

    Chamado *novo = (Chamado *)malloc(sizeof(Chamado));
    if(novo == NULL){
        printf("ERRO: Falhou ao alocar memoria para Chamado. . .\n");
        return 0;
    }

    novo->codigo = codChamado;
    novo->prioridade = prioridade;
    novo->status = status;
    novo->ocorrencia = ocorrencia;
    novo->equipe = NULL;
    novo->prox = NULL;

    inserirCircularChamado(listaChamados, novo);
    printf("Chamado [%d] gerado para ocorrencia [%d]. . .\n", codChamado, codOcorrencia);
    return 1;
}

int associarEquipe(Chamado *listaChamados, Equipe **listaEquipes, int codChamado, int codEquipe){
    Chamado *chamado = buscarChamado(listaChamados, codChamado);
    if(chamado == NULL){
        printf("ERRO: Chamado [%d] nao encontrado. . .\n", codChamado);
        return 0;
    }

    Equipe *equipe = buscarEquipe(*listaEquipes, codEquipe);
    if(equipe == NULL){
        printf("ERRO: Equipe [%d] nao encontrada. . .\n", codEquipe);
        return 0;
    }

    if(chamado->equipe != NULL){
        printf("ERRO: Chamado [%d] ja esta associado a equipe [%d]. . .\n", codChamado, chamado->equipe->codigo);
        return 0;
    }

    chamado->equipe = equipe;
    printf("Chamado [%d] associado a equipe [%s]. . .\n", codChamado, equipe->nome);
    return 1;
}

int finalizarChamado(Chamado *listaChamados, Equipe *listaEquipes, int codChamado){
    Chamado *chamado = buscarChamado(listaChamados, codChamado);

    (void)listaEquipes;
    if(chamado == NULL){
        printf("ERRO: Chamado [%d] nao encontrado. . .\n", codChamado);
        return 0;
    }

    if(chamado->status == 2){
        printf("ERRO: Chamado [%d] ja foi finalizado. . .\n", codChamado);
        return 0;
    }

    chamado->status = 2;

    if(chamado->equipe != NULL){
        chamado->equipe->total_atendimentos++;
    }

    printf("Chamado [%d] finalizado com sucesso. . .\n", codChamado);
    return 1;
}

void relatorioGeral(Bairro *listaBairros, Equipe *listaEquipes, Chamado *listaChamados){
    int totalBairros = 0;
    int totalSensores = 0;
    int totalOcorrencias = 0;
    int totalEquipes = 0;
    int totalChamados = 0;
    int chamadosAbertos = 0;
    int chamadosFinalizados = 0;

    printf("\n========== RELATORIO GERAL ==========\n");

    if(listaBairros != NULL){
        Bairro *auxBairro = listaBairros->prox;
        do{
            totalBairros++;
            if(auxBairro->listaSensores != NULL){
                Sensor *auxSensor = auxBairro->listaSensores->prox;
                do{
                    totalSensores++;
                    if(auxSensor->listaOcorrencias != NULL){
                        Ocorrencia *auxOcorr = auxSensor->listaOcorrencias->prox;
                        do{
                            totalOcorrencias++;
                            auxOcorr = auxOcorr->prox;
                        }while(auxOcorr != auxSensor->listaOcorrencias->prox);
                    }
                    auxSensor = auxSensor->prox;
                }while(auxSensor != auxBairro->listaSensores->prox);
            }
            auxBairro = auxBairro->prox;
        }while(auxBairro != listaBairros->prox);
    }

    if(listaEquipes != NULL){
        Equipe *auxEquipe = listaEquipes->prox;
        do{
            totalEquipes++;
            printf("Equipe [%s] - Atendimentos: [%d]\n", auxEquipe->nome, auxEquipe->total_atendimentos);
            auxEquipe = auxEquipe->prox;
        }while(auxEquipe != listaEquipes->prox);
    }

    if(listaChamados != NULL){
        Chamado *auxChamado = listaChamados->prox;
        do{
            totalChamados++;
            if(auxChamado->status == 2){
                chamadosFinalizados++;
            }
            else{
                chamadosAbertos++;
            }
            auxChamado = auxChamado->prox;
        }while(auxChamado != listaChamados->prox);
    }

    printf("Bairros: [%d] | Sensores: [%d] | Ocorrencias: [%d]\n", totalBairros, totalSensores, totalOcorrencias);
    printf("Equipes: [%d] | Chamados: [%d] (Abertos: [%d] | Finalizados: [%d])\n",
           totalEquipes, totalChamados, chamadosAbertos, chamadosFinalizados);
    printf("=====================================\n\n");
}

void carregarEquipesArquivo(Equipe **listaEquipes){
    FILE *arq = fopen("equipes.txt", "r");
    if(arq == NULL){
        printf("ERRO: Nao foi possivel abrir o arquivo equipes.txt. . .\n");
        return;
    }

    int codigo;
    char nome[50];
    char especialidade[30];

    while(fscanf(arq, "%d %49s %29s", &codigo, nome, especialidade) == 3){
        cadastrarEquipe(listaEquipes, codigo, nome, especialidade);
    }

    fclose(arq);
}

void carregarChamadosArquivo(Chamado **listaChamados, Bairro *listaBairros){
    FILE *arq = fopen("chamados.txt", "r");
    if(arq == NULL){
        printf("ERRO: Nao foi possivel abrir o arquivo chamados.txt. . .\n");
        return;
    }

    int codChamado, codOcorr, campoExtra, prioridade, status;

    while(fscanf(arq, "%d %d %d %d %d", &codChamado, &codOcorr, &campoExtra, &prioridade, &status) == 5){
        (void)campoExtra;
        gerarChamado(listaChamados, listaBairros, codChamado, codOcorr, prioridade, status);
    }

    fclose(arq);
}

void salvarEquipesArquivo(Equipe *listaEquipes){
    if(listaEquipes == NULL){
        return;
    }

    FILE *arq = fopen("equipes.txt", "w");
    if(arq == NULL){
        printf("ERRO: Arquivo equipes.txt erro ao abrir. . .\n");
        return;
    }

    Equipe *aux = listaEquipes->prox;
    do{
        fprintf(arq, "%d %s %s\n", aux->codigo, aux->nome, aux->especialidade);
        aux = aux->prox;
    }while(aux != listaEquipes->prox);

    fclose(arq);
}

void salvarChamadosArquivo(Chamado *listaChamados){
    if(listaChamados == NULL){
        return;
    }

    FILE *arq = fopen("chamados.txt", "w");
    if(arq == NULL){
        printf("ERRO: Arquivo chamados.txt erro ao abrir. . .\n");
        return;
    }

    Chamado *aux = listaChamados->prox;
    do{
        int codOcorr = (aux->ocorrencia != NULL) ? aux->ocorrencia->codigo : 0;
        int codEquipe = (aux->equipe != NULL) ? aux->equipe->codigo : 0;
        fprintf(arq, "%d %d %d %d %d\n", aux->codigo, codOcorr, codEquipe, aux->prioridade, aux->status);
        aux = aux->prox;
    }while(aux != listaChamados->prox);

    fclose(arq);
}

void liberarChamados(Chamado **listaChamados){
    if(*listaChamados == NULL){
        return;
    }

    Chamado *aux = (*listaChamados)->prox;
    Chamado *prox;

    (*listaChamados)->prox = NULL;

    while(aux != NULL){
        prox = aux->prox;
        aux->equipe = NULL;
        aux->ocorrencia = NULL;
        free(aux);
        aux = prox;
    }

    *listaChamados = NULL;
}

void liberarEquipes(Equipe **listaEquipes){
    if(*listaEquipes == NULL){
        return;
    }

    Equipe *aux = (*listaEquipes)->prox;
    Equipe *prox;

    (*listaEquipes)->prox = NULL;

    while(aux != NULL){
        prox = aux->prox;
        aux->listaChamados = NULL;
        free(aux);
        aux = prox;
    }

    *listaEquipes = NULL;
}
