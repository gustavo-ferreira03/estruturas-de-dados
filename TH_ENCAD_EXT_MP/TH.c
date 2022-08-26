#include "TH.h"

int TH_hash(int mat, int tam){
  return mat % tam;
}

void TH_inicializa(TH tab, int n){
  int i;
  for(i = 0; i < n; i++)tab[i] = NULL;
}

TA* TH_busca(TH tab, int n, int mat){
  int h = TH_hash(mat, n);
  TA *p = tab[h];
  while((p) && (p->mat != mat)) p = p->prox;
  return p;
}

TA* TH_aloca(int mat, float cr){
  TA *novo = (TA*) malloc (sizeof (TA));
  novo->mat = mat;
  novo->cr = cr;
  novo->prox = NULL;
  return novo;
}

void TH_insere(TH tab, int n, int mat, float cr){
  int i = 0, achou = 0, primPosLivre, h = TH_hash(mat, n);
  TA *p = tab[h],*ant = NULL;
  while((p) && (p->mat != mat)){
    ant = p;
    p = p->prox;
  }
  if(p){
    p->cr = cr;
    return;
  }
  TA *novo = TH_aloca(mat,cr);
  if(!ant)tab[h] = novo;
  else ant->prox = novo;
}

void TH_libera(TH tab, int n){
  int i;
  for(i = 0; i < n; i++)
    if(tab[i]){
      TA *p = tab[i], *q;
      while(p){
        q = p;
        p = p->prox;
        free(q);
      }
    }
}

float TH_retira(TH tab, int n, int mat){
  int h = TH_hash(mat,n);
  if(!tab[h]) return -1;
  TA *p = tab[h],*ant = NULL;
  float cr = -1;
  while((p) && (p->mat != mat)){
    ant = p;
    p = p->prox;
  }
  if(!p) return cr;
  if(!ant) tab[h] = p->prox;
  else ant->prox = p->prox;
  cr = p->cr;
  free(p);
  return cr;
}

void TH_imprime(TH tab, int n){
  int i;
  for(i = 0; i < n; i++){
    printf("%d: ", i);
    if(tab[i]){
      TA *p = tab[i];
      printf("\n");
      while(p){
        printf("\t%d\t%f\t%p\n", p->mat, p->cr, p->prox);
        p = p->prox;
      }
    }
    else printf("NULL\n");
  }
}
