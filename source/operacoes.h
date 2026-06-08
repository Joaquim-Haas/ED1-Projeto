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

// Equipe CRUD
void cadastrarEquipe(Equipe **topo, int id, char *nome, char *especialidade);
Equipe *buscarEquipe(Equipe *listaEquipes, int codigo);
void listarEquipes(Equipe *listaEquipes);
void removerEquipe(Equipe **listaEquipes, int codigo);

// Chamado CRUD
Chamado *alocarChamado(int codigo, int prioridade, int status, Ocorrencia *ocorrencia);
void cadastrarChamado(Chamado **listaChamadosGeral, int codigo, int prioridade, int status, Ocorrencia *ocorrencia);
Chamado *buscarChamado(Chamado *listaChamadosGeral, Equipe *listaEquipes, int codigo);
int associarChamadoEquipe(Equipe *listaEquipes, int codChamado, int codEquipe, Chamado **listaChamadosGeral);
int finalizarChamado(Chamado *listaChamadosGeral, Equipe *listaEquipes, int codChamado);
int gerarChamado(Chamado **listaChamadosGeral, Bairro *listaBairros, int codigo, int codOcorrencia, int prioridade, int status);

//Regra automarica
void verificarGeracaoAutomatica(Bairro *listaBairros, Equipe *listaEquipes, Chamado **listaChamadosGeral, Sensor *sensor, Ocorrencia *ocorrencia);

// arquivo I/O
void carregarEquipes(char *nomeArquivo, Equipe **listaEquipes);
void carregarChamados(char *nomeArquivo, Chamado **listaChamadosGeral, Equipe *listaEquipes, Bairro *listaBairros);
void carregarEquipesArquivo(Equipe **listaEquipes);
void carregarChamadosArquivo(Chamado **listaChamadosGeral, Equipe *listaEquipes, Bairro *listaBairros);
void salvarEquipes(char *nomeArquivo, Equipe *listaEquipes);
void salvarChamados(char *nomeArquivo, Chamado *listaChamadosGeral, Equipe *listaEquipes);
void salvarEquipesArquivo(Equipe *listaEquipes);
void salvarChamadosArquivo(Chamado *listaChamadosGeral, Equipe *listaEquipes);

// limpar
void liberarEquipesChamados(Equipe **listaEquipes, Chamado **listaChamadosGeral);
void liberarEquipes(Equipe **listaEquipes);
void liberarChamados(Chamado **listaChamadosGeral);

// relatorios
void relatorioBairrosMaisOcorrencias(Bairro *listaBairros);
void relatorioSensoresOffline(Bairro *listaBairros);
void relatorioOcorrenciasCriticasAbertas(Bairro *listaBairros);
void relatorioEquipeMaisAtendimentos(Equipe *listaEquipes);
void relatorioQuantidadeSensoresBairro(Bairro *listaBairros);
void relatorioOcorrenciasPorSeveridade(Bairro *listaBairros);
void relatorioGeral(Bairro *listaBairros, Equipe *listaEquipes, Chamado *listaChamadosGeral);

#endif
