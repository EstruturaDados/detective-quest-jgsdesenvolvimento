#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para representar um cômodo (nó da árvore de navegação)
typedef struct Sala {
    char nome[50];
    char descricao[200];
    char pista[100];        // Pista encontrada neste cômodo
    int temPista;           // Flag para indicar se tem pista
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura para representar uma pista (nó da árvore BST)
typedef struct PistaBST {
    char descricao[100];
    struct PistaBST *esquerda;
    struct PistaBST *direita;
} PistaBST;

// Protótipos das funções
Sala* criarSala(char *nome, char *descricao, char *pista, int temPista);
PistaBST* inserirPista(PistaBST *raiz, char *pista);
void emOrdem(PistaBST *raiz);
void liberarArvore(Sala *raiz);
void liberarBST(PistaBST *raiz);
void explorarSalas(Sala *raiz, PistaBST **pistasColetadas);

int main() {
    printf("=== Detective Quest - Nível Aventureiro ===\n");
    printf("Exploração da Mansão com Coleta de Pistas\n\n");
    
    // Árvore BST para armazenar pistas coletadas (inicialmente vazia)
    PistaBST *pistasColetadas = NULL;
    
    // Construção da árvore binária representando a mansão
    Sala *hallEntrada = criarSala("Hall de Entrada", 
        "Um grande hall com escadaria central e lustre de cristal. Você sente que algo importante aconteceu aqui.",
        "Porta principal arrombada", 1);
    
    Sala *salaEstar = criarSala("Sala de Estar", 
        "Um ambiente aconchegante com sofás, lareira e estantes de livros. Há um copo meio cheio em cima da mesa.",
        "Copo com líquido estranho", 1);
    
    Sala *biblioteca = criarSala("Biblioteca", 
        "Estantes repletas de livros antigos. Uma escrivaninha com documentos espalhados chama sua atenção.",
        "Livro com páginas arrancadas", 1);
    
    Sala *salaJantar = criarSala("Sala de Jantar", 
        "Uma mesa longa com capacidade para 12 pessoas. Um candelabro prateado está no centro.",
        "Cadeira desencaixada da mesa", 1);
    
    Sala *cozinha = criarSala("Cozinha", 
        "Utensílios espalhados pela bancada. Há um cheiro estranho vindo da pia.",
        "Faca desaparecida do bloco", 1);
    
    Sala *escritorio = criarSala("Escritório", 
        "Uma sala organizada com computador, arquivos e um cofre aberto vazio na parede.",
        "Cofre violado e vazio", 1);
    
    Sala *quartoHospedes = criarSala("Quarto de Hóspedes", 
        "Cama arrumada perfeitamente. Nada parece fora do lugar, mas o ar está pesado.",
        "Mala com documentos falsos", 1);
    
    Sala *jardim = criarSala("Jardim", 
        "Área externa bem cuidada com roseiras e bancos. Há pegadas na terra molhada.",
        "Pegadas que levam ao muro", 1);
    
    Sala *porao = criarSala("Porão", 
        "Local escuro e úmido com prateleiras de conservas. O ar é pesado e mofado.",
        "Caixa de ferramentas violada", 1);
    
    // Montagem da árvore (mapa da mansão)
    hallEntrada->esquerda = salaEstar;
    hallEntrada->direita = salaJantar;
    
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = jardim;
    
    salaJantar->esquerda = cozinha;
    salaJantar->direita = NULL;
    
    biblioteca->esquerda = NULL;
    biblioteca->direita = escritorio;
    
    cozinha->esquerda = NULL;
    cozinha->direita = quartoHospedes;
    
    escritorio->esquerda = NULL;
    escritorio->direita = porao;
    
    quartoHospedes->esquerda = NULL;
    quartoHospedes->direita = NULL;
    
    jardim->esquerda = NULL;
    jardim->direita = NULL;
    
    porao->esquerda = NULL;
    porao->direita = NULL;
    
    // Início da exploração
    printf("Você está no Hall de Entrada da mansão. Algo misterioso aconteceu aqui...\n");
    printf("Explore os cômodos para encontrar pistas e desvendar o mistério!\n\n");
    
    explorarSalas(hallEntrada, &pistasColetadas);
    
    // Exibir todas as pistas coletadas em ordem alfabética
    printf("\n=== RELATÓRIO FINAL DE INVESTIGAÇÃO ===\n");
    printf("Pistas coletadas (%d no total):\n", 8); // Total de pistas disponíveis
    emOrdem(pistasColetadas);
    
    // Liberação de memória
    liberarArvore(hallEntrada);
    liberarBST(pistasColetadas);
    
    printf("\nInvestigação concluída. As pistas foram organizadas e analisadas!\n");
    return 0;
}

// Cria uma nova sala (nó) da árvore
Sala* criarSala(char *nome, char *descricao, char *pista, int temPista) {
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala != NULL) {
        strcpy(novaSala->nome, nome);
        strcpy(novaSala->descricao, descricao);
        strcpy(novaSala->pista, pista);
        novaSala->temPista = temPista;
        novaSala->esquerda = NULL;
        novaSala->direita = NULL;
    }
    return novaSala;
}

// Insere uma pista na árvore BST (ordem alfabética)
PistaBST* inserirPista(PistaBST *raiz, char *pista) {
    if (raiz == NULL) {
        PistaBST *novaPista = (PistaBST*)malloc(sizeof(PistaBST));
        if (novaPista != NULL) {
            strcpy(novaPista->descricao, pista);
            novaPista->esquerda = NULL;
            novaPista->direita = NULL;
            return novaPista;
        }
    }
    
    int comparacao = strcmp(pista, raiz->descricao);
    
    if (comparacao < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (comparacao > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    
    return raiz;
}

// Percorre a árvore BST em ordem (alfabética)
void emOrdem(PistaBST *raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esquerda);
        printf("• %s\n", raiz->descricao);
        emOrdem(raiz->direita);
    }
}

// Função para explorar a mansão e coletar pistas
void explorarSalas(Sala *raiz, PistaBST **pistasColetadas) {
    Sala *atual = raiz;
    char opcao;
    
    while (atual != NULL) {
        printf("\n=== %s ===\n", atual->nome);
        printf("%s\n", atual->descricao);
        
        // Verifica se há pista neste cômodo e se ainda não foi coletada
        if (atual->temPista) {
            printf("\n🔍 VOCÊ ENCONTROU UMA PISTA: %s\n", atual->pista);
            
            // Insere a pista na árvore BST
            *pistasColetadas = inserirPista(*pistasColetadas, atual->pista);
            
            // Marca a pista como coletada
            atual->temPista = 0;
            
            printf("Pista adicionada ao seu caderno de investigação!\n");
        }
        
        // Verifica se é um nó folha (fim do caminho)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("\nVocê chegou a um cômodo sem saída.\n");
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
        printf("p - Ver pistas coletadas\n");
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
                
            case 'p':
                printf("\n=== PISTAS COLETADAS ===\n");
                if (*pistasColetadas == NULL) {
                    printf("Nenhuma pista coletada ainda.\n");
                } else {
                    emOrdem(*pistasColetadas);
                }
                break;
                
            case 's':
                printf("Saindo da exploração...\n");
                return;
                
            default:
                printf("Opção inválida! Use 'e', 'd', 'p' ou 's'.\n");
        }
    }
}

// Libera a memória alocada para a árvore de navegação
void liberarArvore(Sala *raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}

// Libera a memória alocada para a árvore BST de pistas
void liberarBST(PistaBST *raiz) {
    if (raiz != NULL) {
        liberarBST(raiz->esquerda);
        liberarBST(raiz->direita);
        free(raiz);
    }
}