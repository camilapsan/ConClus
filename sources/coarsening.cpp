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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#include "coarsening.h"

/** FASE DE CONTRA��O */
/*
    A partir do grafo original, efetua diversas contra��es de v�rtices, at� obter o grafo base
    
    //APAGAR RELEASE
    fev/2018, 022018: T�cnica espectral para determinar o n�mero de grupos/clusters

*/


///COARSENING--------------------------------------------------------------------------------------------

//f
typeVertex* removeVRem(typeGraph *graph, typeVertex *vRem, typeNeigh **bestNeigh , typeMaxList *maxList) {
    //remove entrada do grafo removido da lista de melhor vizinho
    //01/12/2013
    
    //DIRECTED GRAPH 020115
    if(bestNeigh[vRem->label-1] != NULL) {
        bestNeigh[vRem->label-1]->label2 = -1;
        bestNeigh[vRem->label-1]->quality = -1; 
    }

    //remove de maxGainList
    removeMaxList(maxList, vRem->label-1);
    //remove o vertice rem. do grafo
    //APAGAR DEBUG, APAGAR RELEASE, CONF falta desalocar os ponteiros dele
    graph = removeVertex(graph, vRem->label);
    //APAGAR DEBUG, APAGAR RELEASE, CONF free(vRem);
     //APAGAR DEBUG, APAGAR RELEASE, CONF vRem = NULL;

    return NULL;
}

//f
void updateVTarg(typeGraph *graph, typeVertex *vTarg, typeVertex* vRem, typeElemDendr *merge) {
    ///Atualiza grau de vTarg
    //grau externo //19/10 , 22/10

    graph->vDegree[vTarg->label-1] += (graph->vDegree[vRem->label-1]);
    //DIRECTED GRAPH - 050115 -
    graph->vDegreeArrow[vTarg->label-1] += (graph->vDegreeArrow[vRem->label-1]);

    //grau interno //19/10 , 22/10
    //self edge conta como 2
    //                 dGin(targ) = dGin(targ) +     dGinRem      +        2* #(targ,rem)

    //DIRECTED GRAPH - 050115 - soma peso de vRem -> vTarg
    graph->vDegreeIn[vTarg->label-1]+= graph->vDegreeIn[vRem->label-1] + searchEdgePound(graph , vTarg->label, vRem->label) + searchEdgePound(graph , vRem->label, vTarg->label);

    ///Remove o v�rtice da lista de adjac�ncias de targ
    //DEBUG    /**Assert: */  //deve ser sempre verdadeira DIRECTED GRAPH 31122014
    //DEBUG    assert(vTarg->adjList != NULL); DIRECTED GRAPH 31122014

    vTarg->adjList = removeArvore(vTarg->adjList , vRem->label );
    //caso vRem seja a unica adj de vTarg faz-se necessaria a vertif. abaixo
    if(vTarg->adjList != NULL) {
        vTarg->adjList->parent = NULL;
    }

    vTarg->adjArrow = removeArvore(vTarg->adjArrow , vRem->label );
    //caso vRem seja a unica adj de vTarg faz-se necessaria a vertif. abaixo
    if(vTarg->adjArrow != NULL) {
        vTarg->adjArrow->parent = NULL;
    }
}

//f
int greedyGrouping(typeGraph *graph, typeDendr *dendr, double reductionFactor, typeNeigh **bestNeigh,typeMaxList* maxList, int *lastMergeNv, int numGroupsSpec) {
    int a, b, troca;
    double qual;
    typeElemMaxList *elem = NULL;
    typeElemDendr *merge = NULL;
    typeEdgeAVL *edge = NULL;

    typeVertex *vTarg = NULL,*vRem = NULL;
    int mergeCount = reductionFactor * (graph->nVert  - graph->nDesconex ) + 1; ///280113
    int test = mergeCount;

    int countQual0=0;
    //n�vel inicial � o zero

    //clusterings come�am a ser contados a partir do 0 e v�o at� mergeCount

    int min_qual = 0;
    int limQual0 = graph->nOrig * PERCLIMQUAL0;

    //APAGAR RELEASE
    printf("Lim qual 0 = %d\n", limQual0);

    while(mergeCount > 0  && graph->nVert > numGroupsSpec && countQual0<=limQual0) { //240120
        vTarg = NULL;
        vRem = NULL;

        if(graph->nVert<=0){
            printf("size maxlist=%d\n", maxList->size);
            printMaxListGain(maxList, bestNeigh);
            printGraph(graph);
        }

        elem = selectGraspPair( graph->nVert - graph->nDesconex , maxList , SEED ,bestNeigh , &a , &b);

        if(elem!=NULL ) {
//          DIRECTED GRAPH 31122014
            vTarg = searchVertex(graph->vList,a);
            vRem = searchVertex(graph->vList,b);
            qual = bestNeigh[elem->i]->quality;

             //APAGAR RELEASE BEGIN
            /* nunca deve ser verdadeira */
            if(a == b) {
                printf("FALHOU\n");
                return -1;
            }
            //APAGAR RELEASE END

            //parar de contrair quando n�o valer mais a pena ou quando acabarem os v�rtices (caso extremo, n�o � pra ser atingido)
            ///para de contrair quando escolher um par com mod <=0 210113
            ///deixar de contrair quando escolher x pares que n�o aumentem a modularidade

            ///080320: allow worsening modularity? 
            if(qual>min_qual && vTarg!= NULL && vRem != NULL ) {
                min_qual = 0;
                countQual0 = 0; //090320 reset

                //DEBUG
                #if PRINTALG == TRUE                                        
                    printf("*** MERGE %d %d\n", vTarg->label, vRem->label); ///REM  //DEBUG 1118                                        

                #endif

                //� garantido que ser�o contra�dos, guarda na estrutura de merge ja
                merge=includeMergeDendr(dendr, lastMergeNv, vTarg->label,vRem->label);
                edge = searchEdge(graph->vList, vRem->label , vRem->label) ;
                //guarda laco do vRem, caso exista
              
                ///esse � o ganho da modularidade
                graph->modularity += qual;

                //atualizacoes de coisas gerais
                //garante que QUALQUER valor de modularidade ir� sobreescrever o melhor vizinho

                ///Att. vTarg
                updateVTarg(graph, vTarg , vRem, merge);
             
                //Atualiza ref a adj. do grafo
                //27/10/2012: manda o merge tamb�m, para que a lista de arestas possa ser inclu�da
                //vTarg -> vRem ou vTarg <- vRem ou vTarg <-> vRem
                //lista de arestas com ponta (out) saindo de vRem
                updateAdjs(graph, vTarg, vRem,bestNeigh, maxList, merge);
                //lista de arestas com ponta (in) chegando em vRem
                updateAdjsArrow(graph, vTarg, vRem,bestNeigh, maxList, merge); //f
                
                //remove da lista da maxList
                removeMaxList(maxList, vTarg->label-1);
                bestNeigh[vTarg->label-1]->quality = -2;

                ///calcAVLModularity + updateKtoTarg
                calcTargAVLModularity(graph, bestNeigh , maxList, vTarg, vRem, vTarg->adjList);
                updateAllKtoTarg(graph, bestNeigh , maxList, vTarg, vRem, vTarg->adjArrow);

                //CalcUpdateKtoTarg - adjArrow of vTarg
                //calcula qualidade de TODOS os v�rtices adjacentes ao Targ e define um novo melhor;
                callInsertMaxList(maxList, bestNeigh , createElemMaxList( vTarg->label-1   ) );

                //atualiza bestNeigh
                ///X vRem; mandar para funcao separada
                vRem = removeVRem(graph, vRem, bestNeigh, maxList); //DIRECTED GRAPH 020115

                mergeCount--;
            }
            ///casos em que a contra��o � interrompida
            else {
                countQual0+=1;
            }
        } else{
            return -1;
        }
    }

    if(countQual0 > limQual0){
               return -1;
    }
    
    return 1;
}

//f
typeDendr* coarsening(typeGraph *graph, double reductionFactor, int maxLevels, typeNeigh **bestNeigh,
                      typeDendr *dendr, int *lastMergeNv, int op_spec, char* resPNameBase, int* numGroupsSpec,
                      int sizeCalcSpec) { 
    //APAGAR RELEASE
    //+bestNeigh 09/11/2012 // + sizeCalcSpec 080320
    ///NIVEL DO GRAFO refere-se a um conjunto de contra��es realizadas que corresponde a um n�vel
    ///NIVEL DO MERGE/CONTRACAO � qual o n�mero da contra��o no total.


    int l, continua = 1, mergeAnt;
    //armazena o indice do melhor vizinho de cada um dos v�rtices: typeNeigh **bestNeigh;

    //max gain � uma lista duplamente encadeada com os melhores ganhos
    typeMaxList *maxListGain;
    typeBeginGraphNv *aux = NULL;

    //clock_t start = clock();
    short defGroups=0;
    maxListGain = createMaxListGain(bestNeigh , graph->nOrig);
    dendr->nNv = 1;
    mergeAnt = dendr->nMerge;

    //APAGAR RELEASE
    //SPECTRAL: 
    //fev 2018: stop before maxLevels and perform spectral clustering to identify number of groups
    //stopping condition to find the number of groups: 
        //graph->nOrig/4
        //half/ /4 the number of levels
    //must convert graph to matrix
    //find number of groups
    //continues the coarsening
    //stop when achieves the number of groups

    if(sizeCalcSpec<=0){
        sizeCalcSpec = graph->nOrig;
    }

     if(graph->nVert > (*numGroupsSpec)){
        for(l = 0; graph->nVert>(*numGroupsSpec) && continua==1; l++) {

            //OP OV
            //APAGAR RELEASE BEGIN
            /*
            if(op_spec==1) {
                ///TODO: ONLY NEED TO CALC NUMBER OF GROUPS IN THE FIRST ITERATION OF GRASP!!                
                //ISCO: always recalculate numGroupsSpec
                if((graph->nVert <= graph->nOrig|| graph->nVert<=sizeCalcSpec)  &&  (*numGroupsSpec) < 0) { //231219 //080320 sizeCalcSpec
                    printf("SPEC lv=%d; N=%d\n",l,graph->nVert);                            
                    *numGroupsSpec = (int)( (double)Spec.SpectralNumGroups(graph) *0.7 );
                    printf("numGroupsSpec=%d\n",*numGroupsSpec);
                    defGroups=1;
                }
            }
            //APAGAR RELEASE END
            */

            //retorna o dendograma do grafo anterior
            //lidar com dendograma entre niveis

            //o n�vel atual inicia no valor da quantidade de merges atual
            dendr->beginGraphNv = insertBeginGraphNv( dendr->beginGraphNv , dendr->nMerge);

            //o dendograma, o grafo e a max heap s�o atualizados nessa fun��o
            continua = greedyGrouping(graph, dendr, reductionFactor,bestNeigh, maxListGain, lastMergeNv, *numGroupsSpec);
           
            // 220513: cria um novo nivel
            dendr->nNv++;

            printf("lv=%d; N=%d; m=%lf\n",l,graph->nVert,graph->m);   

            //APAGAR RELEASE 
            //181119: save partition in file
            /*
            char *resPNameMulti = (char*) calloc(sizeof(char),400); //011118
            int* partInt = particaoTradicional(graph, dendr, dendr->beginGraphNv, 1000);
            sprintf( resPNameMulti, "%s_lv_%d.com", resPNameBase , l);
            printf("Save part (coarsening). lv %d in %s\n",l, resPNameMulti);                        
            printParticaoIntFileName(partInt, graph->nOrig , resPNameMulti);

            free(partInt);
            partInt=NULL;
            free(resPNameMulti);
            resPNameMulti=NULL;
            */
        }  

        if(mergeAnt == dendr->nMerge) { //remove esse nv se nenhuma contr. ocorreu
            aux = dendr->beginGraphNv->ant;
            free(dendr->beginGraphNv );
            dendr->beginGraphNv = aux;
            dendr->nNv--;
        }
    }
    clock_t end = clock();

    maxListGain = destroiMaxList(maxListGain);

    return dendr;
}

///ATUALIZA��ES DO GRAFO ---------------------------------------------------------

//APAGAR RELEASE
//otimiz: jogar essa fun��o dentro da que calcula a modularidade da avl de targ->adjList
//APAGAR RELEASE
//?essa funcao est� atualizando todos os valores de modularidade   (k, targ), inclusive os que j� haviam sido em changeEdge?

void *updateKtoTarg(typeGraph *graph, typeEdgeAVL *pK, typeVertex *vTarg, typeVertex *vRem,
                    typeNeigh **bestNeigh, typeMaxList* maxList) {
    typeEdgeAVL *pVKTarg;
    typeVertex *vertK;
    double oldQual;

    //Assert:   //deve ser sempre verdadeira
    if(pK!=NULL) {
        vertK = searchVertex(graph->vList , pK->vAdj);

        pVKTarg = searchEdge( graph->vList , vertK->label , vTarg->label );

        if(pVKTarg!=NULL) {
            //caso k fosse adj vRem, oldQual na verdade era at
            oldQual = pVKTarg->quality;

            //wakitaa cases!
            pVKTarg->quality = modularityIncreaseCluster(graph, vertK->label , vTarg->label);
            //permite que o la�o seja selecionado como melhor vizinho!
            if(vertK->label != vTarg->label) {
                bestNeigh[vertK->label -1] = wakitaCases(bestNeigh,vertK, pVKTarg , oldQual, vRem->label, maxList);
            }
            //wakita
            //unico valor da modularidade  que muda � entre vertK --- vTarg
            //precisa recalcular
        }


        if(pK->right!=NULL) updateKtoTarg( graph,  pK->right, vTarg , vRem, bestNeigh , maxList);
        if(pK->left!= NULL) updateKtoTarg( graph,  pK->left ,  vTarg , vRem, bestNeigh , maxList);
    } else {
        //APAGAR RELEASE
        printf("ERRO: adjList == NULL\n");
    }

    return NULL;
}

/*------------------------------------------------------
Atualiza o melhor vizinho do k, ve se o vTarg passou a ser melhor vizinho do K.

-----------------------------------------------------------*/
void *updateKtoTarg2(typeGraph *graph, typeVertex *vertK, typeVertex *vTarg, typeVertex *vRem,
                     typeNeigh **bestNeigh, typeMaxList* maxList) {
    typeEdgeAVL *pVKTarg= NULL;
    double oldQual;

    //APAGAR RELEASE
    /**Assert:     vertK!= NULL*/
    pVKTarg = searchEdge( graph->vList , vertK->label , vTarg->label );
    if(pVKTarg!=NULL) {
        //caso k fosse adj vRem, oldQual na verdade era at
        oldQual = pVKTarg->quality;

        pVKTarg->quality = modularityIncreaseCluster(graph, vertK->label , vTarg->label);
        bestNeigh[vertK->label -1] = wakitaCases(bestNeigh,vertK, pVKTarg , oldQual, vRem->label, maxList);
        //wakita
        //unico valor da modularidade  que muda � entre vertK --- vTarg
        //precisa recalcular
    }

    return NULL;
}

/*---------------------------------------------------------

Define um novo melhor vizinho para vert (vTarg) a partir do zero.
Para cada vizinho k do vert, atualiza o melhor vizinho do k.

---------------------------------------------------------*/
//f
//onde guardar valores de modularidade/qualidade
void calcTargAVLModularity(typeGraph *graph,typeNeigh **bestNeigh, typeMaxList *maxList, typeVertex *vert, typeVertex *vRem, typeEdgeAVL *neigh) {
   //APAGAR RELEASE
    /**Assert */
    if(neigh!=NULL) {

        //se for self edge
        if(neigh->vAdj == vert->label) {
            neigh->quality = modularityIncreaseCluster(graph, vert->label, neigh->vAdj);
            //11022013: n�o pode contrair v�rtice com ele mesmo, afinal.
        } else {
            neigh->quality = modularityIncreaseCluster(graph, vert->label, neigh->vAdj);
            //se o valor da sele��o do vizinho for melhor que o j� armazenado, substituo
            if(bestNeigh[vert->label-1]!=NULL &&  neigh->quality > bestNeigh[vert->label-1]->quality) {
                bestNeigh[vert->label - 1]->label2 = neigh->vAdj;
                bestNeigh[vert->label - 1]->quality = neigh->quality;
            }
       }

        if(neigh->left!= NULL) {
            calcTargAVLModularity(graph, bestNeigh, maxList, vert, vRem, neigh->left);
        }
        if(neigh->right!= NULL) {
            calcTargAVLModularity(graph, bestNeigh, maxList, vert, vRem, neigh->right);
        }
    }
}

//f
//onde guardar valores de modularidade/qualidade
void updateAllKtoTarg(typeGraph *graph,typeNeigh **bestNeigh, typeMaxList *maxList, typeVertex *vert, typeVertex *vRem, typeEdgeAVL *neigh) {
    /**Assert */
    if(neigh!=NULL) {

        //se for self edge
        if(neigh->vAdj == vert->label) {

        } else {
            updateKtoTarg2(graph, searchVertex(graph->vList ,neigh->vAdj)  , vert , vRem, bestNeigh , maxList);
        }

        if(neigh->left!= NULL) {
            updateAllKtoTarg(graph, bestNeigh, maxList, vert, vRem, neigh->left);
        }
        if(neigh->right!= NULL) {
            updateAllKtoTarg(graph, bestNeigh, maxList, vert, vRem, neigh->right);
        }
    }
}

//f
//retorna a nova raiz do v�rtice contra�do
typeEdgeAVL* changeEdge( typeGraph *graph, typeVertex *vertK, typeVertex *vTarg, typeVertex *vRem,
                         typeNeigh **bestNeigh, double pound, typeEdgeAVL *pTarg, typeMaxList* maxList) {

    typeEdgeAVL *pVKTarg=NULL;
    double oldQual;

    //remove a aresta do v�rtice rem da lista
    //remove TODAS as ocorrencias do vertice removido da �rvore

    //quando remove uma aresta, o grau do v�rtice diminui. Muito importrante
    vertK->adjList = removeArvore(vertK->adjList , vRem->label );

    //adaptar para grafos ponderados, fazer vDegree ser double
    //para otimizar, nao mexe em NADA dos graus desses vertices

    if(vertK->adjList != NULL) {
        vertK->adjList->parent = NULL;
    }

    //procura pelo target na lista do v�rtice
    pVKTarg = searchEdge( graph->vList , vertK->label , vTarg->label );
    if(vTarg->adjList!=NULL) {
        vTarg->adjList->parent = NULL;
    }
    //se j� for adj a targ s� incrementa o peso dele
    if(pVKTarg!=NULL) {
        //vertice k � adjacente  a i e j
        if(vertK->label != pVKTarg->vAdj) {
            //conect. a ambas
            oldQual = pVKTarg->quality;

            //calcula o novo valor da modularidade para essa aresta
            pVKTarg->quality = modularityIncreaseCluster(graph, vertK->label , vTarg->label); //22/10/2012

        }
    }

    /// atualiza��o do wakita
    //o melhor v�rtice de K � escolhido por essa fun��o
    bestNeigh[vertK->label - 1] = wakitaCases(bestNeigh, vertK  ,  pVKTarg, oldQual , vRem->label, maxList);
    return NULL;
}


//f
//FALTA HEADER
//retorna a nova raiz do v�rtice contra�do
typeEdgeAVL* changeEdgeArrow( typeGraph *graph, typeVertex *vertK, typeVertex *vTarg, typeVertex *vRem,
                              typeNeigh **bestNeigh, double pound, typeEdgeAVL *pTarg, typeMaxList* maxList) {

    typeEdgeAVL *pVKTarg=NULL;
    double oldQual;

    //remove TODAS as ocorrencias do vertice removido da �rvore
    //quando remove uma aresta, o grau do v�rtice diminui. Muito importrante
    vertK->adjArrow = removeArvore(vertK->adjArrow , vRem->label );

    //para otimizar, nao mexe em NADA dos graus desses vertices
    //graph->vDegree[vertK->label-1]-=pound;//; pound; //27/10/2012

    if(vertK->adjArrow != NULL) {
        vertK->adjArrow->parent = NULL;
    }

    //procura pelo target na lista do v�rtice
    if(vTarg->adjArrow!=NULL) {
        vTarg->adjArrow->parent = NULL;
    }

    return NULL;
}

//f
/*-----------------------------------------------
Passa os v�rtices que eram adjacentes a vRem,  K ,  para o vTarg.
Tanto a lista de vTarg, quanto a lista dos K s�o atualizadas
vRem -> k
-----------------------------------------------*/
//implementa��o sem uso de pilhas!
void updateAdjs(typeGraph *graph, typeVertex *vTarg, typeVertex *vRem, typeNeigh **bestNeigh, typeMaxList *maxList, typeElemDendr *merge) {
    typeEdgeAVL *q = NULL;
    typeEdgeAVL *pTarg = vTarg->adjList;
    typeEdgeAVL *pRem = vRem->adjList;
    //typeEdgeAVL *selfRem = NULL;
    double pesoSelfRem=0;
    typeVertex *vK = NULL;
    int d=0;

    //pRem � referente a uma aresta a partir de vRem
    //pTarg � a aresta a partir de vTarg, com valor pRem->vAdj;

    if(pRem!=NULL ) {
        pRem->parent = NULL;
    }

    while(pRem != NULL) {
        //visita o v�rtice
        if(d == 0) {
            //tentar ir para a subArv esquerda
            if(pRem->left != NULL) {
                pRem->left->parent = pRem;
                pRem = pRem->left;
            }
            //se nao conseguir tenta a direita

            else d = 1;
        }

        //n�o posso adicionar uma self edge do v�rtice a ser removido

        else if(d == 1) {
           //pRem->vAdj = arestaK deve ser diferente do vertice removido:
            //ele j� foi removido da lista de adjac�ncias

            ///peso do la�o: representado por pRem->vAdj == vTarg->label

            //n�o adiciona o la�o do v�rtice removido
            if(pRem->vAdj != vRem->label) {
                vK = searchVertex(graph->vList , pRem->vAdj);

                //vTarg -> pRem
                //chega nesse caso quando lista de adj � Vazia, insere na raiz!
                if(pTarg==NULL) {
                    vTarg->adjList = createEdgeAVL(pRem->vAdj, pRem->pound);
                    pTarg = vTarg->adjList;
                } else {
                    pTarg = insertCoarsed(pTarg, pRem);
                    //precisa percorrer at� a raiz para balancear e atualizar altura
                    vTarg->adjList = balancearAteRaiz(pTarg);
                    vTarg->adjList->parent = NULL;

                }

                ///Verifica��o para alterar a lista de adjacencias do vertK , adj a vRem.
                //atualiza adjacencias dos v�rtices adjs ao removido
                //pRem->vAdj = arestaK deve ser diferente do alvo: n�o quero entrar no pr�prio la�o

                //REF RELEASE
                //270218: REF
                ///Adiciona  pRem->vAdj (adj a vRem -> pRem->vAdj) na lista de arestas modificadas
                merge->changeEdgeList = insereKChanged(merge->changeEdgeList , pRem->vAdj , pRem->pound);

    
                if(pRem->vAdj != vTarg->label  ) {
                    //pRem <- vTarg
                    includeEdgeSimplesArrow(graph, vK , vTarg, pRem->pound);
                    //entra na lista do vertK ( adjacente ao removido)
                    //remove vertK->vRem atualiza (vertK, vTarg)
                    changeEdge(graph, vK , vTarg , vRem, bestNeigh, pRem->pound, pTarg , maxList);
                    //remove vertK <- vRem
                    changeEdgeArrow(graph, vK , vTarg , vRem, bestNeigh, pRem->pound, pTarg , maxList);
                }
            }
            ///29/10/2012
            //pRem->vAdj = vRem->label: vRem tem um la�o
            else { //29/10/2012
                //selfRem = pRem;
                //guarda o peso do la�o do v�rtice removido, para inserir no la�o do targ no fim da fun��o
                pesoSelfRem = pRem->pound;

                //270218: REF
                //191118: salva o la�o
                merge->changeEdgeList = insereKChanged(merge->changeEdgeList , pRem->vAdj , pRem->pound);
            }

            if(pRem->right != NULL) {
                pRem->right->parent = pRem;
                pRem= pRem->right;
                d = 0;
            } else d = 2;
        } else {
            q =pRem;
            pRem =pRem->parent;
            if(pRem != NULL) {
                if(pRem->left == q)
                    d = 1;
            }
        }
    }

    //27/10/2012: adiciona o peso do la�o do vRem no la�o do targ
    pTarg = searchEdge(graph->vList , vTarg->label , vTarg->label);

    //APAGAR RELEASE CONF
    ///DIRECTED GRAPH - ERROR HERE - PROVAVELMENTE pTarg = NULL
//    printf("pTarg = %p\n",pTarg);
    if(pTarg!=NULL) {
        pTarg->pound += pesoSelfRem;
    } else{

        includeEdgeSimples(graph, vTarg , vTarg, pesoSelfRem);
    }

}

//f
/*-----------------------------------------------
Passa vTarg para os v�rtices que tinham vRem como adjacentes
arestas (k -> vRem)
adjArrow
-----------------------------------------------*/
//implementa��o sem uso de pilhas!
void updateAdjsArrow(typeGraph *graph, typeVertex *vTarg, typeVertex *vRem, typeNeigh **bestNeigh, typeMaxList *maxList, typeElemDendr *merge) {
    typeEdgeAVL *q = NULL;
    typeEdgeAVL *pTarg = vTarg->adjArrow;
    typeEdgeAVL *pRem = vRem->adjArrow;
    double pesoSelfRem=0;
    typeVertex *vK = NULL;
    int d=0;

    //pRem � referente a uma aresta a partir de vRem
    //pTarg � a aresta a partir de vTarg, com valor pRem->vAdj;

    if(pRem!=NULL ) {
        pRem->parent = NULL;
    }

    while(pRem != NULL) {
        //visita o v�rtice
        if(d == 0) {

            //tentar ir para a subArv esquerda
            if(pRem->left != NULL) {
                pRem->left->parent = pRem;
                pRem = pRem->left;
            }
            //se nao conseguir tenta a direita
            else d = 1;
        }

        //n�o posso adicionar uma self edge do v�rtice a ser removido

        else if(d == 1) {
            //pRem->vAdj = arestaK deve ser diferente do vertice removido:
            //ele j� foi removido da lista de adjac�ncias

            ///peso do la�o: representado por pRem->vAdj == vTarg->label
            //vRem <- pRem

            //APAGAR RELEASE
            ///TODO: s� insere pRem em vTarg->adjArrow se pRem->vAdj != vTarg
            ///se for igual, insere/incrementa peso de vTarg->vTarg
            //n�o adiciona o la�o do v�rtice removido
            if(pRem->vAdj != vRem->label) {

                    vK = searchVertex(graph->vList , pRem->vAdj);

                    //chega nesse caso quando lista de adj � Vazia, insere na raiz!
                    if(pTarg==NULL) {
                        vTarg->adjArrow = createEdgeAVL(pRem->vAdj, pRem->pound);
                        pTarg = vTarg->adjArrow;
                    } else {
                        //DIRECTED GRAPH //pTarg <- k
                        pTarg  = insertCoarsed( pTarg , pRem );
                        //precisa percorrer at� a raiz para balancear e atualizar altura
                        vTarg->adjArrow = balancearAteRaiz(pTarg);
                        vTarg->adjArrow->parent = NULL;
                    }

                    ///Verifica��o para alterar a lista de adjacencias do vertK , adj a vRem.
                    //atualiza adjacencias dos v�rtices adjs ao removido
                    //pRem->vAdj = arestaK deve ser diferente do alvo: n�o quero entrar no pr�prio la�o

                    //191118
                    ///Adiciona  pRem->vAdj (adjArrow pRem->vAdj -> vRem) na lista de arestas modificadas
                    merge->changeEdgeArrow = insereKChanged(merge->changeEdgeArrow, pRem->vAdj , pRem->pound);
                   
                    if(pRem->vAdj != vTarg->label  ) {
                        includeEdgeSimples(graph, vK , vTarg, pRem->pound);
                        //entra na lista do vertK ( adjacente ao removido)
                        //remove vertK->vRem atualiza (vertK, vTarg)
                        changeEdge(graph, vK , vTarg , vRem, bestNeigh, pRem->pound, pTarg , maxList);
                        //remove vertK <- vRem
                        changeEdgeArrow(graph, vK , vTarg , vRem, bestNeigh, pRem->pound, pTarg , maxList);
                    }
            }
            ///29/10/2012
            //pRem->vAdj = vRem->label: vRem tem um la�o
            else { //29/10/2012
                //selfRem = pRem;
                //guarda o peso do la�o do v�rtice removido, para inserir no la�o do targ no fim da fun��o
                pesoSelfRem += pRem->pound;

                //191118
                merge->changeEdgeArrow = insereKChanged(merge->changeEdgeArrow , pRem->vAdj , pRem->pound);
            }

            if(pRem->right != NULL) {
                pRem->right->parent = pRem;
                pRem= pRem->right;
                d = 0;
            } else d = 2;
        } else {
            q =pRem;
            pRem =pRem->parent;
            if(pRem != NULL) {
                if(pRem->left == q)
                    d = 1;
            }
        }
    }

    //27/10/2012: adiciona o peso do la�o do vRem no la�o do targ
    pTarg = searchEdge(graph->vList , vTarg->label , vTarg->label);

    if(pTarg!=NULL) {
        pTarg->pound += pesoSelfRem;
    }
    else{
        ///TODO: cria aresta vTarg -> vTarg com peso pesoSelfRem
        includeEdgeSimples(graph, vTarg , vTarg, pesoSelfRem);
    }
}

//superno � um ponteiro para adjacencias do superno
typeEdgeAVL* insertCoarsed(typeEdgeAVL *pTarg, typeEdgeAVL *pRem) {
    int rt = 0 , et = 0;

    //pai da raiz n�o existe

    //pTarg->parent = NULL;
    //percorrer a arvore do n� removido, com pRem, analisando correspond�ncias na �rvore do n� contra�do , com pTarg
    while(pRem!=NULL) {
        //o que eu quero remover � menor que o alvo
        if(pRem->vAdj < pTarg->vAdj) {
            ///Parar procurar na subArv da esquerda.
            // se o target � raiz a �nica possib.
            //se o target tem pai, ent�o o pai deve ser maior que o no a ser removido

            if(pTarg->parent == NULL)et =1;

            if( et ==  1  || pRem->vAdj > pTarg->parent->vAdj  ) {
                et = 1;
                //se existir arvEsq , continua nela
                if(pTarg->left != NULL) {
                    //faz atualiza��es de parent pelo caminho
                    pTarg->left->parent = pTarg;
                    pTarg = pTarg->left;
        
                } else {
                    //se a posi��o est� vazia, achou onde inserir

                    pTarg->left = createEdgeAVL(pRem->vAdj, pRem->pound);

                    pTarg->left->parent = pTarg;
                    return pTarg->left;

                }
            }
            //tem que voltar para o pai e procurar a partir dele
            else {
                //     if(pTarg->parent!=NULL ) //pelas condi��es acima essa condi��o � garantida
                pTarg = pTarg->parent;

            }
        }
        //na sub arv. direita
        else if(pRem->vAdj > pTarg->vAdj) {

            //se o pai � maior que o superno atual, ent�o o n� � procurado na subArv. direita

            //se o pai n�o existe � a unica possib.
            //se o target est� na subArv da esquerda, ent�o o rem precisa ser menor que target->parent
            //se o target esta na subArv da dir, ent�o ACHO QUE ESSE CASO NAO EXISTE

            if(pTarg->parent == NULL)rt =1;

            if( rt ==  1 || pRem->vAdj < pTarg->parent->vAdj  ) {
                rt = 1;
                //continua procurando na direita
                if(pTarg->right != NULL) {

                    pTarg->right->parent = pTarg;
                    pTarg = pTarg->right;

                } else {
                    pTarg->right = createEdgeAVL(pRem->vAdj, pRem->pound);

                    pTarg->right->parent = pTarg;
                    return pTarg->right;
                }
            } else { //precisa procurar a partir do pai
                pTarg = pTarg->parent;

            }
        } else { //s�o iguais, elemento j� existe no contra�do

            pTarg->pound += pRem->pound; //so incrementa peso
            return pTarg;
            //proximo n�
        }
    }
    return NULL;
}

///FIM ATUALIZAR GRAFO--------------------------------------------------------------------------------------------

