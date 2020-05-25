#ifndef MEMORIA_H_INCLUDED
#define MEMORIA_H_INCLUDED

#include "estruturas.h"
#include "defines.h"
#include "vetorAVL.h"
#include "grafo.h"
#include "maxList.h"

#include <list>
#include <utility> 
#include <iostream>

void contraiParesRecorrentes(int *part1, int *part2 , typeParticao *part3, typeGraph *grafo, typeNeigh **melhorVizinho,  typeDendr *dendr, int *lastMergeNv);
void contraiParesRecorrentesEliteSet(typeEliteSet *eliteSet, typeGraph *grafo, typeNeigh **melhorVizinho,  typeDendr *dendr, int *lastMergeNv);
//f
void contraiParesRecorrentes5EliteSet(typeEliteSet *eliteSet, typeGraph *grafo, typeNeigh **melhorVizinho,  typeDendr *dendr, int *lastMergeNv);

//Elite Set

typeEliteSet* inicializaEliteSet(int max); //f
void incluiEliteSet(typeEliteSet* elite , typeParticao *novaP, typeGraph *grafo); //f
void incluiEliteSet_best(typeEliteSet* elite , typeParticao *novaP, typeGraph *grafo); //f

typeParticao *escolheParticaoEliteSet(typeEliteSet *elite);

typeParticao *escolheParticaoEliteSet_notChoosen(typeEliteSet *eliteSet, int* choosen, int num);

typeParticao *escolheParticaoDistanteEliteSet(typeEliteSet *eliteSet, typeGraph *grafo, typeParticao *part);

void printEliteSet(typeEliteSet *elite);

//f
typeEliteSet* destroiEliteSet(typeEliteSet *elite);

typeParticao *getMelhorEliteSet(typeEliteSet *elite); //f


int encontraDistanciaClusterings(typeParticao *particao1, typeParticao *particao2, typeGraph *grafo);

int calcDistanciaClusters(typeCluster *Ci1 , typeCluster *Cj2);

void printCorrespondencias(typeParticao *particao);
void limpaCorrespondencias(typeParticao *particao2); //f


#endif // MEMORIA_H_INCLUDED
