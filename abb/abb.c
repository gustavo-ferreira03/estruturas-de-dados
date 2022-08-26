#include <stdio.h>
#include <stdlib.h>

typedef struct arvbinbusca{
  int info;
  struct arvbinbusca *esq, *dir;
}TABB;

TABB *TABB_inicializa(void){
  return NULL;
}

TABB *TABB_cria(int raiz, TABB *esq, TABB *dir){
  TABB *novo = (TABB *) malloc(sizeof(TABB));
  novo->info = raiz;
  novo->esq = esq;
  novo->dir = dir;
  return novo;
}

void TABB_imp_pre(TABB *a){
  if(a){
    printf("%d ", a->info);
    TABB_imp_pre(a->esq);
    TABB_imp_pre(a->dir);
  }
}

void TABB_imp_pos(TABB *a){
  if(a){
    TABB_imp_pos(a->esq);
    TABB_imp_pos(a->dir);
    printf("%d ", a->info);
  }
}

void TABB_imp_sim(TABB *a){
  if(a){
    TABB_imp_sim(a->esq);
    printf("%d ", a->info);
    TABB_imp_sim(a->dir);
  }
}

void imp_aux(TABB *a, int andar){
  int j;
  if(a){
    imp_aux(a->esq, andar + 1);
    for(j = 0; j <= andar; j++) printf("\t"); //printf("   ");
    printf("%d\n", a->info);
    imp_aux(a->dir, andar + 1);
  }
  else{
   for(j = 0; j <= andar; j++) printf("\t");
   printf("N\n");
  }
}

void TABB_imp_ident(TABB *a){
  imp_aux(a, 0);
}

void TABB_libera(TABB *a){
  if(a){
    TABB_libera(a->esq);
    TABB_libera(a->dir);
    free(a);
  }
}

TABB *TABB_busca(TABB *a, int elem){
  if((!a) || (a->info == elem)) return a;
  if(a->info > elem) return TABB_busca(a->esq, elem);
  return TABB_busca(a->dir, elem);
}

TABB *TABB_insere(TABB *a, int elem){
  if(!a) return TABB_cria(elem, NULL, NULL);
  if(a->info > elem) a->esq = TABB_insere(a->esq, elem);
  else if (a->info < elem) a->dir = TABB_insere(a->dir, elem);
  return a;
}

TABB *TABB_retira(TABB *a, int info){
  if(!a) return a;
  if(info < a->info) 
    a->esq = TABB_retira(a->esq, info);
  else if(info > a->info) 
    a->dir = TABB_retira(a->dir, info);
  else{ //info encontrada
    if((!a->esq) && (!a->dir)){ //CASO (1)
      free(a);
      a = NULL;
    }
    else if((!a->esq) || (!a->dir)){ //CASO (2)
      TABB *temp = a;
      if(!a->esq) a = a->dir;
      else a = a->esq;
      free(temp); 
    }
    else{ //CASO (3)
      TABB *filho = a->esq;
      while(filho->dir) filho = filho->dir;
      a->info = filho->info;
      filho->info = info;
      a->esq = TABB_retira(a->esq, info);
    }
  }
  return a; 
}

int *mN_aux(TABB *a, int n, int *vet, int s){
    if(!a) return vet;
    vet = mN_aux(a->esq, n, vet, s);
    while(vet[s-1]) s++;
    if(a->info < n){
        vet = realloc(vet, sizeof(int)*(++s));
        vet[s-2] = a->info;
        vet[s-1] = 0;
        vet = mN_aux(a->dir, n, vet, s);
    }
    return vet;
}

int *mN(TABB *a, int n){
    TABB *aux = a;
    int s = 1, *vet = malloc(sizeof(int)*s);
    if(!a) return vet;
    while(aux->info > n) aux = aux->esq;
    return mN_aux(aux, n, vet, s);
}

TABB *retira_impares_aux(TABB *a, TABB *aux){
    if(aux->esq) a = retira_impares_aux(a, aux->esq);
    if(aux->dir) a = retira_impares_aux(a, aux->dir);
    if(aux->info % 2 == 1) a = TABB_retira(a, aux->info);
    return a;
}

TABB *retira_impares(TABB *a){
    TABB *aux = a;
    return retira_impares_aux(a, aux);
}

int main(void){
    TABB *a = TABB_inicializa(); 
    int n, *m, i;

    while(1){
        scanf("%d", &n);
        if(n < 0) break;
        a = TABB_insere(a, n);
    }
    TABB_imp_ident(a);
    printf("\n");
    a = retira_impares(a);
    TABB_imp_ident(a);

    TABB_libera(a);
    return 0;
} 