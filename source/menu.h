#ifndef MENU_H
#define MENU_H

#include "urbano.h"
#include "operacoes.h"

void carregarDadosIniciais(Bairro **listaB, Equipe **listaE, Chamado **listaC);
void salvarDadosNosArquivos(Bairro *listaB, Equipe *listaE, Chamado *listaC);
void exibirMenuInterativo(Bairro **listaB, Equipe **listaE, Chamado **listaC);

#endif
