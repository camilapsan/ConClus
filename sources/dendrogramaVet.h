#ifndef DENDOGRAMALDE_H_INCLUDED
#define DENDOGRAMALDE_H_INCLUDED


#include "estruturas.h"
#include "vetorAVL.h"
#include "coarsening.h"
#include "defines.h"
//#include "clusteringList.h"
#include "particao.h"

//220513
int *copyLastMergeNv(int *source , int *dest, int size);
typeDendr *createDendr(); 
//220218
typeDendr* createDendr(int maxLevels , double redFac, int nVert); //f

int *createLastMergeNv(int size); //f

typeElemDendr* includeMergeDendr(typeDendr *dendr, int *lastMergeV, int a , int b); //f

void includeFunctionDendr(typeDendr *dendr, typeElemDendr *merge);

typeBeginGraphNv *createBeginGraphNv();

typeBeginGraphNv *insertBeginGraphNv(typeBeginGraphNv *top , int level);  //f

typeKChanged *insereKChanged(typeKChanged *topK, int vert, double peso);

int *inicializaMapaInt(int n);

int *copiaMapaDestinoInt(typeCluster **fonte, int *destino, int n);

typeCluster** copiaMapaDestinoMapa(typeCluster **fonte, typeCluster **destino, int n);

typeCluster **copiaMapaAloca(typeCluster **fonte, int n);

typeKChanged* removeTopoKChanged(typeKChanged *list);

typeKChanged *destroiChangedList(typeKChanged *top);

typeDendr *destroiDendr(typeDendr *dendr, int maxQtd); //f

void printKChangedList(typeKChanged *list);

void printDendogram(typeDendr *dendr);

void printListRemoved(typeGraph *graph, int n);

void printDendrogram(typeDendr *dendr);

#endif // DENDOGRAMALDE_H_INCLUDED
