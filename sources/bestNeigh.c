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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "bestNeigh.h"

/*calcula a modularidade inicial, com todos os cálculos
depois, só faz as atualizações!*/
//retorna: modularidade de todos os clusters singleton
//f
double  calcGraphModularitySingl(typeGraph *graph) {
    int i;
    double val = 0;
    graph->modularity = 0;

    ///printf("\nMODULARIDADE INICIAL\n");

    //calcula a modularidade inicial, considerando inclusive os vértices isolados que foram removidos
    for(i=1; i <=graph->nVert ; i++) {
        // printf("di %.2d,dj %.2d , m %.2d", graph->vDegree[i] , graph->vDegree[i] , graph->m) ;

        ///13122012: e se o vértice é NULL pois foi retirado do grafo por ser desconexo!!!
        ///13122012: nesse caso dG(v) = 0 e q(v,v) = 0!
        if(graph->vList[i-1]!=NULL) {
            val = modularityIncreaseIgual(graph, i);
            graph->modularity += val;
        }
        //     printf("i= %d val=%f\n",i ,  val);
    }

    graph->modularity = (double)graph->modularity;
//      printf("MODULARITY i= %d val=%lf, m = %d\n",i ,  graph->modularity, graph->m);

    return graph->modularity;
    //ve o que está em cada partição para calcular.
    //inicialmente cada um está na sua própria comunidade

}

//retorna a posição da estrutura bestNeigh correspondente atualizada
typeNeigh *bestAVLModularity(typeNeigh *bestEdge  , typeEdgeAVL *edge, int selfEdge) { //f
    /**Assert: */  //deve ser sempre verdadeira
//    assert(edge!=NULL);

    //DIRECTED GRAPH 020115
    if(edge!=NULL) {
        //laço nunca será o melhor vizinho
        if(edge->quality > bestEdge->quality && edge->vAdj != selfEdge) {

            bestEdge->label2 = edge->vAdj;
            bestEdge->quality = edge->quality;
        }
        if(edge->left!= NULL) bestAVLModularity(bestEdge , edge->left, selfEdge);
        if(edge->right!= NULL) bestAVLModularity(bestEdge , edge->right, selfEdge);

        return bestEdge;
    } else {

        return NULL;
    }
}

//faz os casos de verificação do wakita para definir novo melhor vizinho
//atualizar max List também

///caso contrário o removido continua sendo o melhor vizinho sem problemas
typeNeigh* wakitaCases(typeNeigh **bestNeigh ,typeVertex* vertK, typeEdgeAVL *edge, double oldQual, int rem, typeMaxList *maxList) {
    typeElemMaxList *elem;
    typeNeigh *bestNeighK  = bestNeigh[vertK->label-1];

    ///caso o melhor vizinho seja o vértice que foi removido, precisa procurar a adjacencia inteira

    //   printf("Wakita: maxList->first = %d, %d\n",maxList->first->i+1, bestNeigh[maxList->first->i]->label2);

    if(maxList==NULL) {
    }

    //     printf("Wakita: vertK->label %d\n",vertK->label);
    if(bestNeighK != NULL) {

        if(bestNeighK->label2 == rem) {
            elem = removeMaxList(maxList, vertK->label -1);
            //qualquer coisa substitui
            bestNeighK->quality = -2;
       
            bestNeighK= bestAVLModularity(bestNeighK, vertK->adjList , vertK->label);

            //insere o novo melhor
            // printf("Wakita (%d,%d) = %f\n",vertK->label, bestNeighK->label2, bestNeighK->quality);

            if(maxList!=NULL) {
                callInsertMaxList(maxList, bestNeigh, createElemMaxList( vertK->label-1 ) );
            }
        }
        //se não é o melhor vizinho do alvo ( contraído)
        else if (edge!=NULL && bestNeighK->label2 != edge->vAdj) {
            ///caso 1
            //e se a modularidade diminuiu não faz nada

            ///caso 2
            //se a mod. aumentou, precisa comparar com o valor do melhor vizinho
            if(edge->quality >oldQual && edge->quality > bestNeighK->quality) {
                //se é maior que a mod. do melhor vizinho...

                //remove o antigo melhor viznho

                removeMaxList(maxList, vertK->label-1);
                bestNeighK->label2 = edge->vAdj;

                bestNeighK->quality = edge->quality;

                if(maxList!=NULL) {
                    callInsertMaxList(maxList, bestNeigh, createElemMaxList( vertK->label-1 ) );
                }

            }
        }
        //se é o maior da comunidade
        else {
            ///caso 3

            //se a mod. aumentou não faz nada!
            //só atualiza o valor em bestNeig
            if(edge!=NULL &&  edge->quality > oldQual) {
                removeMaxList(maxList, vertK->label-1);
                bestNeighK->quality = edge->quality;

                if(maxList!=NULL) {
                    callInsertMaxList(maxList, bestNeigh, createElemMaxList( vertK->label-1 ) );
                }
            }

            ///caso 4
            //era o melhor vizinho e a qualidade diminuiu
            //pior caso : precisa procurar por todos as adjacencias para definir a melhor
            //se nao existir adjacencia, marca -1 na posicao
            if(edge!=NULL &&  edge->quality < oldQual) {

                removeMaxList(maxList, vertK->label-1);
                bestNeighK->quality = -2;
                bestNeighK= bestAVLModularity(bestNeighK, vertK->adjList , vertK->label);

                if(maxList!=NULL) {
                    callInsertMaxList(maxList, bestNeigh, createElemMaxList( vertK->label-1 ) );
                }
            }
        }
    }

    //  printf("Wakita: maxList->first = %d, %d\n",maxList->first->i, bestNeigh[maxList->first->i]->label2);

    return bestNeighK;
}

//ainda nao implementada
double modularityGraph(typeGraph *graph) {
    double mod=-2;
    return mod;
}

//f
//DIRECTED GRAPH 050115
double modularityIncreaseCluster(typeGraph *graph,int a, int b) { //DIRECTED GRAPH 31122014
    double kInA, kInB , kTotB, kTotA, arestaAB, arestaBA;
    double deltaQSimples = 0;
    //double deltaQF = 0;
    //(?) ao invez de guardar o grau interno, fazer pound(x,x)*2

    kTotA = graph->vDegree[a-1];
    kTotB = graph->vDegree[b-1];


    kInA = graph->vDegreeIn[a-1];
    kInB = graph->vDegreeIn[b-1];

    arestaAB = searchEdgePound(graph, a, b);
    arestaBA = searchEdgePound(graph, b, a);

//    printf("=d %lf\n",graph->vDegree[b-1]);
//    printf("=r %lf\n",graph->vDegreeArrow[b-1]);

    deltaQSimples = (arestaAB)/(double)(graph->m)   -  graph->LAMBDA_ML * (kTotA * graph->vDegreeArrow[b-1])/(  (long double)graph->m* (long double)graph->m);
    deltaQSimples += (arestaBA)/(double)(graph->m)   -  graph->LAMBDA_ML * (kTotB * graph->vDegreeArrow[a-1])/(  (long double)graph->m* (long double)graph->m);


    return deltaQSimples;
}


//desfazer o cluster feito durante a fase de contração
double modularityIncreaseUndoCluster(typeGraph *graph,int b, int a) {
    //o ganho é "desfeito"
    // obs: o cluster (b,a) deve ser separado (desfeito) antes dessa função ser chamada!
    double q = (double)(-1)* modularityIncreaseCluster(graph, b,a);
    return q;
}

//f
//DIRECTED GRAPH 050115
/* Q =  1/2m   *   ( w_ii - (di^{out} di^{in}) /2m )
*
*/
double modularityIncreaseIgual(typeGraph *graph, int x) {
    double mXY;

    mXY = (double) (
              (searchEdgePound(graph,x,x) -  graph->LAMBDA_ML * (    (graph->vDegree[x-1] * graph->vDegreeArrow[x-1]) /
                      ( (double)graph->m) ) )
          ) * (1.0/ (long double)graph->m) ;
    return mXY;
}

//DIRECTED GRAPH - 050115
///TODO: 01/10/2015 Check
double modularityIncreaseDif(typeGraph *graph,int x, int y) {
    double mXY;
	printf("ModularityIncreaseDif");
    mXY = (double) (
              (searchEdgePound(graph,x,y) - graph->LAMBDA_ML * (    (graph->vDegree[x-1] * graph->vDegreeArrow[y-1])/
                      ( (long double)2 * (long double)graph->m) )    )
          ) *
          (2.0/ (long double)graph->m ) ;

    mXY += (double) (
              (searchEdgePound(graph,y,x) - graph->LAMBDA_ML * (    (graph->vDegree[y-1] * graph->vDegreeArrow[x-1])/
                      ( (long double)2 * (long double)graph->m) )    )
          ) *
          (2.0/ (long double)graph->m ) ;
    return mXY;
}

//DIRECTED GRAPH - 050115
double modularityIncrease(typeGraph *graph,int x, int y) {
    int aXY,degreeX, degreeY;
    double mXY;
    aXY = searchEdgePound(graph,x,y);
//    printf("AXY %d\n", aXY);
    degreeX = graph->vDegree[x-1];
    degreeY = graph->vDegree[y-1];

    if(x!=y) {
        mXY = (double) ( ((double)searchEdgePound(graph,x,y) -  graph->LAMBDA_ML * ( (graph->vDegree[x-1] * graph->vDegreeArrow[y-1])/
                          ( (long double)graph->m) )) *
                         (1/ ((long double)graph->m)) );
        mXY += (double) ( ((double)searchEdgePound(graph,y,x) -  graph->LAMBDA_ML * ( (graph->vDegree[y-1] * graph->vDegreeArrow[x-1])/
                          (  (long double)graph->m) )) *
                         (1/ ((long double)graph->m)) );
    } else {
        mXY = (double) ( ( (double)searchEdgePound(graph,x,y) - graph->LAMBDA_ML * ((graph->vDegree[x-1] * graph->vDegreeArrow[y-1])/
                           ( (long double)graph->m) )) *
                         (1/ ((long double)graph->m)) );
    }

    return mXY;

}

//f
//retorna o índice do melhor par, referente ao que está armazenado em bestNeigh
//para fazer GRASP adaptar para um proc. semi-guloso, com lista de candidatos e etc
typeElemMaxList* selectGraspPair(int graphSize, typeMaxList *maxList, double seedP , typeNeigh **bestNeigh, int *a, int *b) {

    int i;
    int index;
    typeElemMaxList *choosen= NULL;
    int sizeSearch;

    if(SEED == -1) {
        seedP = (double)rand() / (double)RAND_MAX  ;//aleatorio de [0,1]
        sizeSearch = maxList->size * seedP;

    } else {
        sizeSearch = maxList->size * SEED;
    }

    if(sizeSearch==0) {
        index = 0;
        ///   index = 1; //solucao gulosa: grasp
    } else {

        index = rand()%sizeSearch;
    }

    //dentre os seed * |C| primeiros elementos



    /// printf("INDEX GRASP %d\n",index);
    choosen = maxList->first;

    for(i=0 ; i<index && choosen!=NULL ; i++) {
        //procura o elemento correspondente à index na LC
        //choosen = ??

        choosen = choosen->next;
    }

    //caso só exista 1 vértice/cluster no grafo, choosen == NULL;
    if(choosen!=NULL) {
        *a = choosen->i +1;
        *b = bestNeigh [choosen->i ]->label2;
    }
    return choosen;
}

//f
void calcAVLModularity(typeGraph *graph,typeNeigh **bestNeigh, typeVertex *vert,  typeEdgeAVL *neigh) {
//    assert(neigh!=NULL);

    if(neigh!=NULL) {

        //se for self edge
        if(neigh->vAdj == vert->label) {
            neigh->quality = modularityIncreaseCluster(graph, vert->label, neigh->vAdj);
        } else {
            neigh->quality = modularityIncreaseCluster(graph, vert->label, neigh->vAdj);
            //se o valor da seleção do vizinho for melhor que o já armazenado, substituo
            if( neigh->quality > bestNeigh[vert->label-1]->quality) {
                bestNeigh[vert->label - 1]->label2 = neigh->vAdj;
                bestNeigh[vert->label - 1]->quality = neigh->quality;

            }
        }

        if(neigh->left!= NULL) {
            calcAVLModularity(graph, bestNeigh, vert, neigh->left);
        }
        if(neigh->right!= NULL) {
            calcAVLModularity(graph, bestNeigh, vert, neigh->right);
        }
    }
}

//f
/*   mantem uma lista de melhor vizinho, ordenada por rotulo do vertice */
//retorna: lista com melhor vizinho de cada vértice
//obs: usando modularity increase!
typeNeigh ** createBestNeigh(typeGraph *graph) {
    int i;
    //insere na posição correta para evitar ordenação
    typeNeigh **bestNeigh = (typeNeigh**)  malloc(sizeof(typeNeigh*)*graph->nOrig);
    //só preciso armazenar a aresta, mais nada!
    for(i=1; i<=graph->nOrig; i++) {
        if(graph->vList[i-1]!=NULL) { //09/11/2012
            bestNeigh[i-1] = (typeNeigh*) calloc(sizeof(typeNeigh),1);
            bestNeigh[i-1]->quality = MENORQ; //menor que -1, garante que qualquer valor de mod./seleção será armazenado.
            bestNeigh[i-1]->label2 = -1;

            //calcula todos os valores de modularidade e define o melhor vizinho
            calcAVLModularity(graph, bestNeigh, searchVertex(graph->vList, i), searchVertex(graph->vList, i)->adjList );
        } else { //09/11/2012
            ///se essa função for chamada só antes das contrações iniciarem,
            ///só não aloca para os vértices isolados que foram pré contraídos

            ///alternativa é alocar e deixar como -9
            bestNeigh[i-1] = NULL;
        }
    }

    return bestNeigh;
}


void printBestNeighbor(typeNeigh **bestNeigh, int n) {
    int i;
    for(i=0; i<n; i++) {
        if(bestNeigh[i]!= NULL)
            printf("(%d,%d) = %lf\n",i+1, bestNeigh[i]->label2, bestNeigh[i]->quality);
    }
    printf("\n");
}

//f
typeElemMaxList *searchPrev(typeMaxList *maxList, typeNeigh** bestNeigh, double val) {
    typeElemMaxList *aux;

    if(val >= bestNeigh[maxList->first->i]->quality ) {

        //não existe anterior
        //o no a ser inserido vai pra 1a posicai
        return NULL;
    } else {
       
        aux = maxList->last;

        while(aux->prev != NULL  && val> bestNeigh[aux->i]->quality) {
            aux= aux->prev;
        }

        return aux;
    }
}

//aqui, ou a qualidade do que procuro é igual à do aux, ou é menor
//se for menor, é sinal que o elemento procurado não existe
//se for igual, precisa procurar


//f
//procura pelo elemento i  (que representa i , melhor par i) na lista e o remove
//pode retornar o proprio ponteiro, para não precisar realocar
///ATENÇÃO: i = vert->label-1
typeElemMaxList* removeMaxList(typeMaxList *maxList, int i) {
    //110213: maxList vai vir NULL de memoria.c
    typeElemMaxList *aux = NULL;
    if(maxList!=NULL) {
        aux = maxList->first;

        //enquanto a qualidade do elemento que procuro for menor que a do atual
        //precisa continuar procurando
        // printf("i=%d, first=%d\n",i, maxList->first->i);
        //remoção do primeiro
        if(maxList->first != NULL) {

            if( i == maxList->first->i) {
                //   printf("rem first");
                aux = maxList->first;
                if(maxList->first->next != NULL) {
                    maxList->first = maxList->first->next;
                    maxList->first->prev = NULL;
                } else {
                    maxList->first = NULL;
                    maxList->last = NULL;
                }

                maxList->size--;

            } else {
                //  printf("erro1\n");

                //procura o elemento aux com valor i
                while(aux!=NULL && aux->i != i) {
                    // printf(".aux %d %d\n",aux->i+1, i);
                    aux = aux->next;
                }
                //     printf("\n-----\n");


                //remove ultimo
                /** Assert: Aux!= NULL */
                if(aux!=NULL) {
                    if(aux == maxList->last) {
                        maxList->last = maxList->last->prev;
                        aux->prev->next = NULL;


                    }
                    //intermediario
                    else {
                        aux->prev->next = aux->next;
                        aux->next->prev = aux->prev;
                    }

                    maxList->size--;

                }
            }
            free(aux);
            aux = NULL;

            //retorna o que acabou de ser removido
            //ou achou ou não existe!
        }
    }

    return aux;
}

//f
typeMaxList* callInsertMaxList(typeMaxList *maxList, typeNeigh ** bestNeigh ,typeElemMaxList *elem) {
    typeElemMaxList *previous = NULL;

    //250218:
    double qual = 0;
    //110213: casos em que não exista a maxList, será enviado NULL propositalmente, para não reescrever montes de funções
    if(maxList!=NULL) {
        //ocorre quando a única adjacência do vértice seja o laço
        //esse não deve ser inserido na maxList

        ///precisa guardar o tamanho da maxList, para não permitir que o alfa escolha algo que não existe

        //250218
        #if DEF_SPEC == 1
            qual = -2;
        #endif

        if(bestNeigh[elem->i]->label2 != -1 && bestNeigh[elem->i]->quality> qual) { //220213
            if(maxList->first!=NULL) {
                previous = searchPrev(maxList, bestNeigh , bestNeigh[elem->i ]->quality);
            }

            maxList = insertMaxList(maxList , elem, previous);

        }
        //300413: leak
        else {
            free(elem);
            elem = NULL;
        }
    }

    return maxList;
}

//f
//insercao ordenada!
typeMaxList* insertMaxList(typeMaxList *maxList ,typeElemMaxList *elem, typeElemMaxList *previous) {


    if (maxList->first == NULL) {
        maxList->first = elem;
        maxList->last= elem;
    } else {

        //insere antes do primeiro
        if(previous == NULL) {
            maxList->first->prev = elem;
            elem->next = maxList->first;

            maxList->first = elem;

        } else {
            //se o previuous era o ultimo
            if(previous == maxList->last) {
                previous->next = elem;
                elem->prev = previous;
                //elem é o ultimo
                maxList->last= elem;
                //   printf("Ultimo inserido em %p\n",elem);

            } else {
                //insere depois do previous

                elem->next = previous->next;
                previous->next->prev = elem;

                elem->prev = previous;
                elem->prev->next = elem;

            }
        }
    }

    //30012013
    maxList->size++;
    return maxList;


}

typeElemMaxList* createElemMaxList(int i) {
    typeElemMaxList *elem = (typeElemMaxList*) malloc(sizeof(typeElemMaxList));
    elem->i = i;
    elem->next = NULL;
    elem->prev = NULL;

    return elem;
}

//f
//cria lista com os melhores ganhos para merge de vértices
typeMaxList* createMaxListGain(typeNeigh **bestNeigh, int size) {
    typeMaxList *maxList = (typeMaxList*) malloc(sizeof(typeMaxList));
    maxList->first = NULL;
    maxList->last = NULL;
    typeElemMaxList *elem;

    int i;


    maxList->size = 0;

    //insere todos os pares de melhor vizinho na lista
    //guarda rotulo-1;
    for(i=0; i < size; i++) {
        //insere 0, 1, ...
        //ordenados por
        //bestNeigh[0]->quality , bestNeigh[1]->quality, ...

        //já insere na posicao certa

        if(bestNeigh[i]!=NULL) { //09/11/2012
            elem = createElemMaxList( i );
            maxList = callInsertMaxList(maxList, bestNeigh , elem);
        }

    }
    //ordena a lista com quicksort

    return maxList;
}

//f
void printMaxListGain(typeMaxList *maxList, typeNeigh **bestNeigh) {
    typeElemMaxList *aux = maxList->first;
    printf("Max List Gain\n");
    while(aux!=NULL) {
        printf("%2d , %2d = %f\n", aux->i + 1, bestNeigh[aux->i]->label2 , bestNeigh[aux->i]->quality );
        aux= aux->next;
    }
    printf("\n");
}

//f
//é chamada quando a fase de contração termina
typeNeigh** destroiBestNeigh(typeNeigh **bestNeigh, int size) {
    int i;
    for(i=0; i<size; i++) {
        free(bestNeigh[i]);
        bestNeigh[i] = NULL;
    }

    free(bestNeigh);
    bestNeigh= NULL;

    return bestNeigh;
}

//f
typeNeigh **copyBestNeighNew(typeNeigh **fonte, int n) {
    int i;
    typeNeigh **novo = (typeNeigh ** ) malloc(sizeof(typeNeigh*)  * n);

    for(i=0; i<n ; i++) {
        if(fonte[i]!=NULL) {
            novo[i] = (typeNeigh*) malloc(sizeof(typeNeigh));
            novo[i]->label2 = fonte[i]->label2;
            novo[i]->quality = fonte[i]->quality;
        } else {
            novo[i] = NULL;
        }
    }

    return novo;
}

//f
typeNeigh** copyBestNeighDest(typeNeigh **fonte, typeNeigh **destino, int n) { 
    int i;
    for(i=0; i<n ; i++) {
        if(fonte[i]!=NULL && destino[i]!= NULL) { //09/11/2012
            destino[i]->label2 = fonte[i]->label2;
            destino[i]->quality = fonte[i]->quality;
        } else {
            destino[i] = NULL;
        }
    }

    return destino;
}

//f
void calcAVLModularityExp(typeGraph *graph, typeVertex *vert,  typeEdgeAVL *neigh) { 
    assert(neigh!=NULL);

    neigh->quality = modularityIncreaseCluster(graph, vert->label, neigh->vAdj);

    if(neigh->left!= NULL) {
        calcAVLModularityExp(graph, vert, neigh->left);
    }
    if(neigh->right!= NULL) {
        calcAVLModularityExp(graph, vert, neigh->right);
    }
}