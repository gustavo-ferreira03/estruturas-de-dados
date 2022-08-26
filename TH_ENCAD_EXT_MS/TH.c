#include "TH.h"

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
