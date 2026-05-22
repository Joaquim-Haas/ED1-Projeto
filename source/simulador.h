#ifndef SIMULADOR_H
#define SIMULADOR_H

#include "urbano.h"
#include "operacoes.h"

void executarSimulacao(char *nomeArquivo, TBairro **listaB, TEquipe **listaE, TChamado **listaC);
void liberarMemoriaGeral(TBairro **listaB, TEquipe **listaE, TChamado **listaC);
void registrarLog(char *mensagem, char *status);
int verificarConsistenciaDados(TBairro *listaB, TEquipe *listaE, TChamado *listaC);

#endif