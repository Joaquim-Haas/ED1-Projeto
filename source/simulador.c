#include "simulador.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ARQUIVO_LOG "log_execucao.txt"

static FILE *arquivoLog = NULL;

static void trimFinal(char *texto){
    size_t len;

    if(texto == NULL){
        return;
    }

    len = strlen(texto);
    while(len > 0 && (texto[len - 1] == '\n' || texto[len - 1] == '\r' || isspace((unsigned char)texto[len - 1]))){
        texto[len - 1] = '\0';
        len--;
    }
}

static int linhaVazia(const char *texto){
    size_t i;

    if(texto == NULL){
        return 1;
    }

    for(i = 0; texto[i] != '\0'; i++){
        if(!isspace((unsigned char)texto[i])){
            return 0;
        }
    }

    return 1;
}

static Ocorrencia *buscarOcorrenciaGlobal(Bairro *listaBairros, int codOcorrencia){
    return buscarOcorrenciaPorCodigo(listaBairros, codOcorrencia);
}

static void listarTodasOcorrencias(Bairro *listaBairros){
    int encontrou = 0;

    if(listaBairros == NULL){
        printf("Nao ha ocorrencias para listar. . .\n");
        return;
    }

    Bairro *auxBairro = listaBairros->prox;

    do{
        if(auxBairro->listaSensores != NULL){
            Sensor *auxSensor = auxBairro->listaSensores->prox;

            do{
                if(auxSensor->listaOcorrencias != NULL){
                    listarOcorrencias(auxSensor);
                    encontrou = 1;
                }
                auxSensor = auxSensor->prox;
            }while(auxSensor != auxBairro->listaSensores->prox);
        }
        auxBairro = auxBairro->prox;
    }while(auxBairro != listaBairros->prox);

    if(!encontrou){
        printf("Nao ha ocorrencias para listar. . .\n");
    }
}

void registrarLog(char *mensagem, char *status){
    time_t agora;
    struct tm *infoTempo;
    char dataHora[32];

    if(arquivoLog == NULL){
        arquivoLog = fopen(ARQUIVO_LOG, "a");
        if(arquivoLog == NULL){
            printf("ERRO: Nao foi possivel abrir %s para registro. . .\n", ARQUIVO_LOG);
            return;
        }
    }

    agora = time(NULL);
    infoTempo = localtime(&agora);
    strftime(dataHora, sizeof(dataHora), "%d/%m/%Y %H:%M:%S", infoTempo);

    fprintf(arquivoLog, "[%s] [%s] %s\n", dataHora, status, mensagem);
    fflush(arquivoLog);
}

static void fecharLog(void){
    if(arquivoLog != NULL){
        fclose(arquivoLog);
        arquivoLog = NULL;
    }
}

static int processarLinha(char *linha, Bairro **listaB, Equipe **listaE, Chamado **listaC){
    char *comando;
    char bufferLog[512];
    char especialidade[30];

    comando = strtok(linha, " \t");
    if(comando == NULL){
        return 0;
    }

    if(strcmp(comando, "cadastrarBairro") == 0){
        char *tokenCodigo = strtok(NULL, " \t");
        char *tokenNome = strtok(NULL, " \t");

        if(tokenCodigo == NULL || tokenNome == NULL){
            registrarLog("cadastrarBairro - parametros insuficientes", "ERRO");
            return 0;
        }

        if(*listaB != NULL && buscarBairro(*listaB, atoi(tokenCodigo)) != NULL){
            snprintf(bufferLog, sizeof(bufferLog), "cadastrarBairro %s %s - ID ja existente", tokenCodigo, tokenNome);
            registrarLog(bufferLog, "ERRO");
            return 0;
        }

        cadastrarBairro(listaB, atoi(tokenCodigo), tokenNome);
        snprintf(bufferLog, sizeof(bufferLog), "cadastrarBairro %s %s", tokenCodigo, tokenNome);
        registrarLog(bufferLog, "SUCESSO");
        return 0;
    }
    else if(strcmp(comando, "cadastrarSensor") == 0){
        char *tokenSensor = strtok(NULL, " \t");
        char *tokenTipo = strtok(NULL, " \t");
        char *tokenStatus = strtok(NULL, " \t");
        char *tokenBairro = strtok(NULL, " \t");
        int codSensor, tipo, status, codBairro;

        if(tokenSensor == NULL || tokenTipo == NULL || tokenStatus == NULL || tokenBairro == NULL){
            registrarLog("cadastrarSensor - parametros insuficientes", "ERRO");
            return 0;
        }

        codSensor = atoi(tokenSensor);
        tipo = atoi(tokenTipo);
        status = atoi(tokenStatus);
        codBairro = atoi(tokenBairro);

        if(*listaB == NULL || buscarBairro(*listaB, codBairro) == NULL){
            snprintf(bufferLog, sizeof(bufferLog), "cadastrarSensor %s - bairro %d inexistente", tokenSensor, codBairro);
            registrarLog(bufferLog, "ERRO");
            return 0;
        }

        cadastrarSensor(*listaB, codBairro, codSensor, tipo);
        {
            Sensor *sensorCadastrado = buscarSensor(*listaB, codBairro, codSensor);
            if(sensorCadastrado != NULL){
                sensorCadastrado->status = status;
            }
        }
        snprintf(bufferLog, sizeof(bufferLog), "cadastrarSensor %s %s %s %s", tokenSensor, tokenTipo, tokenStatus, tokenBairro);
        registrarLog(bufferLog, "SUCESSO");
        return 0;
    }
    else if(strcmp(comando, "cadastrarEquipe") == 0){
        char *tokenId = strtok(NULL, " \t");
        char *tokenNome = strtok(NULL, " \t");
        char *tokenEsp = strtok(NULL, " \t");

        if(tokenId == NULL || tokenNome == NULL || tokenEsp == NULL){
            registrarLog("cadastrarEquipe - parametros insuficientes", "ERRO");
            return 0;
        }

        if(*listaE != NULL && buscarEquipe(*listaE, atoi(tokenId)) != NULL){
            snprintf(bufferLog, sizeof(bufferLog), "cadastrarEquipe %s %s - ID ja existente", tokenId, tokenNome);
            registrarLog(bufferLog, "ERRO");
            return 0;
        }

        snprintf(especialidade, sizeof(especialidade), "%s", tokenEsp);
        cadastrarEquipe(listaE, atoi(tokenId), tokenNome, especialidade);
        snprintf(bufferLog, sizeof(bufferLog), "cadastrarEquipe %s %s %s", tokenId, tokenNome, tokenEsp);
        registrarLog(bufferLog, "SUCESSO");
        return 0;
    }
    else if(strcmp(comando, "registrarOcorrencia") == 0){
        char *tokenOcorr = strtok(NULL, " \t");
        char *tokenSev = strtok(NULL, " \t");
        char *tokenStatus = strtok(NULL, " \t");
        char *tokenSensor = strtok(NULL, " \t");
        char *tokenBairro = strtok(NULL, " \t");
        char *tokenDesc = strtok(NULL, " \t");
        int codOcorr, severidade, status, codSensor, codBairro;
        Ocorrencia *ocorrencia;

        if(tokenOcorr == NULL || tokenSev == NULL || tokenStatus == NULL || tokenSensor == NULL || tokenBairro == NULL || tokenDesc == NULL){
            registrarLog("registrarOcorrencia - parametros insuficientes", "ERRO");
            return 0;
        }

        codOcorr = atoi(tokenOcorr);
        severidade = atoi(tokenSev);
        status = atoi(tokenStatus);
        codSensor = atoi(tokenSensor);
        codBairro = atoi(tokenBairro);

        if(buscarOcorrenciaGlobal(*listaB, codOcorr) != NULL){
            snprintf(bufferLog, sizeof(bufferLog), "registrarOcorrencia %s - ocorrencia ja existente", tokenOcorr);
            registrarLog(bufferLog, "ERRO");
            return 0;
        }

        if(*listaB == NULL || buscarSensor(*listaB, codBairro, codSensor) == NULL){
            snprintf(bufferLog, sizeof(bufferLog), "registrarOcorrencia %s - sensor %d no bairro %d inexistente", tokenOcorr, codSensor, codBairro);
            registrarLog(bufferLog, "ERRO");
            return 0;
        }

        registrarOcorrencia(*listaB, codBairro, codSensor, codOcorr, severidade, tokenDesc);
        ocorrencia = buscarOcorrencia(*listaB, codBairro, codSensor, codOcorr);
        if(ocorrencia != NULL){
            ocorrencia->status = status;
        }

        snprintf(bufferLog, sizeof(bufferLog), "registrarOcorrencia %s %s %s %s %s %s",
                 tokenOcorr, tokenSev, tokenStatus, tokenSensor, tokenBairro, tokenDesc);
        registrarLog(bufferLog, "SUCESSO");
        return 0;
    }
    else if(strcmp(comando, "gerarChamado") == 0){
        char *tokenChamado = strtok(NULL, " \t");
        char *tokenOcorr = strtok(NULL, " \t");
        char *tokenPrioridade = strtok(NULL, " \t");
        char *tokenStatus = strtok(NULL, " \t");
        int codChamado, codOcorr, prioridade, status;

        if(tokenChamado == NULL || tokenOcorr == NULL || tokenPrioridade == NULL || tokenStatus == NULL){
            registrarLog("gerarChamado - parametros insuficientes", "ERRO");
            return 0;
        }

        codChamado = atoi(tokenChamado);
        codOcorr = atoi(tokenOcorr);
        prioridade = atoi(tokenPrioridade);
        status = atoi(tokenStatus);

        if(gerarChamado(listaC, *listaB, codChamado, codOcorr, prioridade, status)){
            snprintf(bufferLog, sizeof(bufferLog), "gerarChamado %s %s %s %s",
                     tokenChamado, tokenOcorr, tokenPrioridade, tokenStatus);
            registrarLog(bufferLog, "SUCESSO");
        }
        else{
            snprintf(bufferLog, sizeof(bufferLog), "gerarChamado %s %s - falha ao gerar chamado", tokenChamado, tokenOcorr);
            registrarLog(bufferLog, "ERRO");
        }
        return 0;
    }
    else if(strcmp(comando, "associarEquipe") == 0){
        char *tokenChamado = strtok(NULL, " \t");
        char *tokenEquipe = strtok(NULL, " \t");

        if(tokenChamado == NULL || tokenEquipe == NULL){
            registrarLog("associarEquipe - parametros insuficientes", "ERRO");
            return 0;
        }

        if(associarChamadoEquipe(*listaE, atoi(tokenChamado), atoi(tokenEquipe), listaC)){
            snprintf(bufferLog, sizeof(bufferLog), "associarEquipe %s %s", tokenChamado, tokenEquipe);
            registrarLog(bufferLog, "SUCESSO");
        }
        else{
            snprintf(bufferLog, sizeof(bufferLog), "associarEquipe %s %s - falha na associacao", tokenChamado, tokenEquipe);
            registrarLog(bufferLog, "ERRO");
        }
        return 0;
    }
    else if(strcmp(comando, "alterarStatusSensor") == 0){
        char *tokenSensor = strtok(NULL, " \t");
        char *tokenBairro = strtok(NULL, " \t");
        char *tokenStatus = strtok(NULL, " \t");

        if(tokenSensor == NULL || tokenBairro == NULL || tokenStatus == NULL){
            registrarLog("alterarStatusSensor - parametros insuficientes", "ERRO");
            return 0;
        }

        if(alterarStatusSensor(*listaB, atoi(tokenSensor), atoi(tokenBairro), atoi(tokenStatus))){
            snprintf(bufferLog, sizeof(bufferLog), "alterarStatusSensor %s %s %s", tokenSensor, tokenBairro, tokenStatus);
            registrarLog(bufferLog, "SUCESSO");
        }
        else{
            snprintf(bufferLog, sizeof(bufferLog), "alterarStatusSensor %s %s %s - sensor inexistente", tokenSensor, tokenBairro, tokenStatus);
            registrarLog(bufferLog, "ERRO");
        }
        return 0;
    }
    else if(strcmp(comando, "listarSensoresBairro") == 0){
        char *tokenBairro = strtok(NULL, " \t");
        Bairro *bairro;

        if(tokenBairro == NULL){
            registrarLog("listarSensoresBairro - parametros insuficientes", "ERRO");
            return 0;
        }

        bairro = (*listaB != NULL) ? buscarBairro(*listaB, atoi(tokenBairro)) : NULL;
        if(bairro == NULL || bairro->listaSensores == NULL){
            snprintf(bufferLog, sizeof(bufferLog), "listarSensoresBairro %s - bairro ou sensores inexistentes", tokenBairro);
            registrarLog(bufferLog, "ERRO");
            return 0;
        }

        listarSensoresBairro(bairro);
        snprintf(bufferLog, sizeof(bufferLog), "listarSensoresBairro %s", tokenBairro);
        registrarLog(bufferLog, "SUCESSO");
        return 0;
    }
    else if(strcmp(comando, "listarOcorrencias") == 0){
        listarTodasOcorrencias(*listaB);
        registrarLog("listarOcorrencias", "SUCESSO");
        return 0;
    }
    else if(strcmp(comando, "finalizarChamado") == 0){
        char *tokenChamado = strtok(NULL, " \t");

        if(tokenChamado == NULL){
            registrarLog("finalizarChamado - parametros insuficientes", "ERRO");
            return 0;
        }

        if(finalizarChamado(*listaC, *listaE, atoi(tokenChamado))){
            snprintf(bufferLog, sizeof(bufferLog), "finalizarChamado %s", tokenChamado);
            registrarLog(bufferLog, "SUCESSO");
        }
        else{
            snprintf(bufferLog, sizeof(bufferLog), "finalizarChamado %s - chamado inexistente ou ja finalizado", tokenChamado);
            registrarLog(bufferLog, "ERRO");
        }
        return 0;
    }
    else if(strcmp(comando, "relatorioGeral") == 0){
        relatorioGeral(*listaB, *listaE, *listaC);
        registrarLog("relatorioGeral", "SUCESSO");
        return 0;
    }
    else if(strcmp(comando, "FIM") == 0){
        registrarLog("FIM - simulacao encerrada", "SUCESSO");
        return 1;
    }
    else{
        snprintf(bufferLog, sizeof(bufferLog), "Comando desconhecido: %s", comando);
        registrarLog(bufferLog, "ERRO");
    }

    return 0;
}

void executarSimulacao(char *nomeArquivo, Bairro **listaB, Equipe **listaE, Chamado **listaC){
    FILE *entrada;
    char linha[512];
    char linhaCopia[512];

    arquivoLog = fopen(ARQUIVO_LOG, "w");
    if(arquivoLog == NULL){
        printf("ERRO: Nao foi possivel criar %s. . .\n", ARQUIVO_LOG);
        return;
    }

    registrarLog("Inicio da simulacao", "INFO");

    entrada = fopen(nomeArquivo, "r");
    if(entrada == NULL){
        snprintf(linhaCopia, sizeof(linhaCopia), "Nao foi possivel abrir %s", nomeArquivo);
        registrarLog(linhaCopia, "ERRO");
        fecharLog();
        return;
    }

    while(fgets(linha, sizeof(linha), entrada) != NULL){
        trimFinal(linha);
        if(linhaVazia(linha)){
            continue;
        }

        strncpy(linhaCopia, linha, sizeof(linhaCopia) - 1);
        linhaCopia[sizeof(linhaCopia) - 1] = '\0';

        if(processarLinha(linhaCopia, listaB, listaE, listaC)){
            break;
        }
    }

    fclose(entrada);
    registrarLog("Simulacao concluida", "INFO");
    fecharLog();
}

static void verificarChamadoConsistencia(Chamado *chamado, Bairro *listaB, Equipe *listaE, int *consistente, char *mensagem, size_t tamanhoMensagem) {
    if (chamado == NULL) {
        return;
    }

    if (chamado->ocorrencia != NULL && buscarOcorrenciaPorCodigo(listaB, chamado->ocorrencia->codigo) == NULL) {
        snprintf(mensagem, tamanhoMensagem, "Chamado [%d] aponta para ocorrencia invalida [%d]",
                 chamado->codigo, chamado->ocorrencia->codigo);
        registrarLog(mensagem, "INCONSISTENCIA");
        *consistente = 0;
    }

    if (chamado->equipe != NULL && buscarEquipe(listaE, chamado->equipe->codigo) == NULL) {
        snprintf(mensagem, tamanhoMensagem, "Chamado [%d] associado a equipe invalida [%d]",
                 chamado->codigo, chamado->equipe->codigo);
        registrarLog(mensagem, "INCONSISTENCIA");
        *consistente = 0;
    }
}

int verificarConsistenciaDados(Bairro *listaB, Equipe *listaE, Chamado *listaC){
    int consistente = 1;
    char mensagem[256];

    Chamado *aux = listaC;
    while (aux != NULL) {
        verificarChamadoConsistencia(aux, listaB, listaE, &consistente, mensagem, sizeof(mensagem));
        aux = aux->prox;
    }

    Equipe *eq = listaE;
    while (eq != NULL) {
        aux = eq->listaChamados;
        while (aux != NULL) {
            verificarChamadoConsistencia(aux, listaB, listaE, &consistente, mensagem, sizeof(mensagem));
            aux = aux->prox;
        }
        eq = eq->prox;
    }

    if(consistente){
        registrarLog("Verificacao de consistencia concluida sem erros", "SUCESSO");
    }
    else{
        registrarLog("Verificacao de consistencia encontrou problemas", "ERRO");
    }

    return consistente;
}

void liberarMemoriaGeral(Bairro **listaB, Equipe **listaE, Chamado **listaC){
    liberarChamados(listaC);
    liberarEquipes(listaE);
    liberarBairro(listaB);
    printf("Memoria geral liberada com sucesso. . .\n");
}
