#include <stdio.h>
#include <stdlib.h>

#define TAM 101

typedef struct aluno{
  int mat;
  float cr;
}TA;

typedef TA* TH[TAM];

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

int main(void){
  int n, m;
  printf("Digite m (tamanho da tabela hash)... ");
  scanf("%d", &m);
  printf("Digite n (quantidade de matriculas)... ");
  scanf("%d", &n);

  if((n <= 1) || (m <= 1) || (m <= n)) return 0;

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
