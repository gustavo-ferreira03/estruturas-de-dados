#include "TH.h"

int TH_hash (int mat, int n){
  return mat % n;
}

int TH_tent_linear(int mat, int n, int pos){
  return (TH_hash (mat, n) + pos) % n;
}

TA* TH_aloca(int mat, float cr){
  TA *novo = (TA*)malloc(sizeof(TA));
  novo->mat = mat;
  novo->cr = cr;
  return novo;
}

void TH_inicializa(TH tab, int n){
  int i;
  for(i = 0; i < n; i++) tab[i] = NULL;
}

TA* TH_busca(TH tab, int n, int mat){
  int i = 0;
  while(1){
    int h = TH_tent_linear(mat, n, i++);
    if((tab[h]) && (tab[h]->mat == mat)) return tab[h];
    if(i == n)return NULL;
  }
}

void TH_insere(TH tab, int n, int mat, float cr){
  int i = 0, achou = 0, primPosLivre, h;
  while(1){
    h = TH_tent_linear(mat, n, i++);
    if(tab[h]){
      if(tab[h]->mat == mat){
        tab[h]->cr = cr;
        return;
      }
    }
    else if(!achou){
      achou = 1;
      primPosLivre = h;
    }
    if(i == n)break;
  }
  tab[primPosLivre] = TH_aloca(mat,cr);
}

void TH_libera(TH tab, int n){
  int i;
  for(i = 0; i < n ;i++) if(tab[i]) free(tab[i]);
}

float TH_retira(TH tab, int n, int mat){
  float cr = -1;
  int i = 0, h;
  while(1){
    h = TH_tent_linear(mat, n, i++);
    if(tab[h]){
      if(tab[h]->mat == mat){
        cr = tab[h]->cr;
        free(tab[h]);
        tab[h] = NULL;
        return cr;
      }
    }
    if(i == n)return cr;
  }
}

void TH_imprime(TH tab, int n){
  int i;
  for(i = 0; i < n; i++){
    printf("%d: ", i);
    if(tab[i]) printf("%d\t%f\n", tab[i]->mat, tab[i]->cr);
    else printf("NULL\n");
  }
}

