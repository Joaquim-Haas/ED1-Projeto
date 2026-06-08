#ifndef OPERACOES_H
#define OPERACOES_H

#include "urbano.h"

typedef struct equipe Equipe;

typedef struct chamado{
    int codigo;
    int prioridade;
    int status;
    Ocorrencia *ocorrencia;
    Equipe *equipe;
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

void cadastrarEquipe(Equipe **topo, int id, char *nome, char *especialidade);
int gerarChamado(Chamado **listaChamados, Bairro *listaBairros, int codChamado, int codOcorrencia, int prioridade, int status);
int associarEquipe(Chamado *listaChamados, Equipe **listaEquipes, int codChamado, int codEquipe);
int finalizarChamado(Chamado *listaChamados, Equipe *listaEquipes, int codChamado);
void relatorioGeral(Bairro *listaBairros, Equipe *listaEquipes, Chamado *listaChamados);

Equipe *buscarEquipe(Equipe *listaEquipes, int codEquipe);
Chamado *buscarChamado(Chamado *listaChamados, int codChamado);

void carregarEquipesArquivo(Equipe **listaEquipes);
void carregarChamadosArquivo(Chamado **listaChamados, Bairro *listaBairros);
void salvarEquipesArquivo(Equipe *listaEquipes);
void salvarChamadosArquivo(Chamado *listaChamados);

void liberarEquipes(Equipe **listaEquipes);
void liberarChamados(Chamado **listaChamados);

#endif
