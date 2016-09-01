#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// Estruturas

typedef struct arquivo {
  struct arquivo *prox;
  int posicao;
} Arq;

typedef struct bloco {
  char nome[100];
  int tamanho;
  char data[20];
  Arq *arq;
  int posicao;
  struct bloco *prox;
  struct bloco *filho;
} Bloco;

// helpers

char** splitString(char* cmd, char *separator);
char* date();

// Métodos auxiliares

Bloco* localizaDir(Bloco* inicio, char nome[]);
Bloco* localizaArq(Bloco* inicio, char nome[]);
int pegaBloco();
void liberaBloco(int i);
void removeArquivo(Arq* arq);
void imprimiLvlArvore(Bloco* bloco, int nivel);

// Comandos

void criad();
void criaa();
void removed();
void removea();
void verd();
void verset();
void mapa();
void arvore();
void ajuda();


Bloco *raiz;
Bloco *anterior;

int blocosVazio[256];
int verSet[256];
int espacosLivres = 246;
char **argList;


// MAIN

int main () {
  int i;
  char cmd[100], cmd_aux[100];

  ajuda();

  for (i = 0; i < 256; i++) {
    if(i < 10) {
      blocosVazio[i] = 0;
    } else {
      blocosVazio[i] = 1;
    }
  }

  // Inicia a raiz
  raiz = (Bloco*) malloc (sizeof(Bloco));
  strcpy(raiz->nome, "raiz");
  raiz->arq = NULL;
  raiz->prox = NULL;
  raiz->filho = NULL;

  while(1) {
    printf("$ ");

    fgets(cmd, 100, stdin);

    cmd[strlen(cmd) - 1] = '\0';

    if (feof(stdin)) {
      printf("\n");
      exit(0);
    }

    argList = splitString(cmd, " ");

    if(strcmp(argList[0], "criad") == 0) criad();

    else if(strcmp(argList[0], "criaa") == 0) criaa();

    else if(strcmp(argList[0], "removed") == 0) removed();

    else if(strcmp(argList[0], "removea") == 0) removea();

    else if(strcmp(argList[0], "verd") == 0) verd();

    else if(strcmp(argList[0], "verset") == 0) verset();

    else if(strcmp(argList[0], "mapa") == 0) mapa();

    else if(strcmp(argList[0], "arvore") == 0) arvore();

    else if(strcmp(argList[0], "ajuda") == 0) ajuda();

    else if(strcmp(argList[0], "sair") == 0) exit(0);

    else printf("Comando Invalido!\n\n");

  }
}

// helpers

char** splitString(char* cmd, char *separator) {
  int i = 1;
  char** retorno;
  char* s;
  char aux[100];

  strcpy(aux, cmd);

  s = strtok(aux, separator);

  while(s) {
    i++;
    s = strtok(NULL, separator);
  }

  retorno = malloc((i + 1) * sizeof(char*));

  i = 0;
  s = strtok(cmd, separator);

  while (s) {
    retorno[i++] = s;
    s = strtok(NULL, separator);
  }

  retorno[i] = NULL;
  return retorno;
}

char* date() {
  struct timeval tv;
  struct tm* ptm;
  char time_string[40], *data = NULL;
  long milliseconds;

  gettimeofday (&tv, NULL);
  ptm = localtime (&tv.tv_sec);

  strftime (time_string, sizeof (time_string), "%d/%m/%Y %H:%M:%S", ptm);

  data = malloc(strlen(time_string));
  strcpy(data, time_string);

  return data;
}

// Métodos auxiliares

Bloco* localizaDir(Bloco* inicio, char nome[]) {
  Bloco* b = inicio->filho;

  anterior = NULL;
  while (b != NULL) {
    if (b->arq == NULL && strcmp(b->nome, nome) == 0) {
      return b;
    }
    anterior = b;
    b = b->prox;
  }
  return NULL;
}

Bloco* localizaArq(Bloco* inicio, char nome[]) {
  Bloco* b = inicio->filho;
  anterior = NULL;
  while (b != NULL) {
    if (b->arq != NULL && strcmp(b->nome, nome) == 0) {
      return b;
    }
    anterior = b;
    b = b->prox;
  }
  return NULL;
}

int pegaBloco() {
  int i;
  for (i = 10; i < 256; i++) {
    if (blocosVazio[i]) {
      blocosVazio[i] = 0;
      espacosLivres--;
      return i;
    }
  }
  return -1;
}

void liberaBloco(int i) {
  blocosVazio[i] = 1;
  espacosLivres++;
}

void removeArquivo(Arq* arq) {
  if (arq->prox != NULL) removeArquivo(arq->prox);
  liberaBloco(arq->posicao);
  free(arq);
}

void imprimiLvlArvore(Bloco* bloco, int nivel) {
  int nNivel = nivel, i;

  if (bloco->arq == NULL) printf(" |__%s\n", bloco->nome);

  if (bloco->arq == NULL && bloco->filho != NULL) {
    nNivel++;
    for (i = 0; i < nivel; i++) {
      printf(" | ");
    }

    imprimiLvlArvore(bloco->filho, nNivel);
    nNivel--;
  }

  if (bloco->prox != NULL) {
    for( i = 0; i < nNivel-1; i++) {
      printf(" | ");
    }
    imprimiLvlArvore(bloco->prox, nNivel);
  }
}

// Comandos

void criad() {
  if(argList[1] == NULL) {
    printf("Diretório Inválido!\n\n");
    return;
  }

  Bloco* dir = raiz;
  int j = 0, pos;
  char **dirList = splitString(argList[1], "/");

  if (dirList[1] != NULL) {
    do {
      dir = localizaDir(dir, dirList[j]);

      if (dir == NULL) {
        printf("Diretorio nao encontrado: %s\n\n", argList[1]);
        return;
      }

      j++;
    } while (dirList[j+1] != NULL);
  }


  Bloco* temp = localizaDir(dir, dirList[j]);

  if (temp != NULL) {
    printf("Ja existe um diretorio com esse nome no mesmo caminho: %s\n\n", argList[1]);
    return;
  }

  pos = pegaBloco();
  if (pos == -1) {
    printf("Erro: Não há espaço!\n\n");
    return;
  }

  Bloco *novoBloco = (Bloco*) malloc (sizeof(Bloco));

  novoBloco->prox = dir->filho;
  strcpy(novoBloco->nome, dirList[j]);
  strcpy(novoBloco->data, date());
  novoBloco->filho = NULL;
  novoBloco->arq = NULL;
  novoBloco->posicao = pos;
  dir->filho = novoBloco;
}

void criaa() {
  if (argList[1] == NULL || argList[2] == NULL) {
    printf("Erro: Digite o nome e o tamanho do arquivo\n\n");
    return;
  }

  int i, j = 0, tamArq, qtdBlocos, pos;
  Bloco* atual = raiz;
  Bloco* temp;
  char **dirList = splitString(argList[1], "/");

  if (dirList[1] != NULL) {

    do {
      atual = localizaDir(atual, dirList[j]);
      if (atual == NULL) {
        printf("Nao foi possivel encontrar o diretorio: %s\n\n", argList[1]);
        return;
      }
      j++;
    } while(dirList[j+1] != NULL);
  }


  temp = localizaArq(atual, dirList[j]);
  if (temp != NULL) {
    printf("Ja existe um arquivo com esse nome no mesmo caminho: %s\n\n", argList[1]);
    return;
  }

  tamArq = atoi(argList[2]);
  qtdBlocos = (tamArq / 512) + 1;

  if(qtdBlocos > espacosLivres) {
    printf("Nao ha espaco suficiente!\n\n");
    return;
  }

  Arq* arq = (Arq*) malloc (sizeof(Arq));
  Arq* novoArq;
  pos = pegaBloco();
  arq->posicao = pos;
  arq->prox = NULL;

  for (i = 1; i < qtdBlocos; i++) {
    novoArq = (Arq*) malloc (sizeof(Arq));
    novoArq->posicao = pegaBloco();
    novoArq->prox = arq->prox;
    arq->prox = novoArq;

    if (pos == -1) {
      printf("Erro: Nao ha espaco!\n\n");
      return;
    }
  }

  Bloco *novoBloco = (Bloco*) malloc (sizeof(Bloco));
  novoBloco->prox = atual->filho;
  strcpy(novoBloco->nome, dirList[j]);
  strcpy(novoBloco->data, date());
  novoBloco->filho = NULL;
  novoBloco->arq = arq;
  novoBloco->tamanho = tamArq;
  novoBloco->posicao = pos;
  atual->filho = novoBloco;
}

void removed() {
  if (argList[1] == NULL) {
    printf("Erro: Digite o nome do diretorio.\n\n");
    return;
  }

  char **dirList = splitString(argList[1], "/");
  Bloco* atual = raiz;
  Bloco* temp;
  int j = 0;

  if (dirList[1] != NULL) {

    do {
      atual = localizaDir(atual, dirList[j]);

      if (atual == NULL) {
        printf("Diretorio nao encontrado: %s\n\n", dirList[j]);
        return;
      }
      j++;
    } while(dirList[j+1]!=NULL);
  }

  temp = localizaDir(atual,dirList[j]);
  if (temp != NULL) {

    if (temp->filho != NULL) {
      printf("Erro: Diretorio nao esta vazio.\n\n");
    } else {

      if (anterior != NULL) {
        anterior->prox = temp->prox;
      } else {
        atual->filho = temp->prox;
      }

      liberaBloco(temp->posicao);
      free(temp);
      printf("Diretorio %s removido\n\n", dirList[j]);
    }
  } else {
    printf("Diretorio nao encontrado: %s\n\n", dirList[j]);
  }
}

void removea() {
  if (argList[1] == NULL) {
    printf("Erro: Digite o nome do arquivo\n\n");
    return;
  }

  Bloco* atual = raiz;
  Bloco* temp;
  char **dirList = splitString(argList[1], "/");
  int j = 0;

  if (dirList[1] != NULL) {
    do {
      atual = localizaDir(atual, dirList[j]);

      if (atual == NULL) {
        printf("Diretorio nao encontrado: %s\n\n", dirList[j]);
        return;
      }
      j++;
    } while (dirList[j+1] != NULL);
  }


  temp = localizaArq(atual, dirList[j]);

  if (temp != NULL) {

    removeArquivo(temp->arq);

    if (anterior != NULL) {
      anterior->prox = temp->prox;
    } else {
      atual->filho = temp->prox;
    }

    free(temp);
    printf("Arquivo %s removido\n\n", dirList[j]);

  } else {
    printf("Arquivo nao encontrado: %s\n\n", dirList[j]);
  }
}

void verd() {
  int totalArq = 0, totalDir = 0, tamArq = 0;
  int tamanhoLivre = espacosLivres * 512;

  Bloco* atual = raiz;

  if (argList[1] == NULL) return;

  char **dirList = splitString(argList[1], "/");

  int j = 0;

  do {
    atual = localizaDir(atual, dirList[j]);
    if (atual == NULL) {
      printf("Diretorio nao encontrado: %s!\n\n", argList[1]);
      return;
    }
    j++;
  } while (dirList[j] != NULL);

  atual = atual->filho;

  if (atual == NULL) {
    printf("Nenhum arquivo ou diretorio encontrado para listar.\n\n");
  } else {
    while(atual != NULL) {
      if (atual->arq == NULL) {
        printf("%s    <DIR>    %s\n\n", atual->data, atual->nome);
        totalDir++;
      } else {
        printf("%s    %d    %s\n", atual->data, atual->tamanho, atual->nome );
        totalArq++;
        tamArq += atual->tamanho;
      }
      atual = atual->prox;
    }

    printf("\n%d arquivo(s)     %d bytes ocupados\n", totalArq, tamArq);
    printf("%d diretorio(s)   %d bytes disponiveis\n", totalDir, tamanhoLivre);
    printf("\n");
  }
}

void verset() {
  int i;

  for (i = 0; i < 256; i++) {
    if (i < 10) {
      verSet[i] = 0;
    } else {
      verSet[i] = 1;
    }
  }

  if (argList[1] == NULL) {
    printf("Erro: Digite o nome do arquivo.\n\n");
    return;
  }

  char **dirList = splitString(argList[1], "/");

  Bloco* atual = raiz;
  Bloco* temp;
  int j = 0;

  if (dirList[1] != NULL) {

    do {
      atual = localizaDir(atual, dirList[j]);
      if (atual == NULL) {
        printf("Diretorio nao encontrado: %s\n\n", argList[1]);
        return;
      }
      j++;
    } while(dirList[j+1] != NULL);
  }

  temp = localizaArq(atual,dirList[j]);

  if (temp != NULL) {
    Arq* tempArq = temp->arq;

    while(tempArq != NULL) {
      verSet[tempArq->posicao] = 0;
      tempArq = tempArq->prox;
    }

    printf("Setores ocupados pelo arquivo: %s\n", argList[1]);

    for (i = 0; i < 256; i++) {
      if (i < 10) {
        printf("B");
      } else if (verSet[i]) {
        printf("_");
      } else {
        printf("#");
      }
    }

    printf("\n\n");

  } else {
    printf("Arq nao encontrado: %s\n\n", argList[1]);
  }
}

void mapa() {
  int i;
  for (i = 0; i < 256; i++) {
    if (i < 10) {
      printf("B");
    } else if (blocosVazio[i]) {
      printf("_");
    } else {
      printf("#");
    }
  }
  printf("\n");
}

void arvore() {
  Bloco* atual = raiz->filho;

  if (atual != NULL) {
    printf("\nRaiz\n");
    imprimiLvlArvore(atual, 1);
    printf("\n");
  }
}

void ajuda() {
  printf("\ncriad   [dir/]diretorio        -> Cria diretorio\n");
  printf("criaa   [dir/]arquivo tamanho  -> Cria arquivo   \n");
  printf("removed [dir/]diretorio        -> Remove diretorio\n");
  printf("removea [dir/]arquivo          -> Remove arquivo\n");
  printf("verd    [dir]                  -> Mostra diretorio\n");
  printf("verset  [dir/]arquivo          -> Mostra setores ocupados pelo arquivo\n");
  printf("mapa                           -> Mostra tabela de setores\n");
  printf("arvore                         -> Mostra a arvore de diretorios\n");
  printf("ajuda                          -> Mostra a lista de comandos\n");
  printf("sair                           -> Finaliza o programa \n\n");
}
