#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para representar um cômodo (nó da árvore)
typedef struct Sala {
    char nome[50];
    char descricao[200];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Protótipos das funções
Sala* criarSala(char *nome, char *descricao);
void explorarSalas(Sala *raiz);
void liberarArvore(Sala *raiz);

int main() {
    printf("=== Detective Quest - Nível Novato ===\n");
    printf("Exploração da Mansão com Árvore Binária\n\n");
    
    // Construção da árvore binária representando a mansão
    Sala *hallEntrada = criarSala("Hall de Entrada", 
        "Um grande hall com escadaria central e lustre de cristal. Você sente que algo importante aconteceu aqui.");
    
    Sala *salaEstar = criarSala("Sala de Estar", 
        "Um ambiente aconchegante com sofás, lareira e estantes de livros. Há um copo meio cheio em cima da mesa.");
    
    Sala *biblioteca = criarSala("Biblioteca", 
        "Estantes repletas de livros antigos. Uma escrivaninha com documentos espalhados chama sua atenção.");
    
    Sala *salaJantar = criarSala("Sala de Jantar", 
        "Uma mesa longa com capacidade para 12 pessoas. Um candelabro prateado está no centro.");
    
    Sala *cozinha = criarSala("Cozinha", 
        "Utensílios espalhados pela bancada. Há um cheiro estranho vindo da pia.");
    
    Sala *escritorio = criarSala("Escritório", 
        "Uma sala organizada com computador, arquivos e um cofre aberto vazio na parede.");
    
    Sala *quartoHospedes = criarSala("Quarto de Hóspedes", 
        "Cama arrumada perfeitamente. Nada parece fora do lugar, mas o ar está pesado.");
    
    // Montagem da árvore (mapa da mansão)
    hallEntrada->esquerda = salaEstar;
    hallEntrada->direita = salaJantar;
    
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = NULL;
    
    salaJantar->esquerda = cozinha;
    salaJantar->direita = NULL;
    
    biblioteca->esquerda = NULL;
    biblioteca->direita = escritorio;
    
    cozinha->esquerda = NULL;
    cozinha->direita = quartoHospedes;
    
    escritorio->esquerda = NULL;
    escritorio->direita = NULL;
    
    quartoHospedes->esquerda = NULL;
    quartoHospedes->direita = NULL;
    
    // Início da exploração
    printf("Você está no Hall de Entrada da mansão. Algo misterioso aconteceu aqui...\n");
    printf("Explore os cômodos para encontrar pistas!\n\n");
    
    explorarSalas(hallEntrada);
    
    // Liberação de memória
    liberarArvore(hallEntrada);
    
    printf("\nExploração concluída. Volte sempre para investigar mais mistérios!\n");
    return 0;
}

// Cria uma nova sala (nó) da árvore
Sala* criarSala(char *nome, char *descricao) {
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala != NULL) {
        strcpy(novaSala->nome, nome);
        strcpy(novaSala->descricao, descricao);
        novaSala->esquerda = NULL;
        novaSala->direita = NULL;
    }
    return novaSala;
}

// Função para explorar a mansão (percorrer a árvore)
void explorarSalas(Sala *raiz) {
    Sala *atual = raiz;
    char opcao;
    
    while (atual != NULL) {
        printf("\n=== %s ===\n", atual->nome);
        printf("%s\n", atual->descricao);
        
        // Verifica se é um nó folha (fim do caminho)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("\n🔍 Você chegou a um cômodo sem saída. Esta pode ser uma pista importante!\n");
            printf("Voltando ao caminho anterior...\n");
            break;
        }
        
        // Apresenta opções de navegação
        printf("\nOpções de exploração:\n");
        if (atual->esquerda != NULL) {
            printf("e - Ir para a esquerda (%s)\n", atual->esquerda->nome);
        }
        if (atual->direita != NULL) {
            printf("d - Ir para a direita (%s)\n", atual->direita->nome);
        }
        printf("s - Sair da exploração\n");
        
        printf("\nPara onde deseja ir? ");
        scanf(" %c", &opcao);
        
        // Processa a escolha do usuário
        switch (opcao) {
            case 'e':
                if (atual->esquerda != NULL) {
                    atual = atual->esquerda;
                } else {
                    printf("Não há caminho para a esquerda!\n");
                }
                break;
                
            case 'd':
                if (atual->direita != NULL) {
                    atual = atual->direita;
                } else {
                    printf("Não há caminho para a direita!\n");
                }
                break;
                
            case 's':
                printf("Saindo da exploração...\n");
                return;
                
            default:
                printf("Opção inválida! Use 'e', 'd' ou 's'.\n");
        }
    }
}

// Libera a memória alocada para a árvore
void liberarArvore(Sala *raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}