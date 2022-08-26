#include <stdio.h>
#include <stdlib.h>

#define TAM 101

typedef struct num{
  int number;
  int qt;
  struct num *prox;
}TN;

typedef TN* TH[TAM];

int TH_hash(int number, int tam){
  return number % tam;
}

void TH_inicializa(TH tab, int n){
  int i;
  for(i = 0; i < n; i++)tab[i] = NULL;
}

TN* TH_busca(TH tab, int n, int number){
  int h = TH_hash(number, n);
  TN *p = tab[h];
  while((p) && (p->number != number)) p = p->prox;
  return p;
}

TN* TH_aloca(int number){
  TN *novo = (TN*) malloc (sizeof (TN));
  novo->number = number;
  novo->qt = 1;
  novo->prox = NULL;
  return novo;
}

void TH_insere(TH tab, int n, int number){
  TN *b = TH_busca(tab, n, number);
  if(b) b->qt = b->qt + 1;
  else {
    int i = 0, primPosLivre, h = TH_hash(number, n);
    TN *p = tab[h], *ant = NULL;
    while((p) && (p->number != number)){
        ant = p;
        p = p->prox;
    }
    TN *novo = TH_aloca(number);
    if(!ant)tab[h] = novo;
    else ant->prox = novo;
  }
}

void TH_libera(TH tab, int n){
  int i;
  for(i = 0; i < n; i++)
    if(tab[i]){
      TN *p = tab[i], *q;
      while(p){
        q = p;
        p = p->prox;
        free(q);
      }
    }
}

float TH_retira(TH tab, int n, int number){
  int h = TH_hash(number,n);
  if(!tab[h]) return -1;
  TN *p = tab[h],*ant = NULL;
  float qt = -1;
  while((p) && (p->number != number)){
    ant = p;
    p = p->prox;
  }
  if(!p) return qt;
  if(!ant) tab[h] = p->prox;
  else ant->prox = p->prox;
  qt = p->qt;
  free(p);
  return qt;
}

void TH_imprime(TH tab, int n){
  int i;
  for(i = 0; i < n; i++){
    printf("%d: ", i);
    if(tab[i]){
      TN *p = tab[i];
      printf("\n");
      while(p){
        printf("\t%d\t%d\t%p\n", p->number, p->qt, p->prox);
        p = p->prox;
      }
    }
    else printf("NULL\n");
  }
}

void TH_compara(TH tab1, int n1, TH tab2, int n2){
  TN *aux, *b;
  int i;
  for(i = 0; i < n1; i++){
    aux = tab1[i];
    while(aux){
      b = TH_busca(tab2, n2, aux->number);
      if((!b) || (aux->qt != b->qt)){
        printf("Os números não aparecem a mesma quantidade de vezes\n");
        return;
      }
      aux = aux->prox;
    }
  }
  printf("Os números aparecem a mesma quantidade de vezes\n");
}

int main(void){
    //   v1 = [1, 2, 3, 4, 5, 1, 5, 5, 4];
    //   v2 = [1, 2, 6, 7, 3, 4, 5, 5, 1, 5, 4];
    int *v1, *v2, n1, n2, i;
    TH tab1, tab2;

    TH_inicializa(tab1, n1);
    TH_inicializa(tab2, n2);

    printf("Insira a qtd de números de v1: \n");
    scanf("%d", &n1);
    v1 = (int *) malloc(sizeof(int)*n1);
    for(i = 0; i < n1; i++){
        scanf("%d", &v1[i]);
        TH_insere(tab1, n1, v1[i]);
    }

    printf("Insira a qtd de números de v2: \n");
    scanf("%d", &n2);
    v2 = (int *) malloc(sizeof(int)*n2);
    for(i = 0; i < n2; i++){
        scanf("%d", &v2[i]);
        TH_insere(tab2, n2, v2[i]);
    }


    TH_imprime(tab1, n1);
    TH_imprime(tab2, n2);

    TH_compara(tab1, n1, tab2, n2);

    free(v1);
    free(v2);
    TH_libera(tab1, n1);
    TH_libera(tab2, n2);

    return 0;
}
