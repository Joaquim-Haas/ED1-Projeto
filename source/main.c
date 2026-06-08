#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "urbano.h"
#include "operacoes.h"
#include "simulador.h"

int main(void){
    Bairro *listaBairros = NULL;
    Equipe *listaEquipes = NULL;
    Chamado *listaChamados = NULL;

    carregarBairrosArquivo(&listaBairros);
    carregarSensoresArquivo(listaBairros);
    carregarOcorrenciasArquivo(listaBairros);
    carregarEquipesArquivo(&listaEquipes);
    carregarChamadosArquivo(&listaChamados, listaBairros);

    executarSimulacao("entrada_simulacao.txt", &listaBairros, &listaEquipes, &listaChamados);

    verificarConsistenciaDados(listaBairros, listaEquipes, listaChamados);

    salvarBairrosArquivo(listaBairros);
    salvarSensoresArquivo(listaBairros);
    salvarOcorrenciasArquivo(listaBairros);
    salvarEquipesArquivo(listaEquipes);
    salvarChamadosArquivo(listaChamados);

    liberarMemoriaGeral(&listaBairros, &listaEquipes, &listaChamados);

    return 0;
}
