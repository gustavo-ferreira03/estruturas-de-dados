#include <stdio.h>
#include <stdlib.h>

typedef struct arvbin{
  int info, cor;
  struct arvbin *esq, *dir;
}TAB;

TAB *TAB_inicializa(void){
  return NULL;
}

TAB *TAB_cria(int raiz, TAB *esq, TAB *dir){
  TAB *novo = (TAB *) malloc(sizeof(TAB));
  novo->info = raiz;
  novo->esq = esq;
  novo->dir = dir;
  return novo;
}

TAB *TAB_busca(TAB *a, int elem){
  if((!a) || (a->info == elem)) return a;
  TAB *resp = TAB_busca(a->esq, elem);
  if(resp) return resp;
  return TAB_busca(a->dir, elem);
}

void TAB_imp_pre(TAB *a){
  if(a){
    printf("%d ", a->info);
    TAB_imp_pre(a->esq);
    TAB_imp_pre(a->dir);
  }
}

void TAB_imp_pos(TAB *a){
  if(a){
    TAB_imp_pos(a->esq);
    TAB_imp_pos(a->dir);
    printf("%d ", a->info);
  }
}

void TAB_imp_sim(TAB *a){
  if(a){
    TAB_imp_sim(a->esq);
    printf("%d ", a->info);
    TAB_imp_sim(a->dir);
  }
}

void imp_aux(TAB *a, int andar){
  int j;
  if(a){
    imp_aux(a->esq, andar + 1);
    for(j = 0; j <= andar; j++) printf("\t"); //printf("   ");
    printf("%d | %d\n", a->info, a->cor);
    imp_aux(a->dir, andar + 1);
  }
  else{
   for(j = 0; j <= andar; j++) printf("\t");
   printf("N\n");
  }
}

void TAB_imp_ident(TAB *a){
  imp_aux(a, 0);
}

void TAB_libera(TAB *a){
  if(a){
    TAB_libera(a->esq);
    TAB_libera(a->dir);
    free(a);
  }
}

int maior(int x, int y){
  if(x >= y) return x;
  return y;
}

int TAB_altura(TAB *a){
  if(!a) return -1;
  return maior(TAB_altura(a->esq), TAB_altura(a->dir)) + 1;
}

TAB *copia(TAB *a){
    if(!a) return NULL;
    TAB *new = (TAB *) malloc(sizeof(TAB));

    new->info = a->info;
    new->esq = copia(a->esq);
    new->dir = copia(a->dir);

    return new;
}

TAB *espelho(TAB *a){
    if(!a) return NULL;
    TAB *new = (TAB *) malloc(sizeof(TAB));

    new->info = a->info;
    new->esq = copia(a->dir);
    new->dir = copia(a->esq);

    return new;
}

TAB *mr(TAB *a){
    if(!a) return NULL;
    TAB *maior = a, *maior_esq, *maior_dir;
    maior_esq = mr(a->esq);
    maior_dir = mr(a->dir);
    if(maior_esq && (maior->info < maior_esq->info)) maior = maior_esq;
    if(maior_dir && (maior->info < maior_dir->info)) maior = maior_dir;

    return maior;
}

TAB *mn(TAB *a){
    if(!a) return NULL;
    TAB *menor = a, *menor_esq, *menor_dir;
    menor_esq = mn(a->esq);
    menor_dir = mn(a->dir);
    if(menor_esq && (menor->info > menor_esq->info)) menor = menor_esq;
    if(menor_dir && (menor->info > menor_dir->info)) menor = menor_dir;

    return menor;
}

int igual(TAB *a1, TAB *a2){
    if(!a1 && !a2) return 1;
    if((!a1 || !a2) || (a1->info != a2->info)) return 0;

    return igual(a1->esq, a2->esq) && igual(a2->dir, a2->dir);
}

TAB *pr(TAB *a, TAB *ant){
    if(!a->esq && !a->dir) return ant;
    if(a->dir) return pr(a->dir, a);
    return pr(a->esq, a);
}

TAB *TAB_retira(TAB *a, int elem){
    TAB *no = TAB_busca(a, elem), *dp = pr(a, NULL), *dp_f;

    if(dp->dir) dp_f = dp->dir;
    else dp_f = dp->esq;
    no->info = dp_f->info;

    TAB_libera(dp_f);
    if(dp->dir) dp->dir = NULL;
    else dp->esq = NULL;

    return a;
}

TAB *retira_pares_aux(TAB *a, TAB *aux){
    if(aux->esq) a = retira_pares_aux(a, aux->esq);
    if(aux->dir) a = retira_pares_aux(a, aux->dir);
    while(aux->info % 2 == 0) a = TAB_retira(a, aux->info);
    return a;
}

TAB *retira_pares(TAB *a){
    TAB *aux = a;
    return retira_pares_aux(a, aux);
}

void colore_aux(TAB *a, int c){
    if(a){
        a->cor = c;
        colore_aux(a->esq, !c);
        colore_aux(a->dir, !c);
    }
}

void colore(TAB *a){
    colore_aux(a, 0);
}

int main(void){
    int no, pai;
    printf("Digite a raiz da arvore... ");
    scanf("%d", &no);
    TAB *a = TAB_cria(no, NULL, NULL), *resp;
    char repete;
    do{
        char lado[2];
        printf("Quer continuar inserindo nos na árvore (digite N ou n para parar)? ");
        scanf(" %c", &repete);
        if((repete == 'N') || (repete == 'n')) break;
        printf("Digite o pai (que deve existir), o filho a ser inserido na arvore e a posição E(ESQ) ou D(DIR)... ");
        scanf("%d%d%s", &pai, &no, lado);
        resp = TAB_busca(a, pai);
        if(!resp){
        TAB_libera(a);
        return 0;
        }
        if(strcmp(lado, "E") == 0) resp->esq = TAB_cria(no, NULL, NULL);
        else resp->dir = TAB_cria(no, NULL, NULL);
    }while(1);
    colore(a);
    TAB_imp_ident(a);
    printf("\n");

    a = retira_pares(a);
    colore(a);
    TAB_imp_ident(a);
    printf("\n");
    
    TAB_libera(a);
    // TAB_libera(a_new);
    return 0;
}