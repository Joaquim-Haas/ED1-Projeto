#ifndef SIMULADOR_H
#define SIMULADOR_H

#include "urbano.h"
#include "operacoes.h"

void executarSimulacao(char *nomeArquivo, Bairro **listaB, Equipe **listaE, Chamado **listaC);
void liberarMemoriaGeral(Bairro **listaB, Equipe **listaE, Chamado **listaC);
void registrarLog(char *mensagem, char *status);
int verificarConsistenciaDados(Bairro *listaB, Equipe *listaE, Chamado *listaC);

#endif