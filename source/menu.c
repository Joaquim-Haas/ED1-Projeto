#include "menu.h"
#include "simulador.h"
#include <stdio.h>
#include <stdlib.h>

static void pausar(void) {
    printf("\nPressione Enter para continuar. . .");
    getchar();
}

static void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

static void menuBairros(Bairro **listaB) {
    int opcao = -1;

    while (opcao != 0) {
        printf("\n--- Gerenciar Bairros ---\n");
        printf("1. Cadastrar bairro\n");
        printf("2. Listar bairros\n");
        printf("3. Remover bairro\n");
        printf("0. Voltar\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 1) {
            int codigo;
            char nome[50];
            int erro = 0;
            printf("Codigo do bairro: ");
            scanf("%d", &codigo);
            printf("Nome do bairro: ");
            scanf("%49s", nome);
            limparBuffer();
            
            // Pré-checagem de erro
            if (*listaB != NULL && buscarBairro(*listaB, codigo) != NULL) erro = 1;
            
            cadastrarBairro(listaB, codigo, nome);
            
            if (erro) registrarLog("Operacao Manual: Cadastro Bairro - Falha (ID Duplicado)", "ERRO");
            else registrarLog("Operacao Manual: Cadastro Bairro", "SUCESSO");

        } else if (opcao == 2) {
            if (*listaB == NULL) registrarLog("Operacao Manual: Listar Bairros - Falha (Lista Vazia)", "ERRO");
            else registrarLog("Operacao Manual: Listar Bairros", "SUCESSO");
            
            listarBairros(*listaB);
            
        } else if (opcao == 3) {
            int codigo;
            int erro = 0;
            Bairro *b = NULL;
            printf("Codigo do bairro a remover: ");
            scanf("%d", &codigo);
            limparBuffer();
            
            // Pré-checagem de erro
            if (*listaB != NULL) b = buscarBairro(*listaB, codigo);
            if (b == NULL) erro = 1;
            else if (b->listaSensores != NULL) erro = 2;

            removerBairro(listaB, codigo);
            
            if (erro == 1) registrarLog("Operacao Manual: Remocao Bairro - Falha (Nao encontrado)", "ERRO");
            else if (erro == 2) registrarLog("Operacao Manual: Remocao Bairro - Falha (Possui sensores)", "ERRO");
            else registrarLog("Operacao Manual: Remocao Bairro", "SUCESSO");

        } else if (opcao != 0) {
            printf("Opcao invalida!\n");
        }
    }
}

static void menuSensores(Bairro *listaB, Equipe *listaE, Chamado **listaC) {
    int opcao = -1;

    while (opcao != 0) {
        printf("\n--- Gerenciar Sensores ---\n");
        printf("1. Cadastrar sensor\n");
        printf("2. Listar sensores de um bairro\n");
        printf("3. Alterar status do sensor\n");
        printf("0. Voltar\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 1) {
            int codBairro, codSensor, tipo;
            int erro = 0;
            printf("Codigo do bairro: ");
            scanf("%d", &codBairro);
            printf("Codigo do sensor: ");
            scanf("%d", &codSensor);
            printf("Tipo do sensor: ");
            scanf("%d", &tipo);
            limparBuffer();
            
            // Pré-checagem
            if (listaB == NULL || buscarBairro(listaB, codBairro) == NULL) erro = 1;

            cadastrarSensor(listaB, codBairro, codSensor, tipo);
            
            if (erro) registrarLog("Operacao Manual: Cadastro Sensor - Falha (Bairro inexistente)", "ERRO");
            else registrarLog("Operacao Manual: Cadastro Sensor", "SUCESSO");

        } else if (opcao == 2) {
            int codBairro;
            Bairro *bairro;
            printf("Codigo do bairro: ");
            scanf("%d", &codBairro);
            limparBuffer();
            bairro = buscarBairro(listaB, codBairro);
            if (bairro == NULL || bairro->listaSensores == NULL) {
                printf("ERRO: Bairro inexistente ou sem sensores cadastrados.\n");
                registrarLog("Operacao Manual: Listar Sensores - Falha (Bairro/Sensores inexistentes)", "ERRO");
            } else {
                listarSensoresBairro(bairro);
                registrarLog("Operacao Manual: Listar Sensores", "SUCESSO");
            }
            
        } else if (opcao == 3) {
            int codBairro, codSensor, novoStatus;
            Sensor *sensor;
            printf("Codigo do bairro: ");
            scanf("%d", &codBairro);
            printf("Codigo do sensor: ");
            scanf("%d", &codSensor);
            printf("Novo status (1=online, 0=offline): ");
            scanf("%d", &novoStatus);
            limparBuffer();
            
            if (alterarStatusSensor(listaB, codSensor, codBairro, novoStatus)) {
                sensor = buscarSensor(listaB, codBairro, codSensor);
                verificarGeracaoAutomatica(listaB, listaE, listaC, sensor, NULL);
                registrarLog("Operacao Manual: Alteracao de Status de Sensor", "SUCESSO");
            } else {
                registrarLog("Operacao Manual: Alteracao de Status - Falha (Sensor/Bairro invalido)", "ERRO");
            }
        } else if (opcao != 0) {
            printf("Opcao invalida!\n");
        }
    }
}

static void registrarOcorrenciaManual(Bairro *listaB, Equipe *listaE, Chamado **listaC) {
    int codBairro, codSensor, codOcorr, severidade;
    char descricao[100];
    int erro = 0;
    Ocorrencia *ocorrencia;
    Sensor *sensor;

    printf("\n--- Registrar Ocorrencia Manual ---\n");
    printf("Codigo do bairro: ");
    scanf("%d", &codBairro);
    printf("Codigo do sensor: ");
    scanf("%d", &codSensor);
    printf("Codigo da ocorrencia: ");
    scanf("%d", &codOcorr);
    printf("Severidade (1 a 5): ");
    scanf("%d", &severidade);
    printf("Descricao (uma palavra): ");
    scanf("%99s", descricao);
    limparBuffer();

    if (listaB == NULL || buscarSensor(listaB, codBairro, codSensor) == NULL) erro = 1;

    registrarOcorrencia(listaB, codBairro, codSensor, codOcorr, severidade, descricao);
    
    if (erro) {
        registrarLog("Operacao Manual: Registro Ocorrencia - Falha (Sensor/Bairro inexistente)", "ERRO");
    } else {
        ocorrencia = buscarOcorrencia(listaB, codBairro, codSensor, codOcorr);
        sensor = buscarSensor(listaB, codBairro, codSensor);
        if (ocorrencia != NULL) {
            verificarGeracaoAutomatica(listaB, listaE, listaC, sensor, ocorrencia);
            registrarLog("Operacao Manual: Registro Ocorrencia", "SUCESSO");
        }
    }
}

static void listarChamadosPendentes(Chamado *listaC, Equipe *listaE) {
    int encontrou = 0;

    printf("\n--- Chamados nao atribuidos ---\n");
    Chamado *aux = listaC;
    while (aux != NULL) {
        printf("Chamado [%d] - Prioridade [%d] - Status [%d]", aux->codigo, aux->prioridade, aux->status);
        if (aux->ocorrencia != NULL) {
            printf(" - Ocorrencia [%d]", aux->ocorrencia->codigo);
        }
        printf("\n");
        encontrou = 1;
        aux = aux->prox;
    }

    printf("\n--- Chamados por equipe ---\n");
    Equipe *eq = listaE;
    while (eq != NULL) {
        aux = eq->listaChamados;
        while (aux != NULL) {
            printf("Equipe [%s] - Chamado [%d] - Prioridade [%d] - Status [%d]",
                   eq->nome, aux->codigo, aux->prioridade, aux->status);
            if (aux->ocorrencia != NULL) {
                printf(" - Ocorrencia [%d]", aux->ocorrencia->codigo);
            }
            printf("\n");
            encontrou = 1;
            aux = aux->prox;
        }
        eq = eq->prox;
    }

    if (!encontrou) {
        printf("Nenhum chamado cadastrado.\n");
        registrarLog("Operacao Manual: Listar Chamados - Falha (Nenhum cadastrado)", "ERRO");
    } else {
        registrarLog("Operacao Manual: Listar Chamados", "SUCESSO");
    }
}

static void menuEquipesChamados(Bairro *listaB, Equipe **listaE, Chamado **listaC) {
    int opcao = -1;

    while (opcao != 0) {
        printf("\n--- Equipes e Chamados ---\n");
        printf("1. Cadastrar equipe\n");
        printf("2. Listar equipes\n");
        printf("3. Remover equipe\n");
        printf("4. Gerar chamado manual\n");
        printf("5. Associar chamado a equipe\n");
        printf("6. Finalizar chamado\n");
        printf("7. Listar chamados\n");
        printf("0. Voltar\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 1) {
            int id;
            char nome[50], especialidade[30];
            int erro = 0;
            printf("Codigo da equipe: ");
            scanf("%d", &id);
            printf("Nome: ");
            scanf("%49s", nome);
            printf("Especialidade (tipo de sensor): ");
            scanf("%29s", especialidade);
            limparBuffer();
            
            if (*listaE != NULL && buscarEquipe(*listaE, id) != NULL) erro = 1;
            
            cadastrarEquipe(listaE, id, nome, especialidade);
            
            if (erro) registrarLog("Operacao Manual: Cadastro Equipe - Falha (ID Duplicado)", "ERRO");
            else registrarLog("Operacao Manual: Cadastro Equipe", "SUCESSO");

        } else if (opcao == 2) {
            if (*listaE == NULL) registrarLog("Operacao Manual: Listar Equipes - Falha (Vazio)", "ERRO");
            else registrarLog("Operacao Manual: Listar Equipes", "SUCESSO");
            listarEquipes(*listaE);
            
        } else if (opcao == 3) {
            int id;
            int erro = 0;
            printf("Codigo da equipe a remover: ");
            scanf("%d", &id);
            limparBuffer();
            
            if (*listaE == NULL || buscarEquipe(*listaE, id) == NULL) erro = 1;
            
            removerEquipe(listaE, id);
            
            if (erro) registrarLog("Operacao Manual: Remover Equipe - Falha (Nao encontrada)", "ERRO");
            else registrarLog("Operacao Manual: Remover Equipe", "SUCESSO");

        } else if (opcao == 4) {
            int codChamado, codOcorr, prioridade, status;
            printf("Codigo do chamado: ");
            scanf("%d", &codChamado);
            printf("Codigo da ocorrencia (0 se nenhuma): ");
            scanf("%d", &codOcorr);
            printf("Prioridade: ");
            scanf("%d", &prioridade);
            printf("Status: ");
            scanf("%d", &status);
            limparBuffer();
            if(gerarChamado(listaC, listaB, codChamado, codOcorr, prioridade, status)) {
                registrarLog("Operacao Manual: Gerar Chamado", "SUCESSO");
            } else {
                registrarLog("Operacao Manual: Gerar Chamado - Falha (Invalido ou Duplicado)", "ERRO");
            }
        } else if (opcao == 5) {
            int codChamado, codEquipe;
            printf("Codigo do chamado: ");
            scanf("%d", &codChamado);
            printf("Codigo da equipe: ");
            scanf("%d", &codEquipe);
            limparBuffer();
            if(associarChamadoEquipe(*listaE, codChamado, codEquipe, listaC)) {
                registrarLog("Operacao Manual: Associar Chamado", "SUCESSO");
            } else {
                registrarLog("Operacao Manual: Associar Chamado - Falha", "ERRO");
            }
        } else if (opcao == 6) {
            int codChamado;
            printf("Codigo do chamado a finalizar: ");
            scanf("%d", &codChamado);
            limparBuffer();
            if(finalizarChamado(*listaC, *listaE, codChamado)) {
                registrarLog("Operacao Manual: Finalizar Chamado", "SUCESSO");
            } else {
                registrarLog("Operacao Manual: Finalizar Chamado - Falha", "ERRO");
            }
        } else if (opcao == 7) {
            listarChamadosPendentes(*listaC, *listaE);
        } else if (opcao != 0) {
            printf("Opcao invalida!\n");
        }
    }
}

static void menuRelatorios(Bairro *listaB, Equipe *listaE, Chamado *listaC) {
    int opcao = -1;

    while (opcao != 0) {
        printf("\n--- Relatorios ---\n");
        printf("1. Bairros com mais ocorrencias\n");
        printf("2. Sensores offline\n");
        printf("3. Ocorrencias criticas abertas\n");
        printf("4. Equipe com mais atendimentos\n");
        printf("5. Quantidade de sensores por bairro\n");
        printf("6. Ocorrencias por severidade\n");
        printf("7. Relatorio geral\n");
        printf("0. Voltar\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1: relatorioBairrosMaisOcorrencias(listaB); break;
            case 2: relatorioSensoresOffline(listaB); break;
            case 3: relatorioOcorrenciasCriticasAbertas(listaB); break;
            case 4: relatorioEquipeMaisAtendimentos(listaE); break;
            case 5: relatorioQuantidadeSensoresBairro(listaB); break;
            case 6: relatorioOcorrenciasPorSeveridade(listaB); break;
            case 7: relatorioGeral(listaB, listaE, listaC); break;
            case 0: break;
            default: printf("Opcao invalida!\n");
        }

        if (opcao != 0) {
            registrarLog("Operacao Manual: Visualizar Relatorio", "INFO");
            pausar();
        }
    }
}

void carregarDadosIniciais(Bairro **listaB, Equipe **listaE, Chamado **listaC) {
    carregarBairrosArquivo(listaB);
    carregarSensoresArquivo(*listaB);
    carregarOcorrenciasArquivo(*listaB);
    carregarEquipesArquivo(listaE);
    carregarChamadosArquivo(listaC, *listaE, *listaB);
}

void salvarDadosNosArquivos(Bairro *listaB, Equipe *listaE, Chamado *listaC) {
    salvarBairrosArquivo(listaB);
    salvarSensoresArquivo(listaB);
    salvarOcorrenciasArquivo(listaB);
    salvarEquipesArquivo(listaE);
    salvarChamadosArquivo(listaC, listaE);
}

void exibirMenuInterativo(Bairro **listaB, Equipe **listaE, Chamado **listaC) {
    int opcao = -1;

    do {
        printf("\n=== SMARTCITY: MONITORAMENTO URBANO ===\n");
        printf("1. Cadastrar/Gerenciar Bairros\n");
        printf("2. Cadastrar/Gerenciar Sensores\n");
        printf("3. Registrar Ocorrencia Manual\n");
        printf("4. Gerenciar Equipes e Chamados\n");
        printf("5. Visualizar Relatorios no Terminal\n");
        printf("6. Inserir dados de arquivos.txt\n");
        printf("0. Salvar e Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                menuBairros(listaB);
                break;
            case 2:
                menuSensores(*listaB, *listaE, listaC);
                break;
            case 3:
                registrarOcorrenciaManual(*listaB, *listaE, listaC);
                break;
            case 4:
                menuEquipesChamados(*listaB, listaE, listaC);
                break;
            case 5:
                menuRelatorios(*listaB, *listaE, *listaC);
                break;
            case 6:
                printf("\n--- Carregando dados dos arquivos .txt ---\n");
                carregarDadosIniciais(listaB, listaE, listaC); 
                printf("\nDados carregados com sucesso!\n");
                registrarLog("Operacao Manual: Carga Inicial de Arquivos", "SUCESSO");
                pausar(); 
                break;
            case 0:
                printf("Salvando dados e encerrando...\n");
                salvarDadosNosArquivos(*listaB, *listaE, *listaC);
                registrarLog("FIM - Sessao Manual Encerrada", "INFO");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
}