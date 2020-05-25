#ifndef CLUSTERINGLIST_H_INCLUDED
#define CLUSTERINGLIST_H_INCLUDED

#include "clusteringList.h"
#include "estruturas.h"
#include "defines.h"
#include "vetorAVL.h"
#include "bestNeigh.h"

typeCluster *procuraIesimoCluster(typeClustering *clustering, int iesimo);
typeClustering* obtemClusteringList(int *particao , int nVert);

typeClustering* inicializaClustering();
typeElemCluster* criaElemCluster(int ind);

typeCluster* criaCluster(int ind);
typeClustering* criaClustering();

typeCluster *insereCluster(typeClustering *list , int ind);

//270213
typeCluster *procuraIesimoCluster(typeClustering *clustering, int iesimo);

typeCluster *procuraCluster(typeClustering *clustering, int ind);
typeCluster *procuraClusterAnt(typeClustering *clustering, int ind);
typeElemCluster *procuraElemClusterAnt(typeCluster *cluster , int ind);

typeCluster *insereVerticeClusterIsolado(typeClustering *clustering, typeCluster *cluster, int vert, int nVert);
typeCluster* insereVerticeNoClusterInd(typeClustering *clustering , int vert, int indCluster);

void insereVerticeNoClusterPtr(typeCluster *list , int vertice);

int verifClusterIsolado(typeCluster *cluster);

typeClustering* obtemClusteringList(int *particao , int nVert);
void printCluster(typeCluster *cluster);
void printClustering(typeClustering *clustering);

typeCluster *removeCluster(typeClustering* clustering, typeCluster *cluster);
typeElemCluster *removeElemClusterInd(typeClustering* clustering, int indCluster, int elem);
typeElemCluster *removeElemClusterPtr(typeClustering* clustering, typeCluster *cluster, int elem);

typeCluster *copiaCluster(typeCluster *fonte, typeCluster *destino);
typeClustering *copiaClustering(typeClustering *fonte);

typeClustering *destroiClustering(typeClustering *clustering);

#endif // CLUSTERINGLIST_H_INCLUDED
