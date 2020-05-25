/**************************************************************************
*	 ConClus software
*
*	 Copyright (c) 2015 Camila Santos, Mariá Nascimento
*
*	 This file is part of ConClus software.
*
* 	 ConClus software is free software: you can redistribute it and/or modify
*    it under the terms of the GNU Affero General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    ConClus software is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU Affero General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*****************************************************************************/

#include "preProcessamento.h"


typeSinglVList *createSinglVList()
{
    typeSinglVList* list = (typeSinglVList*) malloc(sizeof(typeSinglVList));
    list->first = NULL;
    list->last = NULL;

    return list;
}

typeSinglV *createSinglV(int elem)
{
    typeSinglV *novo = (typeSinglV*) malloc(sizeof(typeSinglV));
    novo->label = elem;
    novo->next = NULL;

    return novo;
}

//f
typeSinglVList *destroiSinglVList(typeSinglVList *list){
    typeSinglV *aux = list->first;
    typeSinglV *aux2 = NULL;

    while(aux!=NULL){
        aux2 = aux;
        aux = aux->next;

        free(aux2);
        aux2 = NULL;
    }

    list->first = NULL;
    list->last = NULL;

    free(list);
    list = NULL;

    return list;

}

void insertSinglList(typeSinglVList *list, int elem)
{
    typeSinglV* novo = createSinglV(elem);

    //lista vazia
    if(list->last == NULL)
    {
        list->first = novo;
        list->last = novo;
    }
    //insere depois do ultimo
    else
    {
        list->last->next = novo;
        list->last = novo;
    }
}

/*  percorre o grafo procurando por vértices isolados (v->adjList == NULL)
    remove esses vértices do grafo e os salva em uma lista*/
//retorna: lista com vertices removidos
//f
typeSinglVList* procSinglVertices(typeGraph* grafo)
{
    int i;
    typeSinglVList *list = createSinglVList();

    grafo->nDesconex = 0 ;///200113

    for(i=0; i<grafo->nOrig ; i++)
    {
        //nenhum vertice é NULL, pois a contração do grafo ainda não foi iniciada
        //singl. vertice
        //17/01/13: se só possuir laço também deve entrar (e guardar o peso?).
        //DIRECTED GRAPH 31122014 - degree-in = degreeArrow tem que ser 0 também
        if(grafo->vList[i]->adjList == NULL && grafo->vDegreeArrow[i] == 0)
        {
            //remove do grafo
            free(grafo->vList[i]);
            grafo->vList[i] = NULL;

            //guarda na lista
            insertSinglList(list, i+1);
            grafo->nDesconex++; ///200113
        }

    }

    return list;
}

 //f
//13122012
//insere os vértices desconexos (singletons) no mapa da partição
int* recuperaSinglVMaxMapa(typeSinglVList *list, int *mapa)
{
    typeSinglV *aux = list->first;

    while(aux!=NULL)
    {
        mapa[aux->label-1] = aux->label;
        aux = aux->next;
    }

    return mapa;
}


void printSinglVList(typeSinglVList *list)
{
    typeSinglV *aux = list->first;

    printf("Lista de vertices isolados (Singletons)\n");
    while(aux!=NULL)
    {
        printf("%d\n", aux->label);
        aux = aux->next;
    }
}
