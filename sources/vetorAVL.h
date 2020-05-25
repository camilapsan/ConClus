#ifndef VETOR_AVL_H_INCLUDED
#define VETOR_AVL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include "estruturas.h"

typeVertex* searchVertex(typeVertex** vertexList,int label); //f
void printAdjList(typeEdgeAVL *root);
void printGraph(typeGraph*);
int hashFuncion(int label);


typeVertex** createVertexList(int nVert);
typeVertex* createVertex(int label);

typeGraph* createGraph(int nVert);
double modularityIncreaseCluster(typeGraph *graph,int b, int a);


typeEdgeAVL *createEdgeAVL(int vertex, double pound);
int maior(int num1, int num2) ;
int calculaAltura(typeEdgeAVL *noAux);
typeEdgeAVL *rotaSimplesDir(typeEdgeAVL *k2);
typeEdgeAVL *rotaSimplesEsq(typeEdgeAVL *k2);
typeEdgeAVL *rotaDuplaDir(typeEdgeAVL *k3);
typeEdgeAVL *rotaDuplaEsq(typeEdgeAVL *k3);

int calculaFB (typeEdgeAVL *noAux);
typeEdgeAVL *verificaBalanceamento(typeEdgeAVL *noAux);
typeEdgeAVL *procuraNoCausa(typeEdgeAVL *noAux);
typeEdgeAVL* rotacionarNo(typeEdgeAVL *noDesb, typeEdgeAVL *noCausa);
typeEdgeAVL *balancear(typeEdgeAVL *noAux);
typeEdgeAVL *balancearAteRaiz(typeEdgeAVL *aux);

typeEdgeAVL* insereArvore(typeEdgeAVL *noAux, typeEdgeAVL *vertex);

typeEdgeAVL* includeEdgeSimples(typeGraph *graph, typeVertex *vertX, typeVertex *vertY, double pound);
void includeEdge(typeGraph *, typeVertex *vertX, typeVertex *vertY, double pound); //f

typeEdgeAVL* includeEdgeSimplesArrow(typeGraph *graph, typeVertex *vertX, typeVertex *vertY, double pound); //f

typeEdgeAVL *maiorValor(typeEdgeAVL *aux);


double searchEdgePound(typeGraph *graph, int label1, int label2); //f
typeEdgeAVL* searchEdge(typeVertex** vertexList, int label1, int label2); //f
typeEdgeAVL* searchEdgeArrow(typeVertex** vertexList, int label1, int label2); //191118


typeEdgeAVL *removeAdjList(typeEdgeAVL *aux, typeGraph *graph);
typeGraph* removeVertex(typeGraph *graph, int rem);
typeEdgeAVL *removeArvore(typeEdgeAVL *noAux, int info);
#endif // VETOR_AVL_H_INCLUDED
