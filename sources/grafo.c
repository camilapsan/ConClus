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

#include "grafo.h"

//f
int calcMaxNiveis(typeGraph *grafo, double fatReducao) {
    int nv;
    double base;

    base = 1.0 / (1.0 - fatReducao ) ; //110113

    //num de niveis é log |V|, na base 1/fatReducao
    //converte para base 10

    nv = (int) (log10(grafo->nVert)/log10(base));
    nv += 1;

    return nv;
}

//f
//DIRECTED GRAPH - 050115
double calculaModularidadeGrafo(typeGraph *grafo, typeParticao *particao) {
    int i,j;
    long double qTotal = 0;
    ///funcao tem que considerar os vértices dentro tbm...

//da pra otimizar, já que o grafo é não direcionado, mas a função é só para testes
    for(i=1; i <= grafo->nOrig ; i++) {
        if(searchVertex(grafo->vList , i )!=NULL ) {
            for(j=1; j<= grafo->nOrig ; j++) {
                if(searchVertex(grafo->vList , j )!=NULL) {
                    //calcula modularidade do termo q(i,j)
                    ///SE ESTAO NA MESMA PARTICAO
                    if( particao->mapa[i-1] == particao->mapa[j-1] ) {
                        qTotal += searchEdgePound(grafo, i , j) -  grafo->LAMBDA_ML * ( grafo->vDegree[i-1] * grafo->vDegreeArrow[j-1])/
                                  ( (long double)grafo->m);
                    }
                }

            }
        }
    }

    qTotal = qTotal / ( 
 (long double)grafo->m);

    return qTotal;
}

//f
//DIRECTED GRAPH - 050115
double calculaModularidadeGrafoInt(typeGraph *grafo, int *mapa) {
    int i,j;
    long double qTotal = 0;
    ///funcao tem que considerar os vértices dentro tbm...

//da pra otimizar, já que o grafo é não direcionado, mas a função é só para testes
    for(i=1; i <= grafo->nOrig ; i++) {
        if(searchVertex(grafo->vList , i )!=NULL ) {
            for(j=1; j<= grafo->nOrig ; j++) {
                if(searchVertex(grafo->vList , j )!=NULL) {
                    //calcula modularidade do termo q(i,j) se estão na mesma comunidade
                    if( mapa[i-1] == mapa[j-1] ) {
                        qTotal += searchEdgePound(grafo, i , j) -  grafo->LAMBDA_ML * ( grafo->vDegree[i-1] * grafo->vDegreeArrow[j-1])/
                                  ( (long double)grafo->m);
                    }
                }

            }
        }
    }

    qTotal = qTotal / ( (long double)grafo->m);

    return qTotal;
}

//DIRECTED GRAPH - 050115
double calculaModularidadeGrafoS(typeGraph *grafo, typeParticao *particao) {
    int i,j;
    long double qTotal = 0;
    ///funcao tem que considerar os vértices dentro tbm...

//da pra otimizar, já que o grafo é não direcionado, mas a função é só para testes
    for(i=1; i <= grafo->nOrig ; i++) {
        if(searchVertex(grafo->vList , i )!=NULL ) {
            for(j=1; j<= grafo->nOrig ; j++) {
                if(searchVertex(grafo->vList , j )!=NULL) {
                    //calcula modularidade do termo q(i,j)
                    ///SE ESTAO NA MESMA PARTICAO
                    if( particao->mapa[i-1] == particao->mapa[j-1] ) {
                        qTotal += searchEdgePound(grafo, i , j) -  ( grafo->vDegree[i-1] * grafo->vDegreeArrow[j-1])/
                                  ( (long double)grafo->m);
                    }
                }

            }
        }
    }

    qTotal = qTotal / (  (long double)grafo->m);

    return qTotal;
}

void printGraph(typeGraph *graph) {
    int i;
    typeVertex *vert;

    printf("GRAFO\n");
    printf("n = %d \nm = %d\nQ= %lf\n", graph->nVert , graph->m, graph->modularity);
    for(i=1; i<=graph->nOrig ; i++) {
        vert = searchVertex(graph->vList,i);

        //uma lista de vértices ativos facilitaria bastante
        if(vert!=NULL) {
            printf("%3d [%.0lf][in:%.0lf] .:\t", vert->label, graph->vDegree[vert->label-1] , graph->vDegreeIn[vert->label-1]);
            if(vert->adjList!=NULL) {
                printAdjList(vert->adjList);
            }

            printf("\n");
        }
    }
}

void printGraphArrow(typeGraph *graph) {
    int i;
    typeVertex *vert;

    printf("GRAFO ARROW\n");
    printf("n = %d \nm = %d\nQ= %lf\n", graph->nVert , graph->m, graph->modularity);
    for(i=1; i<=graph->nOrig ; i++) {
        vert = searchVertex(graph->vList,i);

        if(vert!=NULL) {
            printf("%3d [%.0lf][in:%.0lf] .:\t", vert->label, graph->vDegreeArrow[vert->label-1] , graph->vDegreeIn[vert->label-1]);
            if(vert->adjArrow!=NULL) { //201118
                printAdjList(vert->adjArrow);
            }

            printf("\n");
        }
    }
    printf("end..arrow\n");

}

void printAdjList(typeEdgeAVL *root) {
    if(root!=NULL) {
        printf("%2d (%d)  ",root->vAdj , (int)root->pound);

        if(root->left!=NULL) {
            printAdjList(root->left);
        }

        if(root->right!=NULL) {
            printAdjList(root->right);
        }
    }
}

void printGraphFile(typeGraph *graph, FILE *file) {
    int i;
    typeVertex *vert;

    fprintf(file, "GRAFO\n");
    fprintf(file, "n = %d \nm = %d\nQ= %lf\n", graph->nVert , graph->m, graph->modularity);
    for(i=1; i<=graph->nOrig ; i++) {
        vert = searchVertex(graph->vList,i);

        if(vert!=NULL) {
            fprintf(file, "\n%3d [%.0lf][in:%.0lf]  .:\t", vert->label, graph->vDegree[vert->label-1], graph->vDegreeIn[vert->label-1]);
            if(vert->adjList!=NULL) {
                printAdjListFile(vert->adjList,file);
            }

            fprintf(file,"\n");
        }
    }
}

void printAdjListFile(typeEdgeAVL *root, FILE *file) {

    if(root!=NULL) {
        fprintf(file, "%2d (%d)  ",root->vAdj , (int)root->pound);

        if(root->left!=NULL) {
            printAdjListFile(root->left,file);
        }

        if(root->right!=NULL) {
            printAdjListFile(root->right,file);
        }
    }
}

typeGraph *createGraph(int size) {
    typeGraph *grafo = (typeGraph*) malloc(sizeof(typeGraph));
    grafo->nVert = size;
    grafo->nOrig = size;
    grafo->nDesconex = 0; //200113

    grafo->vList = createVertexList(size);
    grafo->vDegree = (double*) calloc(sizeof(double),size);
    grafo->vDegreeIn = (double*) calloc(sizeof(double),size);//19102012
    grafo->vDegreeArrow = (double*) calloc(sizeof(double),size);//DIRECTED GRAPH 31122015
    grafo->m = 0;
    grafo->modularity = 0;

    return grafo;
}

typeEdgeAVL* copyAVL(typeEdgeAVL *source, typeEdgeAVL *dest) {

    dest = createEdgeAVL(source->vAdj, source->pound);
    dest->altura = source->altura;
    dest->quality = source->quality;

    if(source->left != NULL) {
        dest->left = copyAVL(source->left , dest->left);

    }
    if(source->right != NULL) {
        dest->right =  copyAVL(source->right , dest->right);
    }

    return dest;
}

//f
typeGraph *copyGraph(typeGraph *source) {
    typeGraph *graph = (typeGraph*) malloc(sizeof(typeGraph));
    int i;

    graph->m = source->m;
    graph->modularity = source->modularity;
    graph->nOrig = source->nOrig;
    graph->nVert = source->nVert;
    graph->nDesconex = source->nDesconex;

    graph->vDegree = (double*) calloc(sizeof(double),source->nOrig);
    graph->vDegreeIn = (double*) calloc(sizeof(double), source->nOrig);
    //DIRECTED GRAPH - 050115
    graph->vDegreeArrow = (double*) calloc(sizeof(double), source->nOrig);

    //220218
    graph->LAMBDA_ML = source->LAMBDA_ML;

    graph->vList = (typeVertex**) malloc(sizeof(typeVertex*)  * (graph->nOrig)  );
    //incluir as arestas
    for(i=0; i<graph->nOrig ; i++) {
        graph->vDegree[i] = source->vDegree[i];
        graph->vDegreeIn[i] = source->vDegreeIn[i];
        //DIRECTED GRAPH - 050115
        graph->vDegreeArrow[i] = source->vDegreeArrow[i];

        if(source->vList[i]!=NULL) {
            graph->vList[i] = createVertex(i+1);
            //pre-processamento de vértices isolados feito

            //APAGAR RELEASE
            /**Assert*/
////            assert(source->vList[i]->adjList != NULL);
            if (source->vList[i]->adjList != NULL) {
                graph->vList[i]->contribuicao = source->vList[i]->contribuicao;  //030113
                graph->vList[i]->adjList = copyAVL(source->vList[i]->adjList, graph->vList[i]->adjList);
            }

            //DIRECTED GRAPH 020114 - copy adjArrow
            if (source->vList[i]->adjArrow != NULL) {
                graph->vList[i]->adjArrow = copyAVL(source->vList[i]->adjArrow, graph->vList[i]->adjArrow);
            }

        } else {
            graph->vList[i] = NULL;
        }

        //DIRECTED GRAPH 020114 - copy adjArrow
    }
    return graph;
}

typeGraph *copyGraphDest(typeGraph *source, typeGraph *graph) {
    int i;

    graph->m = source->m;
    graph->modularity = source->modularity;
    graph->nOrig = source->nOrig;
    graph->nVert = source->nVert;

    graph->nDesconex = source->nDesconex; //200113

    //incluir as arestas
    for(i=0; i<graph->nOrig ; i++) {
        graph->vDegree[i] = source->vDegree[i];
        graph->vDegreeIn[i] = source->vDegreeIn[i];
        //   printf("\n%d-------adj = %p --\n",i, source->vList[i]->adjList);
        if(source->vList[i]!=NULL) {
            if(graph->vList[i] == NULL) {
                graph->vList[i] = createVertex(i+1);
            }
            //pre-processamento de vértices isolados feito
            /**Assert*/
            assert(source->vList[i]->adjList != NULL);

            graph->vList[i]->contribuicao = source->vList[i]->contribuicao;  //030113

            if(graph->vList[i]->adjList != NULL) {
                graph->vList[i]->adjList = removeAdjList(graph->vList[i]->adjList , graph);
            }
            graph->vList[i]->adjList = copyAVL(source->vList[i]->adjList, graph->vList[i]->adjList);

            //DIRECTED GRAPH 020114 - copy adjArrow
            if(graph->vList[i]->adjArrow != NULL) {
                graph->vList[i]->adjArrow = removeAdjList(graph->vList[i]->adjArrow , graph);
            }
            graph->vList[i]->adjArrow = copyAVL(source->vList[i]->adjArrow, graph->vList[i]->adjArrow);


        } else {
            graph->vList[i] = NULL;
        }

    }

    return graph;
}

typeEdgeAVL* copyAVLModularity(typeEdgeAVL *source, typeEdgeAVL *dest) {
    assert(dest!=NULL && source!=NULL);

    dest->quality = source->quality;

    if(source->left != NULL) {
        dest->left = copyAVLModularity(source->left , dest->left);

    }
    if(source->right != NULL) {
        dest->right =  copyAVLModularity(source->right , dest->right);
    }

    return dest;
}


typeGraph *copyGraphModularity(typeGraph *fonte, typeGraph *destino) {
    int i;

    destino->modularity = fonte->modularity;

    //incluir as arestas
    for(i=0; i<destino->nOrig ; i++) {
        //   printf("\n%d-------adj = %p --\n",i, source->vList[i]->adjList);
        ///aqui a lista de vértices existentes também seria útil
        if(fonte->vList[i]!=NULL) {
            //pre-processamento de vértices isolados feito

            //APAGAR RELEASE
            /**Assert*/
            //assert(fonte->vList[i]->adjList != NULL  && destino->vList[i]->adjList!=NULL );

            destino->vList[i]->contribuicao = fonte->vList[i]->contribuicao; //030113
            destino->vList[i]->adjList = copyAVLModularity(fonte->vList[i]->adjList, destino->vList[i]->adjList);
        }
    }

    return destino;
}

//APAGAR RELEASE
///otimizar
typeGraph *resetaGrafoNovaIteracao(typeGraph *source, typeGraph *graph, typeDendr *dendr) {
    int i , vert;

    graph->m = source->m;
    graph->modularity = source->modularity;
    graph->nOrig = source->nOrig;

    ///a partir daqui pode
    //incluir as arestas
    for(i=0; i<graph->nOrig ; i++) {

        graph->vDegree[i] = source->vDegree[i];
        graph->vDegreeIn[i] = source->vDegreeIn[i];

        if(source->vList[i]!=NULL) {
            if(graph->vList[i] == NULL) {
                graph->vList[i] = createVertex(i+1);
            }
            //pre-processamento de vértices isolados feito
            assert(source->vList[i]->adjList != NULL);

            graph->vList[i]->contribuicao = source->vList[i]->contribuicao;  //030113

            if(graph->vList[i]->adjList != NULL) {
            ///150513:não é necessário copiar todas as adjs. uma vez que graph já foi expandido para o source
                graph->vList[i]->adjList = removeAdjList(graph->vList[i]->adjList , graph);
            }
            ///só precisa atualizar a AVL de vértices que foram alvos de pré contração - não
            graph->vList[i]->adjList = copyAVL(source->vList[i]->adjList, graph->vList[i]->adjList);

        } else {
            if(graph->vList[i]!=NULL) {
                removeVertex(graph, i+1);
            }
            graph->vList[i] = NULL;
        }

    }
    return graph;
}

//f
typeGraph *destroiGrafo(typeGraph *grafo) {
    int i;
    if(grafo!=NULL) {
        for(i=1 ; i <= grafo->nOrig; i++) {
            grafo = removeVertex(grafo, i);
        }
        free(grafo->vDegree);
        grafo->vDegree = NULL;

        free(grafo->vDegreeIn);
        grafo->vDegreeIn = NULL;

        free(grafo->vList);
        grafo->vList = NULL;

        free(grafo);
        grafo = NULL;

        return grafo;
    }
}

void printGraphDegree(typeGraph *graph) {
    int i;
    printf("Graus dos vértices\n");

    for(i=0; i<graph->nOrig; i++) {
        if(graph->vDegree[i]!= -1) {
            printf("[%d] = %.0lf\n",i+1, graph->vDegree[i] );
        }
    }
}

void printGraphDegreeIn(typeGraph *graph) {
    int i;
    printf("Graus Internos dos vértices\n");

    for(i=0; i<graph->nOrig; i++) {
        {
            printf("[%d] = %.0lf\n",i+1, graph->vDegreeIn[i] );
        }
    }
}

void printPajekAdjList(int vert, typeEdgeAVL *root, FILE *file) {
    /**Assert */
    if(root!=NULL) {
        //por ser não direcionado, só deve imprimir no arquivo 1 aresta.
        //(u,v) . tem-se que u <= v
        ///200213
        if(vert <= root->vAdj) {
            fprintf(file, "%d %d %lf\n",vert , root->vAdj , root->pound);

            if(root->left!=NULL) {
                // printf("esq ");
                printPajekAdjList(vert, root->left,file);
            }

            if(root->right!=NULL) {
                //  printf("dir ");
                printPajekAdjList(vert, root->right,file);
            }
        }
    }
}

void printPajekFile(typeGraph *graph) {
    int i;
    typeVertex *vert;
    FILE *file = fopen("karate_exML_nv.paj","w");

    fprintf(file, "*Vertices %d\n",graph->nOrig);
    fprintf(file, "*Edges\n");

    for(i=1; i<=graph->nOrig ; i++) {
        vert = searchVertex(graph->vList,i);

        if(vert!=NULL) {
            if(vert->adjList!=NULL) {
                printPajekAdjList(vert->label, vert->adjList,file);
            }
        }
    }

    fclose(file);
}