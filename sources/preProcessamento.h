#ifndef PREPROCESSAMENTO_H_INCLUDED
#define PREPROCESSAMENTO_H_INCLUDED

#include "estruturas.h"
#include "defines.h"
#include "vetorAVL.h"
#include "coarsening.h"

void insertSinglList(typeSinglVList *list, int elem);
typeSinglVList* procSinglVertices(typeGraph* grafo); //f
int* recuperaSinglVMaxMapa(typeSinglVList *list, int *particao); //f
void printSinglVList(typeSinglVList *list);
typeSinglVList *destroiSinglVList(typeSinglVList *list); //f

#endif // PREPROCESSAMENTO_H_INCLUDED
