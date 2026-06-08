# Registro de Alterações

## Resumo (última atualização)

Implementação do **menu interativo manual** (`menu.c`), escolha de modo no `main.c` (simulação ou menu), e funções auxiliares de compatibilidade em `operacoes.c` (`gerarChamado`, aliases de arquivo, `liberarChamados`/`liberarEquipes`).

---

## Menu interativo (novo)

| Arquivo | Descrição |
|---------|-----------|
| `source/menu.h` | Declarações de `exibirMenuInterativo`, `carregarDadosIniciais`, `salvarDadosNosArquivos` |
| `source/menu.c` | Submenus para bairros, sensores, ocorrências, equipes/chamados e relatórios |

### Fluxo do `main.c`
1. Carrega todos os `.txt` ao iniciar
2. Pergunta: `[1]` Simulação automática ou `[2]` Menu interativo
3. Modo 1: executa `entrada_simulacao.txt`, verifica consistência e salva
4. Modo 2: menu manual; opção `0` salva e encerra
5. `liberarMemoriaGeral` ao final

---

## Resumo anterior

Implementação completa do módulo **Simulador (Integrante 3)** e das funções faltantes de **Operações (Integrante 2)**, além de funções auxiliares em `urbano.c` e integração no `main.c`.

---

## Arquivos criados

| Arquivo | Descrição |
|---------|-----------|
| `source/simulador.c` | Interpretador, log, verificação de consistência e `liberarMemoriaGeral` |
| `source/operacoes.c` | Equipes, chamados, relatório, persistência e liberação |
| `CONTEXTO_PROJETO.md` | Documentação geral do projeto |
| `ALTERACOES.md` | Este arquivo |

---

## Arquivos modificados

### `source/urbano.h` / `source/urbano.c`
- **`buscarOcorrenciaPorCodigo`** — busca ocorrência em todos os bairros/sensores (necessário para `gerarChamado`)
- **`alterarStatusSensor`** — altera status de um sensor e retorna sucesso/erro

### `source/operacoes.h`
- Estrutura `Chamado` ganhou campo `Equipe *equipe`
- Declarações de: `gerarChamado`, `associarEquipe`, `finalizarChamado`, `relatorioGeral`, buscas, carregar/salvar arquivos, `liberarEquipes`, `liberarChamados`

### `source/main.c`
- Carrega todos os `.txt` iniciais
- Executa `entrada_simulacao.txt`
- Verifica consistência, salva dados e chama `liberarMemoriaGeral`

---

## Funções implementadas no simulador (`simulador.c`)

| Função | O que faz |
|--------|-----------|
| `executarSimulacao` | Abre o arquivo, lê linha a linha, usa `strtok`/`strcmp` e despacha os 11 comandos |
| `registrarLog` | Grava em `log_execucao.txt` com data/hora e status (`SUCESSO`, `ERRO`, `INFO`) |
| `verificarConsistenciaDados` | Valida ponteiros de chamados → ocorrências e equipes |
| `liberarMemoriaGeral` | Libera chamados, equipes e bairros (cascata completa) |

---

## Funções implementadas em operações (`operacoes.c`)

| Função | O que faz |
|--------|-----------|
| `cadastrarEquipe` | Cadastra equipe em lista circular |
| `gerarChamado` | Cria chamado vinculado a uma ocorrência existente |
| `associarEquipe` | Associa chamado a equipe via `chamado->equipe` |
| `finalizarChamado` | Marca chamado como finalizado (status 2) e incrementa atendimentos |
| `relatorioGeral` | Imprime totais e resumo das equipes |
| `carregarEquipesArquivo` / `carregarChamadosArquivo` | Leitura dos `.txt` |
| `salvarEquipesArquivo` / `salvarChamadosArquivo` | Gravação dos `.txt` |
| `liberarEquipes` / `liberarChamados` | Liberação de memória |

---

## Decisões técnicas

1. **Status de sensor na simulação** — `cadastrarSensor` no urbano não recebe status; o simulador define o status após o cadastro (mesma abordagem de `carregarSensoresArquivo`).
2. **Ordem dos parâmetros de `registrarOcorrencia`** — o simulador remapeia a ordem do arquivo para a assinatura da função.
3. **`listarOcorrencias` sem argumentos** — o simulador percorre todos os bairros e sensores.
4. **`Equipe.listaChamados`** — mantido na struct, mas não populado; associação feita por `Chamado.equipe` para não corromper a lista circular global.

---

## Como testar

```bash
cd source
gcc -Wall -Wextra -std=c99 -o cidade_inteligente main.c urbano.c operacoes.c simulador.c menu.c
./cidade_inteligente
```

Verificar:
- Saída no terminal (cadastros, listagens, relatório)
- Arquivo `log_execucao.txt` gerado
- Ausência de vazamento de memória (valgrind, se disponível)
