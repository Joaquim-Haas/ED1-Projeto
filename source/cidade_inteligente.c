#include <stdio.h>
#include <stdlib.h>

#include "urbano.h"
#include "operacoes.h"
#include "simulador.h"
#include "menu.h"

int main(void) {
    Bairro *listaBairros = NULL;
    Equipe *listaEquipes = NULL;
    Chamado *listaChamados = NULL;

    carregarDadosIniciais(&listaBairros, &listaEquipes, &listaChamados);

    printf("=========================================\n");
    printf("  SMARTCITY - MENU INTERATIVO (MANUAL)   \n");
    printf("=========================================\n");

    exibirMenuInterativo(&listaBairros, &listaEquipes, &listaChamados);

    liberarMemoriaGeral(&listaBairros, &listaEquipes, &listaChamados);

    return 0;
}
