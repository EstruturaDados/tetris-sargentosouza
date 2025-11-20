#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 5

// --------------------- ESTRUTURA PEÇA -------------------------
typedef struct {
    char tipo;  // 'I', 'O', 'T', 'L'
    int id;     // exclusivo e crescente
} Peca;

// ---------------------- FILA ------------------------------
typedef struct {
    Peca itens[MAX_FILA];
    int inicio;
    int fim;
    int quantidade;
} Fila;

// ---------------------- PILHA ------------------------------
typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;

int geradorID = 0;

// ---------------- IMPRESSÃO DO FORMATO ASCII --------------------

void imprimirFormato(char tipo) {
    printf("\nFormato da peca %c:\n", tipo);

    switch(tipo) {
        case 'I':
            printf("####\n");
            break;
        case 'O':
            printf("##\n");
            printf("##\n");
            break;
        case 'T':
            printf("###\n");
            printf(" #\n");
            break;
        case 'L':
            printf("###\n");
            printf("#\n");
            break;
    }
    printf("\n");
}

// ---------------- GERAR PEÇA AUTOMÁTICA ----------------------

char gerarTipo() {
    char tipos[4] = {'I', 'O', 'T', 'L'};
    return tipos[rand() % 4];
}

Peca gerarPeca() {
    Peca nova;
    nova.tipo = gerarTipo();
    nova.id = geradorID++;
    return nova;
}

// --------------------- FILA -------------------------

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->quantidade = 0;

    for(int i = 0; i < MAX_FILA; i++) {
        f->itens[i] = gerarPeca();
        f->quantidade++;
        f->fim = (f->fim + 1) % MAX_FILA;
    }
}

int filaVazia(Fila *f) {
    return f->quantidade == 0;
}

void enqueue(Fila *f, Peca p) {
    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % MAX_FILA;
    f->quantidade++;
}

Peca dequeue(Fila *f) {
    Peca removida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->quantidade--;
    return removida;
}

void exibirFila(Fila *f) {
    printf("Fila: ");
    if (filaVazia(f)) {
        printf("[vazia]\n");
        return;
    }

    int i = f->inicio;
    for(int c = 0; c < f->quantidade; c++) {
        printf("[%c%d] ", f->itens[i].tipo, f->itens[i].id);
        i = (i + 1) % MAX_FILA;
    }
    printf("\n");
}

// ---------------------- PILHA ------------------------

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == MAX_PILHA - 1;
}

void push(Pilha *p, Peca item) {
    if (!pilhaCheia(p)) {
        p->itens[++p->topo] = item;
    }
}

Peca pop(Pilha *p) {
    return p->itens[p->topo--];
}

void exibirPilha(Pilha *p) {
    printf("Reserva: ");
    if (pilhaVazia(p)) {
        printf("[vazia]\n");
        return;
    }
    for(int i = 0; i <= p->topo; i++) {
        printf("[%c%d] ", p->itens[i].tipo, p->itens[i].id);
    }
    printf("\n");
}

// ---------------------- OPERAÇÕES ------------------------

void jogarPeca(Fila *f) {
    Peca jogada = dequeue(f);

    printf("\nPeca jogada: [%c%d]\n", jogada.tipo, jogada.id);
    imprimirFormato(jogada.tipo);

    // repõe automaticamente uma nova peça
    enqueue(f, gerarPeca());
}

void reservarPeca(Fila *f, Pilha *p) {

    if (pilhaCheia(p)) {
        printf("\nNao ha espaco na reserva!\n");
        return;
    }

    Peca reservada = dequeue(f);
    push(p, reservada);

    printf("\nPeca reservada: [%c%d]\n", reservada.tipo, reservada.id);

    enqueue(f, gerarPeca()); // repõe peça automaticamente
}

void usarReserva(Pilha *p) {

    if (pilhaVazia(p)) {
        printf("\nNao ha pecas reservadas!\n");
        return;
    }

    Peca usada = pop(p);

    printf("\nPeca usada da reserva: [%c%d]\n", usada.tipo, usada.id);
    imprimirFormato(usada.tipo);
}

// ---------------------- PROGRAMA PRINCIPAL -----------------------

int main() {
    srand(time(NULL));

    Fila fila;
    Pilha reserva;

    inicializarFila(&fila);
    inicializarPilha(&reserva);

    int opcao;

    do {
        printf("\n------- MENU -------\n");
        printf("1 - Jogar peca\n");
        printf("2 - Reservar peca\n");
        printf("3 - Usar peca reservada\n");
        printf("4 - Sair\n");
        printf("---------------------\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                jogarPeca(&fila);
                break;

            case 2:
                reservarPeca(&fila, &reserva);
                break;

            case 3:
                usarReserva(&reserva);
                break;

            case 4:
                printf("\nSaindo...\n");
                return 0;

            default:
                printf("Opcao invalida!\n");
        }

        // mostra estado sempre
        exibirFila(&fila);
        exibirPilha(&reserva);

    } while(opcao != 4);

    return 0;
}
