#ifndef COARSENING_H_INCLUDED

#define COARSENING_H_INCLUDED

#include "vetorAVL.h"
#include "estruturas.h"
#include "bestNeigh.h"
#include "dendrogramaVet.h"
#include "defines.h"
#include "listaPrecedencia.h"
#include "preProcessamento.h"
#include "maxList.h"

//f
typeDendr* coarsening(typeGraph *graph, double reductionFactor, int maxLevels, typeNeigh **bestNeigh,  typeDendr *dendr, int *lastMergeNv, int op_spec, char* resPNameBase, int* numGroupsSpec, int sizeBase); //09/11/2012, 231219: numGroupsSpec + sizeCalcSpec 080320

int greedyGrouping(typeGraph *graph, typeDendr *dendr, double reductionFactor,
                   typeNeigh **bestNeigh, typeMaxList* maxHeapGain, int *lastMergeNv, int numGroupsSpec); //f

void updateVTarg(typeGraph *graph, typeVertex *vTarg, typeVertex* vRem, typeElemDendr *merge); //f
typeVertex* removeVRem(typeGraph *graph, typeVertex *vRem, typeNeigh **bestNeigh , typeMaxList *maxList); //f

//f
void updateAdjs(typeGraph *graph, typeVertex *vTarg, typeVertex *vRem, typeNeigh **bestNeigh, typeMaxList *maxList, typeElemDendr *merge);

//f
typeEdgeAVL* insertCoarsed(typeEdgeAVL *pCoarsed, typeEdgeAVL *pRem);

//f
typeEdgeAVL* changeEdge( typeGraph *graph, typeVertex *vertK, typeVertex *vTarg, typeVertex *vRem,
                        typeNeigh **bestNeigh, double pound, typeEdgeAVL*, typeMaxList*);


//f
void calcTargAVLModularity(typeGraph *graph,typeNeigh **bestNeigh, typeMaxList *maxList, typeVertex *vert, typeVertex *vRem, typeEdgeAVL *neigh);

//f
void updateAllKtoTarg(typeGraph *graph,typeNeigh **bestNeigh, typeMaxList *maxList, typeVertex *vert, typeVertex *vRem, typeEdgeAVL *neigh);

//f
void updateAdjsArrow(typeGraph *graph, typeVertex *vTarg, typeVertex *vRem, typeNeigh **bestNeigh, typeMaxList *maxList, typeElemDendr *merge);
#endif
// COARSENING_H_INCLUDED
