#ifndef GRAFO_H_INCLUDED
#define GRAFO_H_INCLUDED


#include "estruturas.h"
#include "vetorAVL.h"
#include "buscaLocal.h"
#include "dendrogramaVet.h"
#include "clusteringList.h"

//f
int calcMaxNiveis(typeGraph *grafo, double fatReducao); //f

double calculaModularidadeGrafo(typeGraph *grafo, typeParticao *particao); //f
double calculaModularidadeGrafoS(typeGraph *grafo, typeParticao *particao);

double calculaModularidadeGrafoInt(typeGraph *grafo, int *mapa); //f

typeGraph *createGraph(int size);
void printGraph(typeGraph *graph);
void printGraphArrow(typeGraph *graph);

void printGraphFile(typeGraph *graph, FILE *file);
void printAdjListFile(typeEdgeAVL *root, FILE *file);
typeEdgeAVL* copyAVL(typeEdgeAVL *source, typeEdgeAVL *dest);

//f
typeGraph *copyGraph(typeGraph *source);

typeGraph *copyGraphDest(typeGraph *source, typeGraph *graph);

typeGraph *copyGraphModularity(typeGraph *fonte, typeGraph *destino);

typeGraph *resetaGrafoNovaIteracao(typeGraph *source, typeGraph *graph, typeDendr *dendr);

typeGraph *destroiGrafo(typeGraph *grafo); //f

void printGraphDegree(typeGraph *graph);
void printGraphDegreeIn(typeGraph *graph);

void printPajekFile(typeGraph *graph);


#endif // GRAFO_H_INCLUDED
