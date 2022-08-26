#include <stdio.h>
#include <stdlib.h>

typedef struct num{
  int ch, valor, prox, status;
}TNUM;

int TH_hash(int ch, int tam);
void TH_inicializa(char *arq, int tam);
int TH_busca(char *tabHash, char *dados, int tam, int ch);
int TH_retira(char *tabHash, char *arq, int tam, int ch);
void TH_insere(char *tabHash, char *arq, int tam, int ch, int valor);
void TH_imprime (char *nome_hash, char *nome_dados, int tam);
