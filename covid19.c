#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define LIVRE 0
#define CARREGADO 1

int infec_count[3] = {0,0,0};
int lab_count[3] = {0,0,0};

typedef struct laboratorio {
  int id; // para determinar a posição
  int min_count;
  pthread_t thread_lab;
  sem_t* mutex;
  sem_t* bancada;
} laboratorio;

typedef struct infectado {
  int id;
  int min_count;
  pthread_t thread_infec;
  sem_t* mutex;
  sem_t* bancada;
} infectado;

void* lab_func (void* arg) {
  laboratorio* lab = (laboratorio*) arg;
  int posicao_1_bancada = lab->id*2;
  int posicao_2_bancada = posicao_1_bancada + 1;

  int valor_1_bancada, valor_2_bancada;

  while (
    lab_count[0] < lab->min_count ||
    lab_count[1] < lab->min_count ||
    lab_count[2] < lab->min_count 
  ) {
    sem_wait(lab->mutex);

    sem_getvalue(&lab->bancada[posicao_1_bancada],&valor_1_bancada);
    sem_getvalue(&lab->bancada[posicao_2_bancada],&valor_2_bancada);

    if (valor_1_bancada == LIVRE && valor_2_bancada == LIVRE) {
      sem_post(&lab->bancada[posicao_1_bancada]);
      sem_post(&lab->bancada[posicao_2_bancada]);
      lab_count[lab->id]+=1;
    } 

    sem_post(lab->mutex);
    sleep(1);
  }


  return NULL;
}

void* infec_func (void* arg) {
  infectado* infec = (infectado*) arg;
  int posicao_1_elemento_1_bancada = infec->id;
  int posicao_2_elemento_1_bancada = infec->id + 3;

  int prox = infec->id + 1;
  if (prox == 4) {
    prox = 0;
  }

  int posicao_1_elemento_2_bancada = prox;
  int posicao_2_elemento_2_bancada = prox +3;

  int valor_1_elemento_1_bancada, valor_2_elemento_1_bancada, valor_1_elemento_2_bancada, valor_2_elemento_2_bancada;

  while (
    infec_count[0] < infec->min_count ||
    infec_count[1] < infec->min_count ||
    infec_count[2] < infec->min_count 
  ) {
    sem_wait(infec->mutex);

    sem_getvalue(&infec->bancada[posicao_1_elemento_1_bancada],&valor_1_elemento_1_bancada);
    sem_getvalue(&infec->bancada[posicao_2_elemento_1_bancada],&valor_2_elemento_1_bancada);
    sem_getvalue(&infec->bancada[posicao_1_elemento_2_bancada],&valor_1_elemento_2_bancada);
    sem_getvalue(&infec->bancada[posicao_2_elemento_2_bancada],&valor_2_elemento_2_bancada);

    int consumir_elemento_1 = -1;
    int consumir_elemento_2 = -1;

    if (valor_1_elemento_1_bancada == CARREGADO) {
      consumir_elemento_1 = posicao_1_elemento_1_bancada;
    } else if (valor_2_elemento_1_bancada == CARREGADO) {
      consumir_elemento_1 = posicao_2_elemento_1_bancada;
    }

    if (valor_1_elemento_2_bancada == CARREGADO) {
      consumir_elemento_2 = posicao_1_elemento_2_bancada;
    } else if (valor_2_elemento_2_bancada == CARREGADO) {
      consumir_elemento_2 = posicao_2_elemento_2_bancada;
    }

    if (consumir_elemento_1 != -1 && consumir_elemento_2 != -1) {
      sem_wait(&infec->bancada[consumir_elemento_1]);
      sem_wait(&infec->bancada[consumir_elemento_2]);

      infec_count[infec->id]+=1;
    }

    sem_post(infec->mutex);
    sleep(10);
  }


  return NULL;
}

int main (int argc,char* argv[]) {
  infectado* infectados = (infectado*)malloc(3*sizeof(infectado));
  laboratorio* laboratorios = (laboratorio*)malloc(3*sizeof(laboratorio));
  sem_t mutex;
  sem_t* bancada = (sem_t*)malloc(6*sizeof(sem_t));
  int i;

  int min_count = atoi(argv[1]);


  sem_init(&mutex,0,1);

  for(i=0; i<6; i++) {
    sem_init(&bancada[i],0,0);
  }

  for(i=0;i<3;i++) {
    infectados[i].id = i;
    infectados[i].min_count = min_count;
    infectados[i].mutex = &mutex;
    infectados[i].bancada = bancada;
  }

  for(i=0;i<3;i++) {
    laboratorios[i].id = i;
    laboratorios[i].min_count = min_count;
    laboratorios[i].mutex = &mutex;
    laboratorios[i].bancada = bancada;
  }

  for(i=0; i<3; i++) {
    pthread_create(&infectados[i].thread_infec,NULL,infec_func,&(infectados[i]));
  }
  
  for(i=0; i<3; i++) {
    pthread_create(&laboratorios[i].thread_lab,NULL,lab_func,&(laboratorios[i]));
  }

  for(i=0; i<3;i++) {
    pthread_join(infectados[i].thread_infec,NULL);
  }

  for(i=0; i<3;i++) {
    pthread_join(laboratorios[i].thread_lab,NULL);
  }

  printf("Laboratório 1: %d\n",lab_count[0]);
  printf("Laboratório 2: %d\n",lab_count[1]);
  printf("Laboratório 3: %d\n",lab_count[2]);

  printf("Infectado 1: %d\n",infec_count[0]);
  printf("Infectado 2: %d\n",infec_count[1]);
  printf("Infectado 3: %d\n",infec_count[2]);

  return 0;
}