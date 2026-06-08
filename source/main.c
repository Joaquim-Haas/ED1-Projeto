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
    int modo;

    //carregarDadosIniciais(&listaBairros, &listaEquipes, &listaChamados);

    printf("=========================================\n");
    printf("       SISTEMA DE MONITORAMENTO URBANO   \n");
    printf("=========================================\n");
    printf("Escolha o modo de operacao:\n");
    printf("[1] Modo Simulacao Automatica (entrada_simulacao.txt)\n");
    printf("[2] Modo Menu Interativo (Manual)\n");
    printf("Opcao: ");
    scanf("%d", &modo);

    if (modo == 1) {
        printf("\nIniciando interpretador de arquivos...\n");
        executarSimulacao("entrada_simulacao.txt", &listaBairros, &listaEquipes, &listaChamados);
        verificarConsistenciaDados(listaBairros, listaEquipes, listaChamados);
        salvarDadosNosArquivos(listaBairros, listaEquipes, listaChamados);
    } else if (modo == 2) {
        registrarLog("INICIO - Sessao em Modo Menu Manual iniciada", "INFO");
        exibirMenuInterativo(&listaBairros, &listaEquipes, &listaChamados);
    } else {
        printf("Opcao invalida. Encerrando programa.\n");
    }

    liberarMemoriaGeral(&listaBairros, &listaEquipes, &listaChamados);

    return 0;
}
