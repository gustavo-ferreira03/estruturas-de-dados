#include <stdio.h>
#include <stdlib.h>

typedef struct num{
  int ch, valor, prox, status;
}TNUM;

int TH_hash(int ch, int tam){
  return ch % tam;
}

void TH_inicializa(char *arq, int tam){
  FILE *fp = fopen(arq, "wb");
  if(!fp) exit(1);
  int i, elem = -1;
  for(i = 0; i < tam; i++)fwrite(&elem, sizeof(int), 1, fp);
  fclose(fp);
}

int TH_busca(char *tabHash, char *dados, int tam, int ch){
  FILE *fp = fopen(tabHash,"rb");
  if(!fp)exit(1);
  int pos, h = TH_hash(ch, tam);
  fseek(fp, h*sizeof(int), SEEK_SET);
  fread(&pos, sizeof(int), 1, fp);
  fclose(fp);
  if(pos == -1)return -1;
  fp = fopen(dados,"rb");
  if(!fp) exit(1);
  fseek(fp, pos, SEEK_SET);
  TNUM aux;
  fread(&aux, sizeof(TNUM), 1, fp);
  while(1){
    if((aux.ch == ch) && (aux.status)) return aux.valor;
    if(aux.prox == -1){
      fclose(fp);
      return -1;
    }
    fseek(fp, aux.prox, SEEK_SET);
    fread(&aux, sizeof(TNUM), 1, fp);
  }
}

int TH_retira(char *tabHash, char *arq, int tam, int ch){
  FILE *fp = fopen(tabHash,"rb");
  if(!fp) exit(1);
  int pos, h = TH_hash(ch, tam);
  fseek(fp, h*sizeof(int), SEEK_SET);
  fread(&pos, sizeof(int), 1, fp);
  fclose(fp);
  if(pos == -1) return -1;
  fp = fopen(arq,"rb+");
  if(!fp) exit(1);
  TNUM aux;
  while(1){
    fseek(fp, pos, SEEK_SET);
    fread(&aux, sizeof(TNUM), 1, fp);
    if((aux.ch == ch) && (aux.status)){
      int valor = aux.valor;
      aux.status = 0;
      fseek(fp, pos, SEEK_SET);
      fwrite(&aux, sizeof(TNUM), 1, fp);
      fclose(fp);
      return valor;
    }
    if(aux.prox == -1){
      fclose(fp);
      return -1;
    }
    pos = aux.prox;
  }
}

void TH_insere(char *tabHash, char *arq, int tam, int ch, int valor){
  FILE *fph = fopen(tabHash, "rb+");
  if(!fph) exit(1);
  int pos, h = TH_hash(ch, tam);
  fseek(fph, h*sizeof(int), SEEK_SET);
  fread(&pos, sizeof(int), 1, fph);
  int ant, prim_pos_livre;
  ant = prim_pos_livre = -1;
  FILE *fp = fopen(arq,"rb+");
  if(!fp){
    fclose(fph);
    exit(1);
  }
  TNUM aux;
  while(pos != -1){
    fseek(fp, pos, SEEK_SET);
    fread(&aux, sizeof(TNUM), 1, fp);
    if(aux.ch == ch){
      aux.valor += valor;
      aux.status = 1;
      fseek(fp, pos, SEEK_SET);
      fwrite(&aux, sizeof(TNUM), 1, fp);
      fclose(fp);
      fclose(fph);
      return;
    }
    if((!aux.status) && (prim_pos_livre == -1)) prim_pos_livre=pos;
    ant = pos;
    pos = aux.prox;
  }
  if(prim_pos_livre == -1){
    aux.ch = ch;
    aux.valor = valor;
    aux.prox = -1;
    aux.status = 1;
    fseek(fp, 0L, SEEK_END);
    pos = ftell(fp);
    fwrite(&aux, sizeof(TNUM), 1, fp);
    if(ant != -1){
      fseek(fp, ant, SEEK_SET);
      fread(&aux, sizeof(TNUM), 1, fp);
      aux.prox = pos;
      fseek(fp, ant, SEEK_SET);
      fwrite(&aux, sizeof(TNUM), 1, fp);
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
  fread(&aux, sizeof(TNUM), 1, fp);
  aux.ch = ch;
  aux.valor = valor;
  aux.status = 1;
  fseek(fp, prim_pos_livre, SEEK_SET);
  fwrite(&aux, sizeof(TNUM), 1, fp);
  fclose(fp);
  fclose(fph);
  return;
}

void TH_imprime (char *nome_hash, char *nome_dados, int tam){
  FILE *fp = fopen(nome_hash, "rb");
  if(!fp) exit(1);
  int vet[tam];
  fread(&vet, sizeof(int), tam, fp);
  fclose(fp);

  fp = fopen(nome_dados, "rb");
  if(!fp) exit(1);
  int i, pos;
  for(i = 0; i < tam; i++){
    int p = vet[i];
    if(p != -1) printf("%d:\n", i);
    TNUM x;
    while(p != -1){
      fseek(fp, p, SEEK_SET);
      pos = ftell (fp);
      fread(&x, sizeof(TNUM), 1, fp);
      printf("$%d: ch: %d\tvalor: %d\tstatus: %d\tprox_end: $%d\n", pos, x.ch, x.valor, x.status, x.prox);
      p = x.prox;
    }
  }
  fclose(fp);
}

char *dif(char *arq1, char *arq2){
  int i, p, elem;
  char *hash = "hash", *dados = "dados", *saida = "saida";
  FILE *fp1, *fp2, *fp_hash, *fp_dados, *fp_saida;
  int t1, t2, t_hash;

  fp1 = fopen(arq1, "rb");
  if(!fp1) return NULL;
  fp2 = fopen(arq2, "rb");
  if(!fp2) return NULL;

  fseek(fp1, 0, SEEK_END);
  fseek(fp2, 0, SEEK_END);
  t1 = ftell(fp1);
  t2 = ftell(fp2);
  rewind(fp1);
  rewind(fp2);

  t_hash = (t1/sizeof(int)) + (t2/sizeof(int));
  TH_inicializa(hash, t_hash);
  fp_dados = fopen(dados, "wb");
  fclose(fp_dados);

  while(1){
    fread(&elem, sizeof(int), 1, fp1);
    if(feof(fp1)) break;
    printf("f1: %d\n", elem);
    TH_insere(hash, dados, t_hash, elem, 1);
  }
  fclose(fp1);
  
  while(1){
    fread(&elem, sizeof(int), 1, fp2);
    if(feof(fp2)) break;
    printf("f2: %d\n", elem);
    TH_insere(hash, dados, t_hash, elem, 1);
  }
  fclose(fp2);

  fp_hash = fopen(hash, "rb");
  int vet[t_hash];
  fread(&vet, sizeof(int), t_hash, fp_hash);
  fclose(fp_hash);
  fp_dados = fopen(dados, "rb");
  fp_saida = fopen(saida, "wb");
  TNUM aux;
  TH_imprime(hash, dados, t_hash);
  printf("\n");
  for(i = 0; i < t_hash; i++){
    p = vet[i];
    while(p != -1){
      fseek(fp_dados, p, SEEK_SET);
      fread(&aux, sizeof(TNUM), 1, fp_dados);
      if(aux.valor == 1) fwrite(&aux.ch, sizeof(int), 1, fp_saida);
      p = aux.prox;
    }
  }
  fclose(fp_dados);
  fclose(fp_saida);
  return saida;
}

void misc(char *arq1, char *arq2, char *saida, int k){
  int elem, p1, p2, vet[k], par[2], i;
  char *hash = "hash", *dados = "dados";
  FILE *fpA, *fpB, *fps, *fph, *fpd;

  fpA = fopen(arq1, "rb");
  if(!fpA) exit(1);
  fpB = fopen(arq2, "rb");
  if(!fpB) exit(1);

  TH_inicializa(hash, k);
  fpd = fopen(dados, "wb");
  fclose(fpd);

  while(1){
    fread(&elem, sizeof(int), 1, fpA);
    if(!feof(fpA)) TH_insere(hash, dados, k, elem, 0);
    fread(&elem, sizeof(int), 1, fpB);
    if(!feof(fpB)) TH_insere(hash, dados, k, elem, 1);

    if(feof(fpA) && feof(fpB)) break;
  }
  fclose(fpA);
  fclose(fpB);

  fph = fopen(hash, "rb");
  fread(&vet, sizeof(int), k, fph);
  fclose(fph);
  
  fpd = fopen(dados, "rb");
  fps = fopen(saida, "wb");
  TNUM auxA, auxB;
  for(i = 1; i < k; i++){
    p1 = vet[i];
    while(p1 != -1){
      fseek(fpd, p1, SEEK_SET);
      fread(&auxA, sizeof(TNUM), 1, fpd);
      if(auxA.valor == 0){
        p2 = vet[k-i];
        while(p2 != -1){
          fseek(fpd, p2, SEEK_SET);
          fread(&auxB, sizeof(TNUM), 1, fpd);
          if(auxB.valor == 1){
            par[0] = auxA.ch;
            par[1] = auxB.ch;
            fwrite(&par, sizeof(int), 2, fps);
          }
          p2 = auxB.prox;
        }
      }
      p1 = auxA.prox;
    }
  }
  fclose(fpd);
  fclose(fps);
}

int main(void){
  int num;
  char nome_dados1[31];
  printf("Digite nome do arquivo... ");
  scanf("%s", nome_dados1);
  FILE *fp = fopen(nome_dados1, "wb");
  if(!fp) exit(1);  
  printf("Digite um numero... NAO digite numeros repetidos... Para interromper insira um numero negativo...\n");
  do{
    scanf("%d", &num);
    if(num < 0) break;
    fwrite(&num, sizeof(int), 1, fp);
  }while(1);
  fclose(fp);
  
  char nome_dados2[31];
  printf("Digite nome do arquivo... ");
  scanf("%s", nome_dados2);
  fp = fopen(nome_dados2, "wb");
  if(!fp) exit(1);
  printf("Digite um numero... NAO digite numeros repetidos... Para interromper insira um numero negativo...\n");
  do{
    scanf("%d", &num);
    if(num < 0) break;
    fwrite(&num, sizeof(int), 1, fp);
  }while(1);
  fclose(fp);

  // arq1 1 2 3 4 5 9 99 150 240 -1 arq2 12 14 19 20 21 13 17 25 720 432 59 993 -1
  char *nome_saida = "saida";
  misc(nome_dados1, nome_dados2, nome_saida, 5);
  TH_imprime("hash", "dados", 5);
  fp = fopen(nome_saida, "rb");
  if(!fp) exit(1);
  int r;
  do{
    r = fread(&num, sizeof(int), 1, fp);
    if(r != 1) break;
    printf("%d\n", num);
  }while(1);
  fclose(fp);

  return 0;
}
