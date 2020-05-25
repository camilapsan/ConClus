#ifndef PARTICAO_H_INCLUDED
#define PARTICAO_H_INCLUDED

#include "estruturas.h"
#include "vetorAVL.h"
#include "coarsening.h"
#include "defines.h"
#include "clusteringList.h"
#include "dendrogramaVet.h"


//f
int *particaoTradicional(typeGraph *grafo, typeDendr *dendr, typeBeginGraphNv *beginGraphNv, int nvEscolhido);

void printParticao(typeParticao* particao, int size);

void printParticaoIntFileName(int* particao, int size, char *nomeArq); //f

typeParticao *inicializaParticao(int n);

typeParticao* copiaParticao(typeParticao *fonte, typeParticao *destino, int n);

typeParticao* copiaParticaoAloca(typeParticao *particao, int n);

int *copiaMapaIntDestinoInt(int *fonte, int *destino, int n);

typeParticao* destroiParticao(typeParticao *particao);

//f
typeParticao* criaParticaoFromInt(int* partInt, typeGraph *grafo);

int *copiaMapaDestinoInt(typeCluster **fonte, int *destino, int n);
#endif 
// PARTICAO_H_INCLUDED
