#include <stdio.h>
#include <stdlib.h>

#define TAM 101

typedef struct aluno{
  int mat;
  float cr;
  struct aluno *prox;
}TA;

typedef TA* TH[TAM];

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


int main(void){
  int n, m;
  printf("Digite m (tamanho da tabela hash)... ");
  scanf("%d", &m);
  printf("Digite n (quantidade de matriculas)... ");
  scanf("%d", &n);

  if((n <= 1) || (m <= 1)) return 0;

  TH tab;
  TH_inicializa(tab, m);

  int i, mat;
  float cr;
  for(i = 0; i < n; i++){
    printf("Digite a matricula e o cr...\n");
    scanf("%d", &mat);
    scanf("%f", &cr);
    TH_insere(tab, m, mat, cr);
  }
  TH_imprime(tab, m);

  char resp;
  do{
    printf("Digite a matricula a ser removida... ");
    scanf("%d", &mat);
    cr = TH_retira(tab, m, mat);
    if(cr != -1) printf("%d\t%f saiu\n", mat, cr);
    printf("Quer continuar? ");
    scanf(" %c", &resp);
  }while((resp != 'N') && (resp != 'n'));
  TH_imprime(tab, m);

  do{
    printf("Digite a matricula a ser procurada... ");
    scanf("%d", &mat);
    TA *p = TH_busca(tab, m, mat);
    if(!p) printf("Elemento nao encontrado!\n");
    else printf("%d\t%f\n", p->mat, p->cr);
    printf("Quer continuar? ");
    scanf(" %c", &resp);
  }while((resp != 'N') && (resp != 'n'));
  TH_imprime(tab, m);

  for(i = 0; i < n/2; i++){
    printf("Digite a matricula e o cr...\n");
    scanf("%d", &mat);
    scanf("%f", &cr);
    TH_insere(tab, m, mat, cr);
  }
  TH_imprime(tab, m);

  TH_libera(tab, m);

  return 0;
}
