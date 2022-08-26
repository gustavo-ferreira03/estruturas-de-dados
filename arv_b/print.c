#include <stdio.h>
#include <stdlib.h>

typedef struct ArvB{
  int nchaves, folha, *chave;
  struct ArvB **filho;
}TARVB;

typedef struct lse{
  int info;
  struct lse *prox;
}TLSE;

int main(int argc, char *argv[]){
    TARVB *arvore = TARVB_Inicializa();
    int t = 2, num, from;
    while(num != -1){
        printf("Digite um numero para adicionar. 0 para imprimir. -9 para remover e -1 para sair\n");
        scanf("%d", &num);
        if(num == -9){
            scanf("%d", &from);
            arvore = TARVB_Retira(arvore, from, t);
            TARVB_Imprime(arvore);
        }
        else if(num == -1){
            printf("\n");
            TARVB_Imprime(arvore);
            TARVB_Libera(arvore);
            return 0;
        }
        else if(!num){
            printf("\n");

            printf("print arv decrescente:\n");
            print_dec(arvore);
            printf("\n");

            printf("print arv crescente:\n");
            print_cre(arvore);
            printf("\n");

            printf("print arvb2lse:\n");
            TLSE *l = arvb2lse(arvore);
            TLSE_print(l);
            printf("\n");

            printf("print inverte(arvb2lse):\n");
            l = inverte(l);
            TLSE_print(l);
            printf("\n");

            printf("print arvb2vet:\n");
            int *v = arvb2vet(arvore);
            for(int i = 0; i < nc(arvore); i++)
                printf("%d ", v[i]);
            printf("\n");

            free(v);
            TLSE_libera(l);
        }
        else
            arvore = TARVB_Insere(arvore, num, t);
        printf("\n\n");
    }
}

void print_dec(TARVB *a) {
    if(!a)
        return;
    print_dec(a->filho[a->nchaves]);
    for(int i = a->nchaves - 1; i >= 0; i--) {
        printf("%d ", a->chave[i]);
        print_dec(a->filho[i]);
    }
}

void print_cre(TARVB *a) {
    if(!a)
        return;
    for(int i = 0; i < a->nchaves; i++) {
        print_cre(a->filho[i]);
        printf("%d ", a->chave[i]);
    }
    print_cre(a->filho[a->nchaves]);
}

int nc(TARVB *a) {
    if(!a)
        return 0;
    int result = a->nchaves;
    for(int i = 0; i <= a->nchaves; i++)
        result += nc(a->filho[i]);
    return result;
}

int *arvb2vet(TARVB *a) {
    int n = nc(a);
    int *v = (int *) malloc(sizeof(int) * n);

    int i = 0;
    tovet(a, v, &i);
    return v;
}

void tovet(TARVB *a, int *v, int *index) {
    if(!a)
        return;
    for(int i = 0; i < a->nchaves; i++) {
        tovet(a->filho[i], v, index);
        v[*index] = a->chave[i];
        (*index)++;
    }
    tovet(a->filho[a->nchaves], v, index);
}

TLSE *arvb2lse(TARVB *a) {
    return tolse(a, NULL);
}

TLSE *tolse(TARVB *a, TLSE *l) {
    if(!a)
        return l;
    for(int i = 0; i < a->nchaves; i++) {
        l = tolse(a->filho[i], l);
        l = TLSE_ins_ini(l, a->chave[i]);
    }
    return tolse(a->filho[a->nchaves], l);
}

TLSE *inverte(TLSE *l) {
    TLSE *prev, *aux;

    prev = NULL;
    aux = l;
    while(aux) {
        TLSE *next = aux->prox;

        aux->prox = prev;
        prev = aux;

        aux = next;
    }
    return prev;
}
