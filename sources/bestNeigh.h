#ifndef BESTNEIGH_H_INCLUDED
#define BESTNEIGH_H_INCLUDED

#include "estruturas.h"
#include "estruturas.h"
#include "vetorAVL.h"
#include "defines.h"

double  calcGraphModularitySingl(typeGraph *graph); //f

void calcAVLModularity(typeGraph *graph,typeNeigh **bestNeigh, typeVertex *vert,  typeEdgeAVL *neigh); //MANTE

typeNeigh *bestAVLModularity(typeNeigh *bestEdge  , typeEdgeAVL *edge, int selfEdge); //f

typeElemMaxList* selectBestPair(typeMaxList *maxList, typeNeigh **bestNeigh, int *a, int *b);


double modularityIncrease(typeGraph *graph,int x, int y);
double modularityIncreaseUndoCluster(typeGraph *graph,int b, int a);
double modularityIncreaseIgual(typeGraph *graph, int x); //f
double modularityIncreaseDif(typeGraph *graph,int x, int y);

typeNeigh* wakitaCases(typeNeigh **,typeVertex * , typeEdgeAVL *edge, double oldQual, int, typeMaxList*);

double modularityIncreaseCluster(typeGraph *graph,int a, int b);

typeElemMaxList *searchPrev(typeMaxList *maxList, typeNeigh** bestNeigh, double val); //f
typeMaxList* insertMaxList(typeMaxList *maxList, typeElemMaxList *elem, typeElemMaxList *previous); //f
typeElemMaxList* removeMaxList(typeMaxList *maxList, int i); //f

typeMaxList* callInsertMaxList(typeMaxList *maxList, typeNeigh ** bestNeigh ,typeElemMaxList *elem); //f
typeMaxList* createMaxListGain(typeNeigh **bestNeigh, int size); //f
typeElemMaxList* createElemMaxList(int i);

typeElemMaxList *reinsert(typeMaxList *maxList, typeNeigh **neigh, typeElemMaxList *elem, int vert);

typeNeigh ** createBestNeigh(typeGraph *graph); //f

int* heapSort(int *, typeNeigh **bestNeigh , int n);

void printMaxHeap( int* maxHeap , typeNeigh **bestNeigh, int n );

int* createMaxHeapGain(typeNeigh **bestNeigh, int size);

typeNeigh **copyBestNeighNew(typeNeigh **fonte, int n); //f

typeNeigh** copyBestNeighDest(typeNeigh **fonte, typeNeigh **destino, int n); //f

typeElemMaxList* selectGraspPair(int graphSize, typeMaxList *maxList, double b8 , typeNeigh **bestNeigh, int *a, int *b); //f

void printBestNeighbor(typeNeigh **bestNeigh, int n);

void printMaxListGain(typeMaxList *maxList, typeNeigh **bestNeigh); //f

typeNeigh** destroiBestNeigh(typeNeigh **bestNeigh, int size); //f

void calcAVLModularityExp(typeGraph *graph, typeVertex *vert,  typeEdgeAVL *neigh);

#endif // BESTNEIGH_H_INCLUDED