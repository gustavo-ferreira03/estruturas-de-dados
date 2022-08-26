#include <stdio.h>
#include <stdlib.h>

typedef struct aluno{
  int mat, prox, status;
  float cr;
}TA;

int TH_hash(int mat, int tam){
  return mat % tam;
}

void TH_inicializa(char *arq, int n){
  FILE *fp = fopen(arq, "wb");
  if(!fp) exit(1);
  int i, elem = -1;
  for(i = 0; i < n; i++)fwrite(&elem, sizeof(int), 1, fp);
  fclose(fp);
}

TA* TH_aloca(int mat, float cr){
  TA *novo = (TA*)malloc(sizeof(TA));
  novo->mat = mat;
  novo->cr = cr;
  novo->prox = -1;
  novo->status = 1;
  return novo;
}

TA* TH_busca(char *tabHash, char *dados, int n, int mat){
  FILE *fp = fopen(tabHash,"rb");
  if(!fp)exit(1);
  int pos, h = TH_hash(mat,n);
  fseek(fp, h*sizeof(int), SEEK_SET);
  fread(&pos, sizeof(int), 1, fp);
  fclose(fp);
  if(pos == -1)return NULL;
  fp = fopen(dados,"rb");
  if(!fp) exit(1);
  fseek(fp, pos, SEEK_SET);
  TA aux;
  fread(&aux, sizeof(TA), 1, fp);
  while(1){
    if((aux.mat == mat) && (aux.status)){
      TA *resp = TH_aloca(aux.mat, aux.cr);
      resp->prox = aux.prox;
      fclose(fp);
      return resp;
    }
    if(aux.prox == -1){
      fclose(fp);
      return NULL;
    }
    fseek(fp, aux.prox, SEEK_SET);
    fread(&aux, sizeof(TA), 1, fp);
  }
}

float TH_retira(char *tabHash, char *arq, int n, int mat){
  FILE *fp = fopen(tabHash,"rb");
  if(!fp) exit(1);
  int pos, h = TH_hash(mat,n);
  fseek(fp, h*sizeof(int), SEEK_SET);
  fread(&pos, sizeof(int), 1, fp);
  fclose(fp);
  if(pos == -1)return -1;
  float cr = -1;
  fp = fopen(arq,"rb+");
  if(!fp)exit(1);
  TA aux;
  while(1){
    fseek(fp, pos, SEEK_SET);
    fread(&aux, sizeof(TA), 1, fp);
    if((aux.mat == mat) && (aux.status)){
      cr = aux.cr;
      aux.status = 0;
      fseek(fp, pos, SEEK_SET);
      fwrite(&aux, sizeof(TA), 1, fp);
      fclose(fp);
      return cr;
    }
    if(aux.prox == -1){
      fclose(fp);
      return cr;
    }
    pos = aux.prox;
  }
}

void TH_insere(char *tabHash, char *arq, int n, int mat, float cr){
  FILE *fph = fopen(tabHash, "rb+");
  if(!fph) exit(1);
  int pos, h = TH_hash(mat, n);
  fseek(fph, h*sizeof(int), SEEK_SET);
  fread(&pos, sizeof(int), 1, fph);
  int ant, prim_pos_livre;
  ant = prim_pos_livre = -1;
  FILE *fp = fopen(arq,"rb+");
  if(!fp){
    fclose(fph);
    exit(1);
  }
  TA aux;
  while(pos != -1){
    fseek(fp, pos, SEEK_SET);
    fread(&aux, sizeof(TA), 1, fp);
    if(aux.mat == mat){
      aux.cr = cr;
      aux.status = 1;
      fseek(fp, pos, SEEK_SET);
      fwrite(&aux, sizeof(TA), 1, fp);
      fclose(fp);
      fclose(fph);
      return;
    }
    if((!aux.status) && (prim_pos_livre == -1))prim_pos_livre=pos;
    ant = pos;
    pos = aux.prox;
  }
  if(prim_pos_livre == -1){
    aux.mat = mat;
    aux.cr = cr;
    aux.prox = -1;
    aux.status = 1;
    fseek(fp, 0L, SEEK_END);
    pos = ftell(fp);
    fwrite(&aux, sizeof(TA), 1, fp);
    if(ant != -1){
      fseek(fp, ant, SEEK_SET);
      fread(&aux, sizeof(TA), 1, fp);
      aux.prox = pos;
      fseek(fp, ant, SEEK_SET);
      fwrite(&aux, sizeof(TA), 1, fp);
    }
    else{
      fseek(fph, h*sizeof(int), SEEK_SET);
      fwrite(&pos, sizeof(int), 1, fph);
    }
    fclose(fp);
    fclose(fph);
    return;
  }
  fseek(fp, prim_pos_livre, SEEK_SET);
  fread(&aux, sizeof(TA), 1, fp);
  aux.mat = mat;
  aux.cr = cr;
  aux.status = 1;
  fseek(fp, prim_pos_livre, SEEK_SET);
  fwrite(&aux, sizeof(TA), 1, fp);
  fclose(fp);
  fclose(fph);
  return;
}

void TH_imprime (char *nome_hash, char *nome_dados, int m){
  FILE *fp = fopen(nome_hash, "rb");
  if(!fp) exit(1);
  int vet[m];
  fread(&vet, sizeof(int), m, fp);
  fclose(fp);

  fp = fopen(nome_dados, "rb");
  if(!fp) exit(1);
  int i, pos;
  for(i = 0; i < m; i++){
    int p = vet[i];
    if(p != -1) printf("%d:\n", i);
    TA x;
    while(p != -1){
      fseek(fp, p, SEEK_SET);
      pos = ftell (fp);
      fread(&x, sizeof(TA), 1, fp);
      printf("$%d: matricula: %d\tcr: %f\tstatus: %d\tprox_end: $%d\n", pos, x.mat, x.cr, x.status, x.prox);
      p = x.prox;
    }
  }
  fclose(fp);
}

void f(char *hash, char *dados, int N, int mat, float cr){
  FILE *fph, *fpd;
  int i = TH_hash(mat, N), p;
  fph = fopen(hash, "rb");
  if(!fph) exit(1);
  fseek(fph, i*sizeof(int), SEEK_SET);
  fread(&p, sizeof(int), 1, fph);
  fclose(fph);

  fpd = fopen(dados, "rb");
  if(!fpd) exit(1);
  TA aux;
  while(p != -1){
    fseek(fpd, p, SEEK_SET);
    fread(&aux, sizeof(TA), 1, fpd);
    if((aux.mat != mat) && (aux.cr <= cr)) TH_retira(hash, dados, N, aux.mat);
    p = aux.prox;
  }
  fclose(fpd);
}

int main(void){
  int m;
  printf("Digite o tamanho da hash... ");
  scanf("%d", &m);

  if(m <= 1) return 0;

  char nome_dados[31], nome_hash[31];
  printf("Digite nome do arquivo de dados... ");
  scanf("%s", nome_dados);
  printf("Digite nome do arquivo de hash... ");
  scanf("%s", nome_hash);

  TH_inicializa(nome_hash, m);
  FILE *fp = fopen(nome_dados, "wb");
  if(!fp) exit(1);
  fclose(fp);

  int mat;
  float cr;
  do{
    printf("Digite a matricula e o cr...\n");
    scanf("%d", &mat);
    scanf("%f", &cr);
    if(mat <= 0) break;
    TH_insere(nome_hash, nome_dados, m, mat, cr);
  }while(1);
  TH_imprime(nome_hash, nome_dados, m);

  char resp;
  do{
    printf("Digite a matricula a ser removida... ");
    scanf("%d", &mat);
    cr = TH_retira(nome_hash, nome_dados, m, mat);
    if(cr != -1) printf("%d %f saiu\n", mat, cr);
    printf("Quer continuar? ");
    scanf(" %c", &resp);
  }while((resp != 'N') && (resp != 'n'));

  TH_imprime(nome_hash, nome_dados, m);
  printf("\n");

  f(nome_hash, nome_dados, m, 5, 8.5);
  TH_imprime(nome_hash, nome_dados, m);

  return 0;
  // 4 dados hash 1 3.4 5 8.9 9 5.3 6 4.0 7 2.5 17 7.9 0 0
}

