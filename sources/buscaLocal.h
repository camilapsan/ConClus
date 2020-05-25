#ifndef BUSCALOCAL_H_INCLUDED
#define BUSCALOCAL_H_INCLUDED

#include "estruturas.h"
#include "defines.h"
#include "vetorAVL.h"
#include "bestNeigh.h"
#include "grafo.h"
#include "dendrogramaVet.h"


//f
double calcTermoGanhoClusterIsolado(typeGraph *grafo, typeParticao *particao, int i);

typeMove *calcMelhorMoveVert(typeGraph *grafo,  typeParticao *particao, typeMove **melhorMove, typeVertex *vert); //f

double calcTermoQ(typeGraph *grafo, int label1, int label2); //f

double calcContribuicao( typeGraph *grafo, typeVertex *vert, typeParticao *particao); //f

double modularidadeClusterMove(typeGraph *grafo, typeClustering *clustering, typeCluster* cluster, int vertMovido);

//f
double calcGanhoModularidadeMove(typeGraph *grafo, typeClustering *clustering, int vertMovido , typeCluster* clusterOrig, typeCluster*clusterDest);

double calcContribuicaoDest(typeGraph *grafo, typeVertex *vert_move, typeVertex* vert_dest, typeParticao *particao); //f

int * shuffle(int *vet, int n); //f

#endif // BUSCALOCAL_H_INCLUDED
