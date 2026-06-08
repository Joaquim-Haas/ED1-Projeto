# Contexto do Projeto — Cidade Inteligente (ED1)

Projeto da disciplina **Estrutura de Dados 1 (UTFPR)** em linguagem C, simulando monitoramento urbano com **listas encadeadas circulares** e **alocação dinâmica**.

## Estrutura de pastas

```
ED1-Projeto/
├── source/                    # Código-fonte principal do sistema
│   ├── main.c                 # Ponto de entrada completo: simulação OU menu (escolha no início)
│   ├── cidade_inteligente.c   # Ponto de entrada só do menu interativo
│   ├── menu.h / menu.c        # Menu interativo manual e funções de carga/salvamento agrupadas
│   ├── urbano.h / urbano.c    # Integrante 1 — Bairros, Sensores, Ocorrências
│   ├── operacoes.h / operacoes.c  # Integrante 2 — Equipes e Chamados
│   ├── simulador.h / simulador.c  # Integrante 3 — Interpretador, log e liberação geral
│   ├── *.txt                  # Arquivos de dados e simulação (entrada/saída)
│   ├── arquivo_1/               # Cenário de teste alternativo 1
│   └── arquivo_2/               # Cenário de teste alternativo 2
├── README.md                  # Descrição geral e clone do repositório
├── CONTEXTO_PROJETO.md        # Este arquivo — visão geral do projeto
└── ALTERACOES.md              # Registro das mudanças feitas no código
```

## Módulos e responsabilidades

| Módulo | Arquivo(s) | Responsabilidade |
|--------|-----------|------------------|
| Urbano | `urbano.c` | Cadastro e gestão de bairros, sensores e ocorrências |
| Operações | `operacoes.c` | Equipes, chamados, associação e relatório |
| Simulador | `simulador.c` | Lê `entrada_simulacao.txt`, grava `log_execucao.txt`, libera memória |
| Main | `main.c` | Escolhe modo de operação e orquestra o fluxo geral |
| Menu | `menu.c` | Submenus para CRUD manual, relatórios e persistência ao sair |

## Hierarquia de dados

```
Bairro (lista circular)
 └── Sensor (lista circular)
      └── Ocorrencia (lista circular)

Equipe (lista circular)
Chamado (lista circular global) → aponta para Ocorrencia e Equipe
```

## Arquivos de entrada/saída

| Arquivo | Descrição |
|---------|-----------|
| `bairros.txt` | Código e nome dos bairros |
| `sensores.txt` | Código, tipo, status e bairro |
| `ocorrencias.txt` | Código, severidade, status, sensor, bairro, descrição |
| `equipes.txt` | Código, nome e especialidade |
| `chamados.txt` | Código, ocorrência, equipe, prioridade, status |
| `entrada_simulacao.txt` | Comandos da simulação (11 comandos + `FIM`) |
| `log_execucao.txt` | Log gerado em tempo real pelo simulador |

## Comandos da simulação

1. `cadastrarBairro`
2. `cadastrarSensor`
3. `cadastrarEquipe`
4. `registrarOcorrencia`
5. `gerarChamado`
6. `associarEquipe`
7. `alterarStatusSensor`
8. `listarSensoresBairro`
9. `listarOcorrencias`
10. `finalizarChamado`
11. `relatorioGeral`
12. `FIM`

## Compilação

Executar dentro da pasta `source/`:

```bash
# Menu interativo apenas:
gcc -Wall -Wextra -std=c99 -o cidade_inteligente cidade_inteligente.c urbano.c operacoes.c simulador.c menu.c
./cidade_inteligente

# Simulação + menu (escolhe no início):
gcc -Wall -Wextra -std=c99 -o programa_teste main.c urbano.c operacoes.c simulador.c menu.c
./programa_teste
```

## Campo não utilizado (opcional)

O campo `Equipe.listaChamados` está declarado na estrutura, mas a associação é feita via `Chamado.equipe` para evitar corrupção da lista circular global de chamados. Se quiser usar `listaChamados`, seria necessário uma lista separada (nós auxiliares) — posso implementar se desejar.
