#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "urbano.h"
#include "operacoes.h" 
#include "simulador.h"

int main(){

    Bairro *listaBairros = NULL;

    printf("==================================================\n");
    printf("          FASE 1: TESTE DE CADASTROS              \n");
    printf("==================================================\n");


    printf("\n--- Cadastrando Bairros ---\n");
    cadastrarBairro(&listaBairros, 10, "Uvaranas");
    cadastrarBairro(&listaBairros, 20, "Centro");
    cadastrarBairro(&listaBairros, 30, "Oficinas");

    imprimirBairros(listaBairros);

    printf("\n--- Cadastrando Sensores ---\n");
    // Dois sensores em Uvaranas (Cod 10)
    cadastrarSensor(listaBairros, 10, 101, 1); // Tipo 1 (ex: Alagamento)
    cadastrarSensor(listaBairros, 10, 102, 2); // Tipo 2 (ex: Desmoronamento)
    
    // Um sensor no Centro (Cod 20)
    cadastrarSensor(listaBairros, 20, 201, 1);
    
    // Teste de Erro: Tentando cadastrar sensor em bairro que não existe
    cadastrarSensor(listaBairros, 99, 999, 1);

    // 3. Cadastrando Ocorrências nos Sensores específicos
    printf("\n--- Cadastrando Ocorrencias ---\n");
    cadastrarOcorrencia(listaBairros, 10, 101, 501, 4, "Alagamento na rotatoria");
    cadastrarOcorrencia(listaBairros, 10, 101, 502, 2, "Nivel da agua subindo proximo a via");
    cadastrarOcorrencia(listaBairros, 20, 201, 503, 5, "Boca de lobo entupida obstruindo cruzamento");

    // Teste de Erro: Tentando cadastrar ocorrência em sensor inexistente
    cadastrarOcorrencia(listaBairros, 10, 888, 504, 1, "Teste Erro");

    printf("\n==================================================\n");
    printf("          FASE 2: IMPRESSAO DA ESTRUTURA          \n");
    printf("==================================================\n");

    // Vamos buscar os bairros para mandar imprimir as sublistas deles
    Bairro *b10 = buscarBairro(listaBairros, 10);
    if(b10 != NULL) {
        imprimirSensores(b10);
        
        // Vamos buscar o sensor 101 de Uvaranas para ver as ocorrências dele
        Sensor *s101 = buscarSensor(listaBairros, 10, 101);
        if(s101 != NULL) {
            imprimirOcorrencias(s101);
        }
    }

    Bairro *b20 = buscarBairro(listaBairros, 20);
    if(b20 != NULL) {
        imprimirSensores(b20);
    }

    printf("\n==================================================\n");
    printf("          FASE 3: TESTE DE REMOÇÃO (TRAVAS)       \n");
    printf("==================================================\n");

    // Teste 1: Tentar remover bairro com sensor ativo (Deve ser barrado pelo PDF!)
    printf("\nTentando remover Bairro 'Centro' (possui sensor 201):\n");
    removerBairro(&listaBairros, 20);

    // Teste 2: Remover um bairro vazio (Oficinas - Cod 30 não ganhou sensores)
    printf("\nTentando remover Bairro 'Oficinas' (vazio):\n");
    removerBairro(&listaBairros, 30);

    // Imprime novamente para checar se "Oficinas" sumiu e os outros continuam vivos
    imprimirBairros(listaBairros);

    printf("\n==================================================\n");
    printf("          FASE 4: ALTERAÇÃO DE DADOS (STATUS)     \n");
    printf("==================================================\n");
    
    // Simulando o que os outros arquivos farão: Alterar dados usando suas buscas
    printf("\nSimulando alteracao de status da ocorrencia 501...\n");
    Ocorrencia *ocorr = buscarOcorrencia(listaBairros, 10, 101, 501);
    if(ocorr != NULL) {
        printf("Status antigo da ocorrencia %d: %d\n", ocorr->codigo, ocorr->status);
        ocorr->status = 3; // Alterando para Finalizado!
        printf("Status NOVO da ocorrencia %d: %d\n", ocorr->codigo, ocorr->status);
    }

    printf("\n==================================================\n");
    printf("          FASE 5: LIBERAÇÃO TOTAL DA MEMÓRIA      \n");
    printf("==================================================\n");
    
    // Ao encerrar o programa, limpa tudo de vez
    liberarBairro(&listaBairros);

    return 0;
}