#include <stdlib.h>
#include <stdio.h>

typedef struct viz {
	int id_viz;
	struct viz *prox_viz;
}TVIZ;

typedef struct grafo{
	int id_no;
	TVIZ *prim_viz;
	struct grafo *prox_no;
}TG;

TG* TG_inicializa(){
  return NULL;
}


void TG_imprime(TG *g){
  while(g){
    printf("Vizinhos do No %d:\n", g->id_no);
    TVIZ *v = g->prim_viz;
    while(v){
      printf("%d ", v->id_viz);
      v = v->prox_viz;
    }
    printf("\n");
    g = g->prox_no;
  }
}

void TG_imp_rec(TG *g){
  if(g){
    printf("%d:\n", g->id_no);
    TVIZ *v = g->prim_viz;
    while(v){
      printf("%d ", v->id_viz);
      v = v->prox_viz;
    }
    TG_imp_rec(g->prox_no);
  }
}

void TG_libera_viz(TVIZ *v){
  while(v){
    TVIZ *temp = v;
    v = v->prox_viz;
    free(temp);
  }
}

void TG_libera_viz_rec(TVIZ *v){
  if(!v) return;
  TG_libera_viz_rec(v->prox_viz);
  free(v);
}

void TG_libera(TG *g){
  while(g){
    TG_libera_viz(g->prim_viz);
    TG *temp = g;
    g = g->prox_no;
    free(temp);
  }
}

void TG_libera_rec(TG *g){
  if(g){
    TG_libera_viz(g->prim_viz);
    TG_libera_rec(g ->prox_no);
    free(g);
  }
}

TG* TG_busca_no(TG* g, int x){
  if((!g) || (g->id_no == x)) return g;
  return(TG_busca_no(g->prox_no, x));
}

TVIZ* TG_busca_aresta(TG *g, int no1, int no2){
  TG *p1 = TG_busca_no(g,no1), *p2 = TG_busca_no(g,no2);
  if((!p1) || (!p2)) return NULL;
  TVIZ *resp = p1->prim_viz;
  while((resp) && (resp->id_viz != no2)) resp = resp->prox_viz;
  return resp;
}

TG* TG_ins_no(TG *g, int x){
  TG *p = TG_busca_no(g, x);
  if(!p){
    p = (TG*) malloc(sizeof(TG));
    p->id_no = x;
    p->prox_no = g;
    p->prim_viz = NULL;
    g = p;
  }
  return g;
}

void TG_ins_um_sentido(TG *g, int no1, int no2){
  TG *p = TG_busca_no(g, no1);
  TVIZ *nova = (TVIZ *) malloc(sizeof(TVIZ));
  nova->id_viz = no2;
  nova->prox_viz = p->prim_viz;
  p->prim_viz = nova;
}

void TG_ins_aresta(TG *g, int no1, int no2){
  TVIZ *v = TG_busca_aresta(g, no1, no2);
  if(v) return;
  TG_ins_um_sentido(g, no1, no2);
  TG_ins_um_sentido(g, no2, no1);
}

void TG_retira_um_sentido(TG *g, int no1, int no2){
  TG *p = TG_busca_no(g, no1);
  if(!p) return;
  TVIZ *ant = NULL, *atual = p->prim_viz;
  while((atual)&&(atual->id_viz != no2)){
    ant = atual;
    atual = atual->prox_viz;
  }
  if(!ant) p->prim_viz = atual->prox_viz;
  else ant->prox_viz = atual->prox_viz;
  free(atual);
}

void TG_retira_aresta(TG *g ,int no1, int no2){
  TVIZ* v = TG_busca_aresta(g,no1,no2);
  if(!v) return;
  TG_retira_um_sentido(g,no1,no2);
  TG_retira_um_sentido(g,no2,no1);
}

TG* TG_retira_no(TG *g, int no){
  TG *p = g, *ant = NULL;
  while((p)&& (p->id_no != no)){
    ant = p;
    p = p->prox_no;
  }
  if(!p) return g;
  while(p->prim_viz) TG_retira_aresta(g, no, p->prim_viz->id_viz);
  if(!ant) g = g->prox_no;
  else ant->prox_no = p->prox_no;
  free(p);
  return g;
}

typedef struct lse{
  int info;
  struct lse *prox;
}TLSE;

TLSE* TLSE_inicializa(void){
  return NULL;
}

TLSE* TLSE_insere(TLSE *l, int elem){
  TLSE *novo = (TLSE *) malloc(sizeof(TLSE));
  novo->prox = l;
  novo->info = elem;
  return novo;
}


void TLSE_imprime(TLSE *l){
  TLSE *p = l;
  while(p){
    printf("%d ", p->info);
    p = p->prox;
  } 
}

void TLSE_imp_rec(TLSE *l){
  if(l){
    printf("%d ", l->info);
    TLSE_imp_rec(l->prox);
  }
}

void TLSE_imp_rec_rev(TLSE *l){
  if(l){
    TLSE_imp_rec_rev(l->prox);
    printf("%d ", l->info);
  }
}

void TLSE_libera(TLSE *l){
  TLSE *p = l, *q;
  while(p){
    q = p;
    p = p->prox;
    free(q);
  } 
}

void TLSE_lib_rec(TLSE *l){
  if(l){
    TLSE_lib_rec(l->prox);
    printf("liberando o elemento %d...\n", l->info);
    free(l);
  }
}

TLSE* TLSE_retira(TLSE *l, int elem){
  TLSE *p = l, *ant = NULL;
  while((p) && (p->info != elem)){
    ant = p;
    p = p->prox;
  }
  if(!p) return l;
  if(!ant) l = l->prox;
  else ant->prox = p->prox;
  free(p);
  return l;
}

TLSE* TLSE_retira_rec(TLSE *l, int elem){
  if(!l) return l;
  if(l->info == elem){
    TLSE *p = l;
    l = l->prox;
    free(p);
  }
  else l->prox = TLSE_retira_rec(l->prox, elem);
  return l;
}

TLSE* TLSE_busca(TLSE *l, int elem){
  TLSE *p = l;
  while((p) && (p->info != elem)) p = p->prox; 
  return p;
}

TLSE* TLSE_busca_rec(TLSE *l, int elem){
  if((!l) || (l->info == elem)) return l;
  return TLSE_busca_rec(l->prox, elem);
}

TLSE* cc_aux(TG *g, TLSE *l, TG *n) {
    l = TLSE_insere(l, n->id_no);
    TVIZ *v = n->prim_viz;
    while(v != NULL){
        if(!TLSE_busca_rec(l, v->id_viz)) l = cc_aux(g, l, TG_busca_no(g, v->id_viz));
        v = v->prox_viz;
    }
    return l;
}

int cc(TG *g){
    int count = 0;
    TLSE *l = TLSE_inicializa();
    TG *aux = g;
    while(aux != NULL){
        if(!TLSE_busca_rec(l, aux->id_no)) {
            count++;
            l = cc_aux(g, l, aux);
        }
        aux = aux->prox_no;
    }
    return count;
}

int main(void){
  TG *g = TG_inicializa();
  int n;
  do{
    scanf("%d", &n);
    if(n <= 0) break;
    g = TG_ins_no(g, n);
    TG_imprime(g);
  }while(1);
  
  int m;
  do{
    scanf("%d%d", &n, &m);
    if((n <= 0) || (m <= 0)) break;
    TG_ins_aresta(g, n, m);
    TG_imprime(g);
  }while(1);

  int t;
  t = cc(g);
  printf("%d\n-------\n", t);
  
  return 0;
}
