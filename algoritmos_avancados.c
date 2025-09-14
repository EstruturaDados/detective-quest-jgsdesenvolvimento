#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAMANHO_HASH 10
#define MAX_SUSPEITOS 5
#define MAX_PISTAS_POR_SUSPEITO 10

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

// Estrutura para um nó da tabela hash (lista encadeada)
typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode *proximo;
} HashNode;

// Estrutura para a tabela hash
typedef struct {
    HashNode *tabela[TAMANHO_HASH];
    int contagemSuspeitos[MAX_SUSPEITOS];
    char suspeitos[MAX_SUSPEITOS][50];
} TabelaHash;

// Protótipos das funções
Sala* criarSala(char *nome, char *descricao, char *pista, int temPista);
PistaBST* inserirPista(PistaBST *raiz, char *pista);
void emOrdem(PistaBST *raiz);
void liberarArvore(Sala *raiz);
void liberarBST(PistaBST *raiz);
void inicializarTabelaHash(TabelaHash *hash);
int funcaoHash(char *chave);
void inserirNaHash(TabelaHash *hash, char *pista, char *suspeito);
void relacionarPistasSuspeitos(TabelaHash *hash);
void mostrarRelacoesPistasSuspeitos(TabelaHash *hash);
void encontrarSuspeitoPrincipal(TabelaHash *hash);
void explorarSalas(Sala *raiz, PistaBST **pistasColetadas, TabelaHash *hash);

int main() {
    printf("=== Detective Quest - Nível Mestre ===\n");
    printf("Sistema Integrado com Tabela Hash de Suspeitos\n\n");
    
    // Árvore BST para armazenar pistas coletadas (inicialmente vazia)
    PistaBST *pistasColetadas = NULL;
    
    // Tabela hash para relacionar pistas com suspeitos
    TabelaHash hash;
    inicializarTabelaHash(&hash);
    
    // Preenche a tabela hash com as relações pista-suspeito
    relacionarPistasSuspeitos(&hash);
    
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
    
    explorarSalas(hallEntrada, &pistasColetadas, &hash);
    
    // Exibir todas as pistas coletadas em ordem alfabética
    printf("\n=== RELATÓRIO FINAL DE INVESTIGAÇÃO ===\n");
    printf("Pistas coletadas:\n");
    emOrdem(pistasColetadas);
    
    // Mostrar relações entre pistas e suspeitos
    printf("\n=== RELAÇÕES PISTAS-SUSPEITOS ===\n");
    mostrarRelacoesPistasSuspeitos(&hash);
    
    // Encontrar e mostrar o suspeito principal
    printf("\n=== CONCLUSÃO DA INVESTIGAÇÃO ===\n");
    encontrarSuspeitoPrincipal(&hash);
    
    // Liberação de memória
    liberarArvore(hallEntrada);
    liberarBST(pistasColetadas);
    
    printf("\nInvestigação concluída. O caso está resolvido!\n");
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

// Inicializa a tabela hash
void inicializarTabelaHash(TabelaHash *hash) {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        hash->tabela[i] = NULL;
    }
    
    for (int i = 0; i < MAX_SUSPEITOS; i++) {
        hash->contagemSuspeitos[i] = 0;
    }
    
    // Definindo os suspeitos
    strcpy(hash->suspeitos[0], "João Silva (Mordomo)");
    strcpy(hash->suspeitos[1], "Maria Santos (Herdeira)");
    strcpy(hash->suspeitos[2], "Pedro Costa (Empresário)");
    strcpy(hash->suspeitos[3], "Ana Oliveira (Cozinheira)");
    strcpy(hash->suspeitos[4], "Carlos Mendes (Jardineiro)");
}

// Função hash simples baseada na soma dos caracteres
int funcaoHash(char *chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        soma += tolower(chave[i]);
    }
    return soma % TAMANHO_HASH;
}

// Insere uma relação pista-suspeito na tabela hash
void inserirNaHash(TabelaHash *hash, char *pista, char *suspeito) {
    int indice = funcaoHash(pista);
    
    HashNode *novoNo = (HashNode*)malloc(sizeof(HashNode));
    strcpy(novoNo->pista, pista);
    strcpy(novoNo->suspeito, suspeito);
    novoNo->proximo = NULL;
    
    // Inserção no início da lista encadeada
    if (hash->tabela[indice] == NULL) {
        hash->tabela[indice] = novoNo;
    } else {
        novoNo->proximo = hash->tabela[indice];
        hash->tabela[indice] = novoNo;
    }
    
    // Atualiza contagem de suspeitos
    for (int i = 0; i < MAX_SUSPEITOS; i++) {
        if (strcmp(hash->suspeitos[i], suspeito) == 0) {
            hash->contagemSuspeitos[i]++;
            break;
        }
    }
}

// Relaciona pistas com suspeitos (dados do caso)
void relacionarPistasSuspeitos(TabelaHash *hash) {
    inserirNaHash(hash, "Porta principal arrombada", "Carlos Mendes (Jardineiro)");
    inserirNaHash(hash, "Copo com líquido estranho", "Ana Oliveira (Cozinheira)");
    inserirNaHash(hash, "Livro com páginas arrancadas", "Maria Santos (Herdeira)");
    inserirNaHash(hash, "Cadeira desencaixada da mesa", "Pedro Costa (Empresário)");
    inserirNaHash(hash, "Faca desaparecida do bloco", "Ana Oliveira (Cozinheira)");
    inserirNaHash(hash, "Cofre violado e vazio", "João Silva (Mordomo)");
    inserirNaHash(hash, "Mala com documentos falsos", "Pedro Costa (Empresário)");
    inserirNaHash(hash, "Pegadas que levam ao muro", "Carlos Mendes (Jardineiro)");
    inserirNaHash(hash, "Caixa de ferramentas violada", "Carlos Mendes (Jardineiro)");
}

// Mostra todas as relações pista-suspeito
void mostrarRelacoesPistasSuspeitos(TabelaHash *hash) {
    printf("Relações encontradas:\n");
    for (int i = 0; i < TAMANHO_HASH; i++) {
        HashNode *atual = hash->tabela[i];
        while (atual != NULL) {
            printf("• %s → %s\n", atual->pista, atual->suspeito);
            atual = atual->proximo;
        }
    }
}

// Encontra e exibe o suspeito principal baseado nas pistas
void encontrarSuspeitoPrincipal(TabelaHash *hash) {
    int maxContagem = 0;
    int indiceSuspeito = -1;
    
    for (int i = 0; i < MAX_SUSPEITOS; i++) {
        printf("%s: %d pistas relacionadas\n", hash->suspeitos[i], hash->contagemSuspeitos[i]);
        
        if (hash->contagemSuspeitos[i] > maxContagem) {
            maxContagem = hash->contagemSuspeitos[i];
            indiceSuspeito = i;
        }
    }
    
    if (indiceSuspeito != -1) {
        printf("\n🔍 SUSPEITO PRINCIPAL: %s\n", hash->suspeitos[indiceSuspeito]);
        printf("   Com %d pistas relacionadas ao caso!\n", maxContagem);
    }
}

// Função para explorar a mansão e coletar pistas
void explorarSalas(Sala *raiz, PistaBST **pistasColetadas, TabelaHash *hash) {
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
        printf("r - Ver relações pistas-suspeitos\n");
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
                
            case 'r':
                printf("\n=== RELAÇÕES PISTAS-SUSPEITOS ===\n");
                mostrarRelacoesPistasSuspeitos(hash);
                break;
                
            case 's':
                printf("Saindo da exploração...\n");
                return;
                
            default:
                printf("Opção inválida! Use 'e', 'd', 'p', 'r' ou 's'.\n");
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