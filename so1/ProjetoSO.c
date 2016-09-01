#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

 enum estados {NOVO, PRONTO, EXECUTANDO, BLOQUEADO, FINALIZADO};

struct processo {
    int id;
    int prioridade;
    int tempoExecucao;
    int contadorPrograma;
    int posicaoInicialMemoria;
    int tamanho;
    int estado;
    struct processo *prox;
};

int memoriaRam[100];

int discoRigido[100];

struct processo *emExecucao;

struct processo *listaProc;
struct processo *criaProcesso (int id, int tamanho, int tempoExecucao, int prioridade, int posInicial);
struct processo *removeProcesso(int pIdProc);
void incluiProcesso(struct processo *pProc);
void exibeProcessos();

struct processo *escSJF() {
  int idMenor = -1;
  int menorTempo = 100000;
  struct processo *listaAux = listaProc;
  if (listaProc == NULL)
    return NULL;
  else{
    while (listaAux != NULL){
      if (listaAux->tamanho < menorTempo){
        idMenor = listaAux->id;
        menorTempo = listaAux->tamanho;
      }
      listaAux = listaAux->prox;
    }
  }
  return removeProcesso(idMenor);
}

struct processo *escPrioridade(){
  int idMenor = -1;
  int menorPrioridade = 100000;
  struct processo *listaAux = listaProc;

  if (listaProc == NULL) {
    return NULL;
  } else {
    while (listaAux != NULL){
      if (listaAux->prioridade < menorPrioridade){
        idMenor = listaAux->id;
        menorPrioridade = listaAux->prioridade;
      }
      listaAux = listaAux->prox;
    }
  }
  return removeProcesso(idMenor);
}

struct processo *escFIFO(){
  if (listaProc == NULL)
    return NULL;
  else
    return removeProcesso(listaProc->id);
}

void exibeProcessos() {
  struct processo *listaAux = listaProc;
  if (listaProc == NULL)
    printf("Nao ha processos\n");
  else{
    printf("Processos: \n");
    while (listaAux != NULL){
      printf("Id %d prioridade %d \n", listaAux->id, listaAux->prioridade);
      listaAux = listaAux->prox;
    }
  }
}

struct processo *removeProcesso(int pIdProc){
  struct processo *procAtual = listaProc, *procAnterior = listaProc;
  while(procAtual != NULL){
    if(procAtual->id == pIdProc){
      if (procAtual == listaProc){
        listaProc = listaProc->prox;
        procAtual->prox = NULL;
        return procAtual;
      }
      else{
        procAnterior->prox = procAtual->prox;
        procAtual->prox = NULL;
        return procAtual;
      }
    }
    else{
      procAnterior = procAtual;
      procAtual = procAtual->prox;
    }
  }
  return NULL;
}

void incluiProcesso(struct processo *pProc){
  struct processo *listaAux = listaProc;
  pProc->estado = PRONTO;

  if (listaProc == NULL)
    listaProc = pProc;
  else{
    while (listaAux->prox != NULL)
        listaAux = listaAux->prox;

    listaAux->prox = pProc;
  }
}

void executaProcesso() {
  unsigned int usecs;

  if (emExecucao != NULL) {
    usecs = 500000 * emExecucao->tempoExecucao;
    printf("Processo %d em execucao\n", emExecucao->id);
    emExecucao->estado = EXECUTANDO;
    usleep(usecs);
    printf("Processo executado em %dms, mudando para FINALIZADO\n\n", usecs);
    emExecucao->estado = FINALIZADO;
  } else {
    printf("Nenhum processo na lista para executar\n");
  }
}

void salvaNoDisco() {
  for (int i = 0; i < emExecucao->tamanho; i++)
  {
    discoRigido[i + emExecucao->posicaoInicialMemoria] = memoriaRam[30+i];
  }

  printf("Processo %d retirado da memoria e salvo no disco\n\n", emExecucao->id);

  emExecucao = NULL;
}

void carregaNaMemoria(struct processo *novo) {
  int maxTam =  novo->tamanho;

  for (int i = 0; i < maxTam; i++)
  {
    memoriaRam[30+i] = discoRigido[i + novo->posicaoInicialMemoria];
  }
  printf("Processo %d carregado na memoria\n\n", novo->id);
}

void trocaContexto(struct processo *vlProc) {
  if ( emExecucao == NULL )
  {
    printf("\n\nNenhum processo em execucao\n\n");
  } else {
    salvaNoDisco();
  }

  carregaNaMemoria(vlProc);

  emExecucao = vlProc;

}

struct processo *criaProcesso (int id, int tamanho, int tempoExecucao, int prioridade, int posInicial) {

  struct processo *vlProc;
  vlProc = (struct processo*)malloc(sizeof(struct processo));
  if (vlProc == NULL) {
        printf("Erro de alocacao de memoria.\n");
        exit(1);
  };
  vlProc->id = id;
  vlProc->tamanho = tamanho;
  vlProc->prioridade = prioridade;
  vlProc->tempoExecucao = tempoExecucao;
  vlProc->estado = NOVO;
  vlProc->prox = NULL;
  vlProc->posicaoInicialMemoria = posInicial;

  for (int i = posInicial; i < posInicial + tamanho; i++)
  {
    discoRigido[i] = 1;
  }

  return(vlProc);
};

int main(void) {
  float startTime, endTime, timeElapsed;

  startTime = (float)clock()/CLOCKS_PER_SEC;

  //Simulação de processos
  incluiProcesso(criaProcesso(1, 12, 3, 1, 0));
  incluiProcesso(criaProcesso(2, 5, 1,  4, 11));
  incluiProcesso(criaProcesso(3, 44, 7, 3, 15));
  incluiProcesso(criaProcesso(4, 30, 6, 2, 58));


  for (int i = 0; i < 4; ++i)
  {
    trocaContexto(escFIFO());
    executaProcesso();

  }

  salvaNoDisco();

  endTime = (float)clock()/CLOCKS_PER_SEC;

  timeElapsed = endTime - startTime;

  printf("\n\nTempo total de execucao usando escalonador FIFO: %fs\n\n", timeElapsed);

  startTime = (float)clock()/CLOCKS_PER_SEC;

  //Simulação de processos
  incluiProcesso(criaProcesso(1, 12, 3, 1, 0));
  incluiProcesso(criaProcesso(2, 5, 1,  4, 11));
  incluiProcesso(criaProcesso(3, 44, 7, 3, 15));
  incluiProcesso(criaProcesso(4, 30, 6, 2, 58));



  for (int i = 0; i < 4; ++i)
  {
    trocaContexto(escSJF());
    executaProcesso();

  }

  salvaNoDisco();

  endTime = (float)clock()/CLOCKS_PER_SEC;

  timeElapsed = endTime - startTime;

  printf("\n\nTempo total de execucao usando escalonador SJF: %fs\n\n", timeElapsed);

  startTime = (float)clock()/CLOCKS_PER_SEC;

  //Simulação de processos
  incluiProcesso(criaProcesso(1, 12, 3, 1, 0));
  incluiProcesso(criaProcesso(2, 5, 1,  4, 11));
  incluiProcesso(criaProcesso(3, 44, 7, 3, 15));
  incluiProcesso(criaProcesso(4, 30, 6, 2, 58));

  for (int i = 0; i < 4; ++i)
  {
    trocaContexto(escPrioridade());
    executaProcesso();

  }

  salvaNoDisco();

  endTime = (float)clock()/CLOCKS_PER_SEC;

  timeElapsed = endTime - startTime;

  printf("\n\nTempo total de execucao usando escalonador por prioridade: %fs\n\n", timeElapsed);


  return 0;
}
