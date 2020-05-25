/**************************************************************************
*	 ConClus software
*
*	 Copyright (c) 2015 Camila Santos, Mari� Nascimento
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

#include "dendrogramaVet.h"

//f
int *createLastMergeNv(int size)
{
    int *lastMNV = (int*) calloc(sizeof(int),size);
    int i;

    for(i=0;i< size;i++)
    {
        lastMNV[i] = -1;
    }

    return lastMNV;
}

//220513
int *copyLastMergeNv(int *source , int *dest, int size)
{
      int i;

    for(i=0;i< size;i++)
    {
        dest[i] = source[i];
    }

    return dest;
}

void printLastMergeNv(int *last, int n)
{
    int i;
    printf("LASTNV\n");
    for(i=0; i < n ; i++)
    {
//        if(last[i]!= -1)
        printf("%d = %d\n",i,last[i]);
    }

}

//f
typeDendr* createDendr(int maxLevels , double redFac, int nVert)
{
    typeDendr *dendr = (typeDendr*) malloc(sizeof(typeDendr));
    
    int qtd = nVert + 1;

    dendr->list = (typeElemDendr**) calloc(sizeof(typeElemDendr*),qtd);
    if(dendr->list == NULL)
    {
        printf("mem�ria insuficiente");
        return NULL;
    }
    dendr->nMerge = 0;
    dendr->nNv = 0;
    
    dendr->beginGraphNv = createBeginGraphNv();
     dendr->beginGraphNv = insertBeginGraphNv( dendr->beginGraphNv , dendr->nMerge);


    return dendr;
}

typeElemDendr *createElemDendr(int a , int b , int nv, int label1, int label2)
{

    typeElemDendr *novo = (typeElemDendr*) malloc(sizeof(typeElemDendr));

    novo->label1 =label1;
    novo->label2 = label2;
    novo->col1 = a;
    novo->col2 = b;

    novo->changeEdgeList = NULL;
    novo->changeEdgeArrow = NULL;

    return novo;
}


//A lista dupl. encadeada que armazena o dendograma n�o faz distin��o entre qual n�vel de contra��o do grafo,
//somente qual "n�vel" de contra��o de v�rtices

//f
//insere sempre no topo do dendograma
typeElemDendr* includeMergeDendr(typeDendr *dendr, int *lastMerge, int a , int b )
{
    int nvA, nvB;
    int novoA, novoB, label1, label2;

    nvA= lastMerge[a-1];
    nvB = lastMerge[b-1];
    typeElemDendr *merge;


   //primeira contracao de a
    if(nvA  == -1 )
    {
        novoA = a*(-1); //armazena -1 para a

        //primeira contra��o, o label � o pr�prio A
        label1 = a;

        //quantidade antes de incrementar � o n�vel atual (comeca do zero!)
    }
    // a ja � um cluster
    else
    {
        //insiro um n�mero POSITiVO, com valor nvAtual - nvUltimaContracao
        novoA = dendr->nMerge - lastMerge[a-1];
        label1 = dendr->list[lastMerge[a-1]]->label1;
    }

    //analogo ao caso de a
    //primeira contracao de b
    if(nvB== -1)
    {
        novoB = b*(-1); //armazena -1 para a
        label2 = b;
    }
    else
    {
        novoB= dendr->nMerge - lastMerge[b-1];
        label2 = dendr->list[lastMerge[b-1]]->label1;//apagar depois!!
    }

    //cria uma contra��o de v�rtices para ser adicionada no dendrogramaLDE
    merge = createElemDendr(novoA, novoB, dendr->nMerge, label1,label2);
    //printf("nvmerge %d\t",dendr->nMerge);

    dendr->list[ dendr->nMerge ] = merge;

    // a �ltima contra��o de ambos os v�rtices � nesse n�vel
    lastMerge[a-1] = dendr->nMerge ;
    lastMerge[b-1] = dendr->nMerge ;

    (dendr->nMerge)++;

    return merge;
}

//f
typeBeginGraphNv *insertBeginGraphNv(typeBeginGraphNv *top , int level)
{
    typeBeginGraphNv *elem = (typeBeginGraphNv*)malloc(sizeof(typeBeginGraphNv));
    elem->i = level;
    elem->ant = top;

    return elem;
}

typeBeginGraphNv *createBeginGraphNv()
{
    return NULL;
}

void printBeginGraphNv(typeBeginGraphNv *graphNv)
{
    typeBeginGraphNv *aux = graphNv;
    printf("\nBegin Graph Nv\n");
    while(aux!=NULL)
    {
        printf("Inicio do nivel = %d\n",aux->i);
        aux = aux->ant;
    }

}

/** LISTA DE ARESTAS MODIFICADAS */

typeKChanged *alocaKChanged(int vert, double peso)
{
    typeKChanged *novo  = (typeKChanged*) malloc(sizeof(typeKChanged));
        //aloca 1 inteiro
    novo->kChanged = vert; //
    novo->peso = peso;

    return novo;
}

typeKChanged *insereKChanged(typeKChanged *topK, int vert, double peso)
{
    typeKChanged *novo = alocaKChanged(vert, peso);

    if(topK == NULL)
    {
        topK = novo ;
        topK->ant = NULL;
    }
    else// insere no topo da pilha
    {
        novo->ant = topK;
        topK = novo;
    }

    return topK;
}

typeKChanged* removeTopoKChanged(typeKChanged *list)
{
    typeKChanged *aux = list->ant;
    list->ant = NULL;
    free(list);
    list = aux;

    return list;
}

typeKChanged *destroiChangedList(typeKChanged *top)
{
    typeKChanged *aux = top;

    while(top!=NULL)
    {
        aux = top->ant;
        top = removeTopoKChanged(top);
        top = aux;
    }

    return top;
}

/** IMPRESSOES */

void printKChangedList(typeKChanged *list)
{
    //printf("Lista de arestas modificadas\n");
    printf("  Arestas modif.: ");
    while(list!=NULL)
    {
        printf("%2d (%.0lf)   ",list->kChanged, list->peso);
        list = list->ant;
    }
    printf("\n");
}

void printDendrogram(typeDendr *dendr)
{
    typeElemDendr *aux;
    int i;

    printf("DENDOGRAMA\n");
    printf("nMerge %d\n",dendr->nMerge);

    for(i=0; i<dendr->nMerge ; i++)
    {
        aux = dendr->list[i];
        printf("%3d %3d %2d \t l1 %2d  l2 %2d\n",i , aux->col1, aux->col2, aux->label1, aux->label2);
        //lista de arestas modif.
        printKChangedList(aux->changeEdgeList);
        printf("\n");
    }
}


void printListRemoved(typeGraph *graph, int n)
{
    int i;
    printf("VERTICES REMOVIDOS\n");
    for(i=0;i<n; i++)
    {
        if( graph->vList[i] == NULL)
        {
            printf("%d REM\n ",i+1);
        }
        else
        {
            printf("%d EXIST\n ",i+1);

        }
    }
}

//f
typeDendr *destroiDendr(typeDendr *dendr , int maxQtd)
{
    int i;
    for(i=0; i<maxQtd ;i++)
    {
        if(dendr->list[i]!=NULL && dendr->list[i]->changeEdgeList!=NULL){
        dendr->list[i]->changeEdgeList =destroiChangedList( dendr->list[i]->changeEdgeList);
        dendr->list[i]->changeEdgeList = NULL;
        }

      free(dendr->list[i]);
      dendr->list[i] = NULL;
    }

    free(dendr->list);
    dendr->list = NULL;
    free(dendr);
    dendr = NULL;

    return dendr;
}


