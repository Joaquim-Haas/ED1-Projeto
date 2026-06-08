#include "operacoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// funcao auxiliar para encontrar um codigo chamado auto-incrementado unico
static int obterNovoCodigoChamado(Chamado *listaChamadosGeral, Equipe *listaEquipes) {
    int max = 8000; // código inicial padrao
    Chamado *aux = listaChamadosGeral;
    while (aux != NULL) {
        if (aux->codigo > max) max = aux->codigo;
        aux = aux->prox;
    }
    Equipe *eq = listaEquipes;
    while (eq != NULL) {
        aux = eq->listaChamados;
        while (aux != NULL) {
            if (aux->codigo > max) max = aux->codigo;
            aux = aux->prox;
        }
        eq = eq->prox;
    }
    return max + 1;
}

// funcao que localiza um sensor dado o seu ponteiro de ocorrencia
static Sensor *buscarSensorPorOcorrencia(Bairro *listaBairros, Ocorrencia *ocorrencia) {
    if (listaBairros == NULL || ocorrencia == NULL) return NULL;
    Bairro *b = listaBairros->prox;
    do {
        if (b->listaSensores != NULL) {
            Sensor *s = b->listaSensores->prox;
            do {
                if (s->listaOcorrencias != NULL) {
                    Ocorrencia *o = s->listaOcorrencias->prox;
                    do {
                        if (o == ocorrencia) return s;
                        o = o->prox;
                    } while (o != s->listaOcorrencias->prox);
                }
                s = s->prox;
            } while (s != b->listaSensores->prox);
        }
        b = b->prox;
    } while (b != listaBairros->prox);
    return NULL;
}

// ==========================================
//             equipe crud
// ==========================================

void cadastrarEquipe(Equipe **topo, int id, char *nome, char *especialidade) {
    if (buscarEquipe(*topo, id) != NULL) {
        printf("ERRO: Ja existe uma equipe cadastrada com o codigo [%d]. . .\n", id);
        return;
    }

    Equipe *novo = (Equipe *)malloc(sizeof(Equipe));
    if (novo == NULL) {
        printf("ERRO: Falhou ao alocar memoria para Equipe. . .\n");
        return;
    }

    novo->codigo = id;
    strcpy(novo->nome, nome);
    strcpy(novo->especialidade, especialidade);
    novo->total_atendimentos = 0;
    novo->listaChamados = NULL;
    novo->prox = NULL;

    // lista encadeada linear insere no final para preservar a ordem de carregamento
    if (*topo == NULL) {
        *topo = novo;
    } else {
        Equipe *aux = *topo;
        while (aux->prox != NULL) {
            aux = aux->prox;
        }
        aux->prox = novo;
    }

    printf("Equipe [%s] cadastrada com sucesso. . .\n", nome);
}

Equipe *buscarEquipe(Equipe *listaEquipes, int codigo) {
    Equipe *aux = listaEquipes;
    while (aux != NULL) {
        if (aux->codigo == codigo) {
            return aux;
        }
        aux = aux->prox;
    }
    return NULL;
}

void listarEquipes(Equipe *listaEquipes) {
    if (listaEquipes == NULL) {
        printf("Nao ha nenhuma equipe cadastrada. . .\n");
        return;
    }
    printf("\n- Listando Equipes -\n");
    Equipe *aux = listaEquipes;
    while (aux != NULL) {
        printf("Equipe ID: [%d] - Nome: [%s] - Especialidade: [%s] - Total de Atendimentos: [%d]\n",
               aux->codigo, aux->nome, aux->especialidade, aux->total_atendimentos);
        aux = aux->prox;
    }
}

void removerEquipe(Equipe **listaEquipes, int codigo) {
    Equipe *aux = *listaEquipes;
    Equipe *ant = NULL;

    while (aux != NULL && aux->codigo != codigo) {
        ant = aux;
        aux = aux->prox;
    }

    if (aux == NULL) {
        printf("ERRO: Equipe com o codigo [%d] nao encontrada para remocao.\n", codigo);
        return;
    }

    if (ant == NULL) {
        *listaEquipes = aux->prox;
    } else {
        ant->prox = aux->prox;
    }

    // Free associado a lista chamados 
    Chamado *c = aux->listaChamados;
    while (c != NULL) {
        Chamado *temp = c->prox;
        free(c);
        c = temp;
    }

    printf("Equipe [%s] (ID: %d) removida com sucesso.\n", aux->nome, aux->codigo);
    free(aux);
}

// ==========================================
//             CHAMADO CRUD
// ==========================================

Chamado *alocarChamado(int codigo, int prioridade, int status, Ocorrencia *ocorrencia) {
    Chamado *novo = (Chamado *)malloc(sizeof(Chamado));
    if (novo == NULL) {
        printf("ERRO: Falhou ao alocar memoria para Chamado. . .\n");
        return NULL;
    }
    novo->codigo = codigo;
    novo->prioridade = prioridade;
    novo->status = status;
    novo->ocorrencia = ocorrencia;
    novo->prox = NULL;
    return novo;
}

void cadastrarChamado(Chamado **listaChamadosGeral, int codigo, int prioridade, int status, Ocorrencia *ocorrencia) {
    // virificando duplicados
    if (buscarChamado(*listaChamadosGeral, NULL, codigo) != NULL) {
        printf("ERRO: Ja existe um chamado cadastrado com o codigo [%d]. . .\n", codigo);
        return;
    }

    Chamado *novo = alocarChamado(codigo, prioridade, status, ocorrencia);
    if (novo == NULL) return;

    // lista encadeada: insere fim
    if (*listaChamadosGeral == NULL) {
        *listaChamadosGeral = novo;
    } else {
        Chamado *aux = *listaChamadosGeral;
        while (aux->prox != NULL) {
            aux = aux->prox;
        }
        aux->prox = novo;
    }
    printf("Chamado [%d] cadastrado com sucesso na lista de chamados gerais. . .\n", codigo);
}

Chamado *buscarChamado(Chamado *listaChamadosGeral, Equipe *listaEquipes, int codigo) {
    // 1-> busca no lista global de não atribuidos
    Chamado *aux = listaChamadosGeral;
    while (aux != NULL) {
        if (aux->codigo == codigo) return aux;
        aux = aux->prox;
    }
    // 2-> buscar em todas as listas de equipes
    Equipe *eq = listaEquipes;
    while (eq != NULL) {
        aux = eq->listaChamados;
        while (aux != NULL) {
            if (aux->codigo == codigo) return aux;
            aux = aux->prox;
        }
        eq = eq->prox;
    }
    return NULL;
}

int associarChamadoEquipe(Equipe *listaEquipes, int codChamado, int codEquipe, Chamado **listaChamadosGeral) {
    // encontra o número chamado na lista global de chamadas não atribuídas.
    Chamado *aux = *listaChamadosGeral;
    Chamado *ant = NULL;

    while (aux != NULL && aux->codigo != codChamado) {
        ant = aux;
        aux = aux->prox;
    }

    if (aux == NULL) {
        // verifica se ja esta atribuido a alguma equipe.
        Chamado *jaAssoc = buscarChamado(NULL, listaEquipes, codChamado);
        if (jaAssoc != NULL) {
            printf("ERRO: Chamado [%d] ja esta associado a uma equipe.\n", codChamado);
        } else {
            printf("ERRO: Chamado [%d] nao encontrado na lista de chamados gerais.\n", codChamado);
        }
        return 0;
    }

    // busca equipe
    Equipe *eq = buscarEquipe(listaEquipes, codEquipe);
    if (eq == NULL) {
        printf("ERRO: Equipe [%d] nao existe. Nao foi possivel associar o chamado.\n", codEquipe);
        return 0;
    }

    // remove da lista global
    if (ant == NULL) {
        *listaChamadosGeral = aux->prox;
    } else {
        ant->prox = aux->prox;
    }

    // adicionar à lista da equipe
    aux->prox = NULL;
    if (eq->listaChamados == NULL) {
        eq->listaChamados = aux;
    } else {
        Chamado *last = eq->listaChamados;
        while (last->prox != NULL) {
            last = last->prox;
        }
        last->prox = aux;
    }

    // define status da chamada como atribuído/em andamento
    aux->status = 2;

    // define o status de ocorrência como associado se existir
    if (aux->ocorrencia != NULL) {
        aux->ocorrencia->status = 2;
    }

    printf("Chamado [%d] associado com sucesso a Equipe [%s] (ID: %d).\n", codChamado, eq->nome, eq->codigo);
    return 1;
}

int gerarChamado(Chamado **listaChamadosGeral, Bairro *listaBairros, int codigo, int codOcorrencia, int prioridade, int status) {
    Ocorrencia *ocorr = NULL;

    if (buscarChamado(*listaChamadosGeral, NULL, codigo) != NULL) {
        printf("ERRO: Ja existe um chamado cadastrado com o codigo [%d]. . .\n", codigo);
        return 0;
    }

    if (codOcorrencia > 0) {
        ocorr = buscarOcorrenciaPorCodigo(listaBairros, codOcorrencia);
        if (ocorr == NULL) {
            printf("ERRO: Ocorrencia [%d] nao encontrada.\n", codOcorrencia);
            return 0;
        }
    }

    cadastrarChamado(listaChamadosGeral, codigo, prioridade, status, ocorr);
    return buscarChamado(*listaChamadosGeral, NULL, codigo) != NULL;
}

int finalizarChamado(Chamado *listaChamadosGeral, Equipe *listaEquipes, int codChamado) {
    // Encontre o item chamado (verificando ambas as listas)
    Chamado *chamadoAchado = NULL;
    Equipe *equipeDoChamado = NULL;

    // busca em chamadas não atribuídas
    Chamado *aux = listaChamadosGeral;
    while (aux != NULL) {
        if (aux->codigo == codChamado) {
            chamadoAchado = aux;
            break;
        }
        aux = aux->prox;
    }

    // caso não encontre, procure em equipes.
    if (chamadoAchado == NULL) {
        Equipe *eq = listaEquipes;
        while (eq != NULL) {
            aux = eq->listaChamados;
            while (aux != NULL) {
                if (aux->codigo == codChamado) {
                    chamadoAchado = aux;
                    equipeDoChamado = eq;
                    break;
                }
                aux = aux->prox;
            }
            if (chamadoAchado != NULL) break;
            eq = eq->prox;
        }
    }

    if (chamadoAchado == NULL) {
        printf("ERRO: Chamado [%d] nao encontrado.\n", codChamado);
        return 0;
    }

    // alterar o status para concluído/resolvido (3)
    chamadoAchado->status = 3;

    // atualizar ocorrência associada para finalizada (3)
    if (chamadoAchado->ocorrencia != NULL) {
        chamadoAchado->ocorrencia->status = 3;
        printf("Ocorrencia [%d] finalizada.\n", chamadoAchado->ocorrencia->codigo);
    }

    // se atribuído a uma equipe, incrementa o total de chamadas concluídas da equipe.
    if (equipeDoChamado != NULL) {
        equipeDoChamado->total_atendimentos++;
        printf("Chamado [%d] finalizado pela Equipe [%s]. Total de atendimentos da equipe: %d\n",
               codChamado, equipeDoChamado->nome, equipeDoChamado->total_atendimentos);
    } else {
        printf("Chamado [%d] finalizado (nao estava associado a nenhuma equipe).\n", codChamado);
    }

    return 1;
}


void verificarGeracaoAutomatica(Bairro *listaBairros, Equipe *listaEquipes, Chamado **listaChamadosGeral, Sensor *sensor, Ocorrencia *ocorrencia) {
    // Caso 1: O sensor fica OFFLINE (status != 1)
    if (sensor != NULL && sensor->status != 1) {
        int jaTemChamado = 0;
        // pesquisa em chamadas nao atribuidas
        Chamado *c = *listaChamadosGeral;
        while (c != NULL) {
            if (c->ocorrencia == NULL && c->status != 3) {
                jaTemChamado = 1;
                break;
            }
            c = c->prox;
        }
        // pesquisar nas chamadas do Teams
        if (!jaTemChamado) {
            Equipe *eq = listaEquipes;
            while (eq != NULL) {
                c = eq->listaChamados;
                while (c != NULL) {
                    if (c->ocorrencia == NULL && c->status != 3) {
                        jaTemChamado = 1;
                        break;
                    }
                    c = c->prox;
                }
                if (jaTemChamado) break;
                eq = eq->prox;
            }
        }

        if (!jaTemChamado) {
            int novoCod = obterNovoCodigoChamado(*listaChamadosGeral, listaEquipes);
            // chamadas automáticas para sensores offline têm alta prioridade
            Chamado *novo = alocarChamado(novoCod, 3, 1, NULL);
            if (novo != NULL) {
                // encontra a equipe por especialidade.
                Equipe *match = listaEquipes;
                while (match != NULL) {
                    if (atoi(match->especialidade) == sensor->tipo) {
                        break;
                    }
                    match = match->prox;
                }

                if (match != NULL) {
                    novo->status = 2;
                    if (match->listaChamados == NULL) {
                        match->listaChamados = novo;
                    } else {
                        Chamado *last = match->listaChamados;
                        while (last->prox != NULL) {
                            last = last->prox;
                        }
                        last->prox = novo;
                    }
                    printf("[AUTOMATICO] Sensor [%d] offline! Chamado [%d] gerado automaticamente e associado a Equipe [%s].\n",
                           sensor->codigo, novoCod, match->nome);
                } else {
                    // adicionar à lista global
                    if (*listaChamadosGeral == NULL) {
                        *listaChamadosGeral = novo;
                    } else {
                        Chamado *last = *listaChamadosGeral;
                        while (last->prox != NULL) {
                            last = last->prox;
                        }
                        last->prox = novo;
                    }
                    printf("[AUTOMATICO] Sensor [%d] offline! Chamado [%d] gerado automaticamente (sem equipe compativel).\n",
                           sensor->codigo, novoCod);
                }
            }
        }
    }

    // ocorrencia crítica registrada (gravidade >= 4)
    if (ocorrencia != NULL && ocorrencia->severidade >= 4 && ocorrencia->status == 1) {
        // encontre o sensor correspondente para obter seu tipo.
        Sensor *sens = buscarSensorPorOcorrencia(listaBairros, ocorrencia);
        if (sens != NULL) {
            // verifica se já existe uma chamada ativa para esta ocorrência.
            int jaTemChamado = 0;
            Chamado *c = *listaChamadosGeral;
            while (c != NULL) {
                if (c->ocorrencia == ocorrencia && c->status != 3) {
                    jaTemChamado = 1;
                    break;
                }
                c = c->prox;
            }
            if (!jaTemChamado) {
                Equipe *eq = listaEquipes;
                while (eq != NULL) {
                    c = eq->listaChamados;
                    while (c != NULL) {
                        if (c->ocorrencia == ocorrencia && c->status != 3) {
                            jaTemChamado = 1;
                            break;
                        }
                        c = c->prox;
                    }
                    if (jaTemChamado) break;
                    eq = eq->prox;
                }
            }

            if (!jaTemChamado) {
                int novoCod = obterNovoCodigoChamado(*listaChamadosGeral, listaEquipes);
                Chamado *novo = alocarChamado(novoCod, ocorrencia->severidade, 1, ocorrencia);
                if (novo != NULL) {
                    Equipe *match = listaEquipes;
                    while (match != NULL) {
                        if (atoi(match->especialidade) == sens->tipo) {
                            break;
                        }
                        match = match->prox;
                    }

                    if (match != NULL) {                   
                        novo->status = 2; 
                        ocorrencia->status = 2;
                        if (match->listaChamados == NULL) {
                            match->listaChamados = novo;
                        } else {
                            Chamado *last = match->listaChamados;
                            while (last->prox != NULL) {
                                last = last->prox;
                            }
                            last->prox = novo;
                        }
                        printf("[AUTOMATICO] Ocorrencia critica [%d] detectada! Chamado [%d] gerado automaticamente e associado a Equipe [%s].\n",
                               ocorrencia->codigo, novoCod, match->nome);
                    } else {
                        if (*listaChamadosGeral == NULL) {
                            *listaChamadosGeral = novo;
                        } else {
                            Chamado *last = *listaChamadosGeral;
                            while (last->prox != NULL) {
                                last = last->prox;
                            }
                            last->prox = novo;
                        }
                        printf("[AUTOMATICO] Ocorrencia critica [%d] detectada! Chamado [%d] gerado automaticamente (sem equipe compativel).\n",
                               ocorrencia->codigo, novoCod);
                    }
                }
            }
        }
    }
}


void carregarEquipes(char *nomeArquivo, Equipe **listaEquipes) {
    FILE *arq = fopen(nomeArquivo, "r");
    if (arq == NULL) {
        printf("ERRO: Nao foi possivel abrir o arquivo [%s] para leitura.\n", nomeArquivo);
        return;
    }
    int id;
    char nome[50];
    char especialidade[30];
    int count = 0;
    while (fscanf(arq, "%d %49s %29s", &id, nome, especialidade) == 3) {
        cadastrarEquipe(listaEquipes, id, nome, especialidade);
        count++;
    }
    fclose(arq);
    printf("Carregadas %d equipes do arquivo [%s].\n", count, nomeArquivo);
}

void carregarChamados(char *nomeArquivo, Chamado **listaChamadosGeral, Equipe *listaEquipes, Bairro *listaBairros) {
    FILE *arq = fopen(nomeArquivo, "r");
    if (arq == NULL) {
        printf("ERRO: Nao foi possivel abrir o arquivo [%s] para leitura.\n", nomeArquivo);
        return;
    }
    int codigo_chamado, codigo_ocorrencia, prioridade, status, codigo_equipe;
    int count = 0;
    while (fscanf(arq, "%d %d %d %d %d", &codigo_chamado, &codigo_ocorrencia, &prioridade, &status, &codigo_equipe) == 5) {
        // busca occurrence
        Ocorrencia *ocorr = NULL;
        if (listaBairros != NULL && codigo_ocorrencia > 0) {
            Bairro *auxB = listaBairros->prox;
            int achou = 0;
            do {
                if (auxB->listaSensores != NULL) {
                    Sensor *auxS = auxB->listaSensores->prox;
                    do {
                        if (auxS->listaOcorrencias != NULL) {
                            Ocorrencia *auxO = auxS->listaOcorrencias->prox;
                            do {
                                if (auxO->codigo == codigo_ocorrencia) {
                                    ocorr = auxO;
                                    achou = 1;
                                    break;
                                }
                                auxO = auxO->prox;
                            } while (auxO != auxS->listaOcorrencias->prox);
                        }
                        if (achou) break;
                        auxS = auxS->prox;
                    } while (auxS != auxB->listaSensores->prox);
                }
                if (achou) break;
                auxB = auxB->prox;
            } while (auxB != listaBairros->prox);
        }

        // adiciona chamada
        if (codigo_equipe > 0) {
            Equipe *eq = buscarEquipe(listaEquipes, codigo_equipe);
            if (eq != NULL) {      
                if (buscarChamado(*listaChamadosGeral, listaEquipes, codigo_chamado) == NULL) {
                    Chamado *novo = alocarChamado(codigo_chamado, prioridade, status, ocorr);
                    if (novo != NULL) {
                        if (eq->listaChamados == NULL) {
                            eq->listaChamados = novo;
                        } else {
                            Chamado *last = eq->listaChamados;
                            while (last->prox != NULL) {
                                last = last->prox;
                            }
                            last->prox = novo;
                        }
                        if (ocorr != NULL && status == 2) {
                            ocorr->status = 2;
                        }
                    }
                }
            } else {
                cadastrarChamado(listaChamadosGeral, codigo_chamado, prioridade, status, ocorr);
            }
        } else {
            cadastrarChamado(listaChamadosGeral, codigo_chamado, prioridade, status, ocorr);
        }
        count++;
    }
    fclose(arq);
    printf("Carregados %d chamados do arquivo [%s].\n", count, nomeArquivo);
}

void salvarEquipes(char *nomeArquivo, Equipe *listaEquipes) {
    FILE *arq = fopen(nomeArquivo, "w");
    if (arq == NULL) {
        printf("ERRO: Nao foi possivel abrir o arquivo [%s] para escrita.\n", nomeArquivo);
        return;
    }
    Equipe *aux = listaEquipes;
    while (aux != NULL) {
        fprintf(arq, "%d %s %s\n", aux->codigo, aux->nome, aux->especialidade);
        aux = aux->prox;
    }
    fclose(arq);
    printf("Equipes salvas com sucesso em [%s].\n", nomeArquivo);
}

void salvarChamados(char *nomeArquivo, Chamado *listaChamadosGeral, Equipe *listaEquipes) {
    FILE *arq = fopen(nomeArquivo, "w");
    if (arq == NULL) {
        printf("ERRO: Nao foi possivel abrir o arquivo [%s] para escrita.\n", nomeArquivo);
        return;
    }
    //salvar chamadas não atribuídas
    Chamado *aux = listaChamadosGeral;
    while (aux != NULL) {
        int codOcorr = aux->ocorrencia ? aux->ocorrencia->codigo : 0;
        fprintf(arq, "%d %d %d %d 0\n", aux->codigo, codOcorr, aux->prioridade, aux->status);
        aux = aux->prox;
    }
    //salvar chamadas atribuídas
    Equipe *eq = listaEquipes;
    while (eq != NULL) {
        aux = eq->listaChamados;
        while (aux != NULL) {
            int codOcorr = aux->ocorrencia ? aux->ocorrencia->codigo : 0;
            fprintf(arq, "%d %d %d %d %d\n", aux->codigo, codOcorr, aux->prioridade, aux->status, eq->codigo);
            aux = aux->prox;
        }
        eq = eq->prox;
    }
    fclose(arq);
    printf("Chamados salvos com sucesso em [%s].\n", nomeArquivo);
}

void liberarChamados(Chamado **listaChamadosGeral) {
    if (listaChamadosGeral == NULL || *listaChamadosGeral == NULL) {
        return;
    }

    Chamado *c = *listaChamadosGeral;
    while (c != NULL) {
        Chamado *temp = c->prox;
        free(c);
        c = temp;
    }
    *listaChamadosGeral = NULL;
}

void liberarEquipes(Equipe **listaEquipes) {
    if (listaEquipes == NULL || *listaEquipes == NULL) {
        return;
    }

    Equipe *eq = *listaEquipes;
    while (eq != NULL) {
        Equipe *tempEq = eq->prox;
        Chamado *c = eq->listaChamados;
        while (c != NULL) {
            Chamado *tempC = c->prox;
            free(c);
            c = tempC;
        }
        free(eq);
        eq = tempEq;
    }
    *listaEquipes = NULL;
}

void liberarEquipesChamados(Equipe **listaEquipes, Chamado **listaChamadosGeral) {
    liberarChamados(listaChamadosGeral);
    liberarEquipes(listaEquipes);
    printf("Memoria de Equipes e Chamados totalmente liberada. . .\n");
}

void carregarEquipesArquivo(Equipe **listaEquipes) {
    carregarEquipes("equipes.txt", listaEquipes);
}

void carregarChamadosArquivo(Chamado **listaChamadosGeral, Equipe *listaEquipes, Bairro *listaBairros) {
    carregarChamados("chamados.txt", listaChamadosGeral, listaEquipes, listaBairros);
}

void salvarEquipesArquivo(Equipe *listaEquipes) {
    salvarEquipes("equipes.txt", listaEquipes);
}

void salvarChamadosArquivo(Chamado *listaChamadosGeral, Equipe *listaEquipes) {
    salvarChamados("chamados.txt", listaChamadosGeral, listaEquipes);
}

void relatorioBairrosMaisOcorrencias(Bairro *listaBairros) {
    if (listaBairros == NULL) {
        printf("Relatorio 1: Nao ha bairros cadastrados.\n");
        return;
    }
    printf("\n--- Relatorio 1: Bairros com maior quantidade de ocorrencias ---\n");
    int maxOcorrencias = -1;
    Bairro *aux = listaBairros->prox;
    do {
        int count = 0;
        if (aux->listaSensores != NULL) {
            Sensor *s = aux->listaSensores->prox;
            do {
                if (s->listaOcorrencias != NULL) {
                    Ocorrencia *o = s->listaOcorrencias->prox;
                    do {
                        count++;
                        o = o->prox;
                    } while (o != s->listaOcorrencias->prox);
                }
                s = s->prox;
            } while (s != aux->listaSensores->prox);
        }
        if (count > maxOcorrencias) {
            maxOcorrencias = count;
        }
        aux = aux->prox;
    } while (aux != listaBairros->prox);

    if (maxOcorrencias <= 0) {
        printf("Nenhuma ocorrencia registrada no sistema.\n");
        return;
    }

    aux = listaBairros->prox;
    do {
        int count = 0;
        if (aux->listaSensores != NULL) {
            Sensor *s = aux->listaSensores->prox;
            do {
                if (s->listaOcorrencias != NULL) {
                    Ocorrencia *o = s->listaOcorrencias->prox;
                    do {
                        count++;
                        o = o->prox;
                    } while (o != s->listaOcorrencias->prox);
                }
                s = s->prox;
            } while (s != aux->listaSensores->prox);
        }
        if (count == maxOcorrencias) {
            printf("Bairro: [%s] (ID: [%d]) - Ocorrencias: [%d]\n", aux->nome, aux->codigo, count);
        }
        aux = aux->prox;
    } while (aux != listaBairros->prox);
}

void relatorioSensoresOffline(Bairro *listaBairros) {
    if (listaBairros == NULL) {
        printf("Relatorio 2: Nao ha bairros cadastrados.\n");
        return;
    }
    printf("\n--- Relatorio 2: Sensores offline ---\n");
    int encontrou = 0;
    Bairro *aux = listaBairros->prox;
    do {
        if (aux->listaSensores != NULL) {
            Sensor *s = aux->listaSensores->prox;
            do {
                if (s->status != 1) { // status != 1 means OFFLINE
                    printf("Bairro: [%s] - Sensor ID: [%d] - Tipo: [%d] - Status: [%d] (OFFLINE)\n",
                           aux->nome, s->codigo, s->tipo, s->status);
                    encontrou = 1;
                }
                s = s->prox;
            } while (s != aux->listaSensores->prox);
        }
        aux = aux->prox;
    } while (aux != listaBairros->prox);

    if (!encontrou) {
        printf("Nenhum sensor offline encontrado.\n");
    }
}

void relatorioOcorrenciasCriticasAbertas(Bairro *listaBairros) {
    if (listaBairros == NULL) {
        printf("Relatorio 3: Nao ha bairros cadastrados.\n");
        return;
    }
    printf("\n--- Relatorio 3: Ocorrencias criticas abertas ---\n");
    int encontrou = 0;
    Bairro *aux = listaBairros->prox;
    do {
        if (aux->listaSensores != NULL) {
            Sensor *s = aux->listaSensores->prox;
            do {
                if (s->listaOcorrencias != NULL) {
                    Ocorrencia *o = s->listaOcorrencias->prox;
                    do {
                        if (o->severidade >= 4 && o->status == 1) {
                            printf("Bairro: [%s] - Sensor ID: [%d] - Ocorrencia ID: [%d] - Severidade: [%d] - Descricao: [%s]\n",
                                   aux->nome, s->codigo, o->codigo, o->severidade, o->descricao);
                            encontrou = 1;
                        }
                        o = o->prox;
                    } while (o != s->listaOcorrencias->prox);
                }
                s = s->prox;
            } while (s != aux->listaSensores->prox);
        }
        aux = aux->prox;
    } while (aux != listaBairros->prox);

    if (!encontrou) {
        printf("Nenhuma ocorrencia critica aberta encontrada.\n");
    }
}

void relatorioEquipeMaisAtendimentos(Equipe *listaEquipes) {
    if (listaEquipes == NULL) {
        printf("Relatorio 4: Nao ha equipes cadastradas.\n");
        return;
    }
    printf("\n--- Relatorio 4: Equipe com maior numero de atendimentos ---\n");
    int maxAtendimentos = -1;
    Equipe *aux = listaEquipes;
    while (aux != NULL) {
        if (aux->total_atendimentos > maxAtendimentos) {
            maxAtendimentos = aux->total_atendimentos;
        }
        aux = aux->prox;
    }

    aux = listaEquipes;
    while (aux != NULL) {
        if (aux->total_atendimentos == maxAtendimentos) {
            printf("Equipe: [%s] (ID: [%d]) - Especialidade: [%s] - Total de Atendimentos: [%d]\n",
                   aux->nome, aux->codigo, aux->especialidade, aux->total_atendimentos);
        }
        aux = aux->prox;
    }
}

void relatorioQuantidadeSensoresBairro(Bairro *listaBairros) {
    if (listaBairros == NULL) {
        printf("Relatorio 5: Nao ha bairros cadastrados.\n");
        return;
    }
    printf("\n--- Relatorio 5: Quantidade de sensores por bairro ---\n");
    Bairro *aux = listaBairros->prox;
    do {
        int count = 0;
        if (aux->listaSensores != NULL) {
            Sensor *s = aux->listaSensores->prox;
            do {
                count++;
                s = s->prox;
            } while (s != aux->listaSensores->prox);
        }
        printf("Bairro: [%s] (ID: [%d]) - Sensores: [%d]\n", aux->nome, aux->codigo, count);
        aux = aux->prox;
    } while (aux != listaBairros->prox);
}

void relatorioOcorrenciasPorSeveridade(Bairro *listaBairros) {
    printf("\n--- Relatorio 6: Quantidade total de ocorrencias por severidade ---\n");
    int counts[6] = {0};
    if (listaBairros != NULL) {
        Bairro *aux = listaBairros->prox;
        do {
            if (aux->listaSensores != NULL) {
                Sensor *s = aux->listaSensores->prox;
                do {
                    if (s->listaOcorrencias != NULL) {
                        Ocorrencia *o = s->listaOcorrencias->prox;
                        do {
                            if (o->severidade >= 1 && o->severidade <= 5) {
                                counts[o->severidade]++;
                            }
                            o = o->prox;
                        } while (o != s->listaOcorrencias->prox);
                    }
                    s = s->prox;
                } while (s != aux->listaSensores->prox);
            }
            aux = aux->prox;
        } while (aux != listaBairros->prox);
    }

    for (int i = 1; i <= 5; i++) {
        printf("Severidade [%d]: %d ocorrencias\n", i, counts[i]);
    }
}

void relatorioGeral(Bairro *listaBairros, Equipe *listaEquipes, Chamado *listaChamadosGeral) {
    printf("\n==================================================");
    printf("\n               RELATORIO GERAL                    ");
    printf("\n==================================================\n");
    relatorioBairrosMaisOcorrencias(listaBairros);
    relatorioSensoresOffline(listaBairros);
    relatorioOcorrenciasCriticasAbertas(listaBairros);
    relatorioEquipeMaisAtendimentos(listaEquipes);
    relatorioQuantidadeSensoresBairro(listaBairros);
    relatorioOcorrenciasPorSeveridade(listaBairros);
    printf("\n==================================================\n");
}
