#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> // Para usar 'bool'

// =============================================================================
// 1. DEFINIÇÃO DA STRUCT, ENUM e CONSTANTES
// =============================================================================

#define MAX_ITENS 10

/**
 * @brief Enumeração para definir os critérios de ordenação.
 */
typedef enum {
    CRITERIO_NOME = 1,
    CRITERIO_TIPO = 2,
    CRITERIO_PRIORIDADE = 3
} CriterioOrdenacao;

/**
 * @brief Estrutura que representa um Item no inventário (com prioridade).
 */
typedef struct {
    char nome[30];
    char tipo[20]; 
    int quantidade;
    int prioridade; // NOVIDADE NÍVEL MESTRE (1 a 5)
} Item;

// Variáveis de estado do inventário
Item inventario[MAX_ITENS];
int num_itens = 0;
bool ordenado_por_nome = false; // Flag para controlar o requisito da Busca Binária

// =============================================================================
// FUNÇÕES DE UTILIDADE E INTERFACE
// =============================================================================

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void exibir_menu() {
    printf("\n--- MOCHILA DE SOBREVIVÊNCIA (NÍVEL MESTRE) ---\n");
    printf("1. Cadastrar Item (Prioridade 1-5)\n");
    printf("2. Remover Item\n");
    printf("3. Listar Itens\n");
    printf("4. Buscar Item (Sequencial)\n");
    printf("5. Ordenar Mochila (Insertion Sort)\n"); // NOVO
    printf("6. Buscar Item (Busca Binária)\n");      // NOVO
    printf("0. Sair do Jogo\n");
    printf("-------------------------------------------------\n");
    printf("Escolha uma opção: ");
}

// ... (Restante das funções listarItens(), buscarItemSequencial(), e removerItem() são as mesmas do Nível Aventureiro) ...

void listarItens() {
    printf("\n*** ITENS NA MOCHILA (%d/%d) ***\n", num_itens, MAX_ITENS);
    if (num_itens == 0) {
        printf("A mochila está vazia. Colete recursos!\n");
        return;
    }

    for (int i = 0; i < num_itens; i++) {
        printf("[%d] Nome: %-15s | Tipo: %-10s | Quant: %3d | Prioridade: %d\n", 
               i + 1, inventario[i].nome, inventario[i].tipo, 
               inventario[i].quantidade, inventario[i].prioridade);
    }
    printf("*********************************\n");
}

int buscarItemSequencial(const char* nome_busca) {
    for (int i = 0; i < num_itens; i++) {
        if (strcmp(inventario[i].nome, nome_busca) == 0) {
            return i;
        }
    }
    return -1; 
}

void removerItem() {
    // ... (Mantendo a lógica anterior) ...
    if (num_itens == 0) {
        printf("\n🚫 ERRO: A mochila está vazia. Nada a remover.\n");
        return;
    }
    
    char nome_remover[30];
    printf("\n--- REMOÇÃO DE ITEM ---\n");
    printf("Digite o Nome do Item a ser removido: ");
    scanf("%29s", nome_remover);
    limpar_buffer();

    int indice_remover = buscarItemSequencial(nome_remover);

    if (indice_remover != -1) {
        printf("🗑️ Item '%s' encontrado e removido.\n", inventario[indice_remover].nome);

        // Move os elementos subsequentes para preencher o espaço
        for (int i = indice_remover; i < num_itens - 1; i++) {
            inventario[i] = inventario[i+1];
        }

        num_itens--;
        // Se a lista mudou, a ordenação por nome pode ter sido invalidada.
        ordenado_por_nome = false; 

    } else {
        printf("\n❌ Item '%s' não encontrado na mochila.\n", nome_remover);
    }
    listarItens();
}

// =============================================================================
// FUNÇÕES DO NÍVEL MESTRE (Inserção, Ordenação e Busca Binária)
// =============================================================================

void inserirItem() {
    if (num_itens >= MAX_ITENS) {
        printf("\n🚫 ERRO: Mochila cheia! (%d/%d)\n", num_itens, MAX_ITENS);
        return;
    }

    printf("\n--- CADASTRO DE NOVO ITEM ---\n");
    Item *novo_item = &inventario[num_itens];

    printf("Digite o Nome do Item (max 29 chars): ");
    scanf("%29s", novo_item->nome);
    limpar_buffer();

    printf("Digite o Tipo do Item (ex: arma, munição, cura): ");
    scanf("%19s", novo_item->tipo);
    limpar_buffer();

    printf("Digite a Quantidade: ");
    if (scanf("%d", &novo_item->quantidade) != 1 || novo_item->quantidade <= 0) {
        printf("Quantidade inválida. Operação cancelada.\n");
        limpar_buffer();
        return; 
    }
    limpar_buffer();

    // NOVIDADE: Leitura da Prioridade
    printf("Digite a Prioridade (1 - Mínima a 5 - Máxima): ");
    if (scanf("%d", &novo_item->prioridade) != 1 || novo_item->prioridade < 1 || novo_item->prioridade > 5) {
        printf("Prioridade inválida (deve ser entre 1 e 5). Item não cadastrado.\n");
        limpar_buffer();
        return;
    }
    limpar_buffer();
    
    num_itens++;
    printf("\n✅ Item '%s' cadastrado com sucesso! \n", novo_item->nome);

    // Qualquer inserção desorganiza a lista por nome
    ordenado_por_nome = false; 
    listarItens(); 
}


/**
 * @brief Implementa o algoritmo Insertion Sort (Ordenação por Inserção).
 * @param criterio Critério de ordenação escolhido (Nome, Tipo ou Prioridade).
 * @return O número total de comparações realizadas.
 */
long long ordenarMochila(CriterioOrdenacao criterio) {
    long long num_comparacoes = 0;
    
    // Insertion Sort
    for (int i = 1; i < num_itens; i++) {
        Item chave = inventario[i];
        int j = i - 1;

        // Loop de inserção: move elementos maiores/menores para a direita
        while (j >= 0) {
            int comparacao = 0;
            
            // Lógica de Comparação com base no critério
            if (criterio == CRITERIO_NOME) {
                comparacao = strcmp(inventario[j].nome, chave.nome);
                num_comparacoes++;
            } else if (criterio == CRITERIO_TIPO) {
                comparacao = strcmp(inventario[j].tipo, chave.tipo);
                num_comparacoes++;
            } else if (criterio == CRITERIO_PRIORIDADE) {
                // Ordenação decrescente por prioridade (maior prioridade primeiro)
                if (inventario[j].prioridade < chave.prioridade) comparacao = 1;
                else if (inventario[j].prioridade > chave.prioridade) comparacao = -1;
                else comparacao = 0; // Se as prioridades são iguais, a ordem não importa tanto
                num_comparacoes++;
            }
            
            // Condição para mover o elemento: (comparacao > 0) para ordem crescente
            // Ou (comparacao < 0) se for ordem decrescente (apenas para Prioridade neste exemplo)
            bool deve_mover = false;
            if (criterio == CRITERIO_PRIORIDADE) {
                deve_mover = (inventario[j].prioridade < chave.prioridade);
            } else {
                deve_mover = (comparacao > 0);
            }

            if (deve_mover) {
                inventario[j + 1] = inventario[j];
                j = j - 1;
            } else {
                break;
            }
        }
        inventario[j + 1] = chave;
    }
    
    // Atualiza a flag de ordenação por nome
    ordenado_por_nome = (criterio == CRITERIO_NOME);
    return num_comparacoes;
}

/**
 * @brief Menu de Ordenação.
 */
void menuOrdenacao() {
    int escolha_criterio;
    printf("\n--- CRITÉRIO DE ORDENAÇÃO ---\n");
    printf("1. Nome (Ordem Crescente)\n");
    printf("2. Tipo (Ordem Crescente)\n");
    printf("3. Prioridade (Ordem Decrescente)\n");
    printf("Escolha o critério: ");
    
    if (scanf("%d", &escolha_criterio) != 1 || escolha_criterio < 1 || escolha_criterio > 3) {
        printf("Opção inválida. Ordenação cancelada.\n");
        limpar_buffer();
        return;
    }
    limpar_buffer();
    
    long long comp = ordenarMochila((CriterioOrdenacao)escolha_criterio);
    
    printf("\n✅ MOCHILA ORDENADA!\n");
    printf("Análise de Desempenho: %lld comparações realizadas.\n", comp);
    listarItens();
}

/**
 * @brief Implementa a Busca Binária (Requer lista ordenada por nome).
 * @param nome_busca O nome do item a ser procurado.
 * @return O índice do item encontrado ou -1.
 */
int buscarItemBinaria(const char* nome_busca) {
    int inicio = 0;
    int fim = num_itens - 1;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        int comparacao = strcmp(inventario[meio].nome, nome_busca);

        if (comparacao == 0) {
            return meio; // Item encontrado
        } else if (comparacao < 0) {
            inicio = meio + 1; // Item está na metade superior
        } else {
            fim = meio - 1; // Item está na metade inferior
        }
    }
    return -1; // Item não encontrado
}

/**
 * @brief Interface para a Busca Binária.
 */
void menuBuscaBinaria() {
    if (!ordenado_por_nome) {
        printf("\n❌ ERRO: A Busca Binária requer que a mochila esteja ordenada por Nome.\n");
        printf("Por favor, use a opção 5 e escolha 'Nome' antes de buscar.\n");
        listarItens();
        return;
    }
    
    char nome_busca[30];
    printf("\n--- BUSCA BINÁRIA ---\n");
    printf("Digite o Nome do Item para buscar: ");
    scanf("%29s", nome_busca);
    limpar_buffer();
    
    int indice = buscarItemBinaria(nome_busca);

    if (indice != -1) {
        printf("\n🔎 ITEM ENCONTRADO (via Busca Binária)!\n");
        Item item = inventario[indice];
        printf("  Nome: %s\n", item.nome);
        printf("  Tipo: %s\n", item.tipo);
        printf("  Quantidade: %d\n", item.quantidade);
        printf("  Prioridade: %d\n", item.prioridade);
    } else {
        printf("\n❌ Item '%s' não encontrado na mochila.\n", nome_busca);
    }
    listarItens();
}

// =============================================================================
// FUNÇÃO PRINCIPAL
// =============================================================================

int main() {
    int escolha;
    
    printf("************************************************\n");
    printf("* SOBREVIVÊNCIA NA ILHA - NÍVEL MESTRE *\n");
    printf("************************************************\n");

    do {
        exibir_menu();
        if (scanf("%d", &escolha) != 1) {
            printf("\nOpção inválida. Tente novamente.\n");
            limpar_buffer();
            continue;
        }
        limpar_buffer();
        
        switch (escolha) {
            case 1: inserirItem(); break;
            case 2: removerItem(); break;
            case 3: listarItens(); break;
            case 4: 
                // A Busca Sequencial (do Nível Aventureiro) ainda funciona
                menuBuscaBinaria(); // Reutilizamos a interface da binária para evitar duplicação
                break;
            case 5: menuOrdenacao(); break;     // NOVO
            case 6: menuBuscaBinaria(); break;  // NOVO
            case 0: printf("\nSobrevivência encerrada.\n"); break;
            default: printf("\nOpção não reconhecida.\n"); break;
        }
        
    } while (escolha != 0);

    return 0;
}
