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

char *dif(char *arq1, char *arq2);

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

  char *nome_saida = NULL;
  //nome_saida = dif(nome_dados1, nome_dados2);
  fp = fopen(nome_saida, "rb");
  if(!fp) exit(1);
  int r;
  do{
    r = fread(&num, sizeof(int), 1, fp);
    if(r != 1) break;
    printf("%d\n", num);
  }while(1);
  fclose(fp);
  free(nome_saida);  

  return 0;
}
