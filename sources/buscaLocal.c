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

#include "buscaLocal.h"


//f
typeMove *calcMelhorMoveVert(typeGraph *grafo,  typeParticao *particao, typeMove **melhorMove, typeVertex *vert)
{  

    typeCluster *clusterVert = particao->mapa[vert->label -1];
    typeCluster *clusterAux = particao->clustering->prim;

    double ganho;
   
    melhorMove[vert->label-1]->cluster = NULL;
    melhorMove[vert->label-1]->quality = MENORQ;

    while(clusterAux!=NULL)
    {
        if(clusterAux != clusterVert && clusterAux->qtd!= 0)
        {
            ganho = calcGanhoModularidadeMove(grafo, particao->clustering, vert->label , clusterVert, clusterAux);         
            if(ganho > melhorMove[vert->label-1]->quality)
            {
                //melhor move pode guardar ponteiro para o cluster também!!!

                melhorMove[vert->label-1]->cluster = clusterAux;
                melhorMove[vert->label-1]->quality = ganho;
            }
        }

        clusterAux = clusterAux->prox;
    }

    ///criar um novo cluster e mover vert para ele
    //somente se o C(vert) já não for isolado: se |clusterVert| > 1. Como é sempre >= 0 então:
    if(clusterVert->qtd != 1)
    {

        ganho = calcTermoGanhoClusterIsolado(grafo, particao, vert->label);
      ///D 21118   printf("ganho=%lf\n",ganho);

        if(ganho > melhorMove[vert->label-1]->quality)
        {
          ///D 21118   printf(".......>> MOVE TERMO ISOLADO\n");
            //melhor move pode guardar ponteiro para o cluster também!!!

            ///cria um cluster com indice 0?
            //pode criar um único cluster com índice 0 e só fazer apontar pro cara: eco
            //ou pode modificar o índice do cluster e usar ele mesmo
            melhorMove[vert->label-1]->cluster = criaCluster(0);
            melhorMove[vert->label-1]->quality = ganho;
        }
    }

    return melhorMove[vert->label-1];

}

//f
/*
    vert->contribuicao deve estar atualizado
    //vertMovido é a origem da aresta e clusterDest é o destino da aresta
*/
double calcGanhoModularidadeMove(typeGraph *grafo, typeClustering *clustering, int vertMovido , typeCluster* clusterOrig, typeCluster* clusterDest)
{ 
    double deltaQ = 0;
     deltaQ =  modularidadeClusterMove(grafo, clustering, clusterDest, vertMovido) - searchVertex(grafo->vList , vertMovido)->contribuicao;

    if(clusterOrig==clusterDest && deltaQ!=0)
    {
        printf("erro clusters iguais!\n");
    }
    return deltaQ;
}


//funções comuns entre as buscas locais

//f
double modularidadeClusterMove(typeGraph *grafo, typeClustering *clustering, typeCluster* cluster, int vertMovido)
{
    double deltaQ = 0; 

    //RELEASE APAGAR
    /**Assert: cluster!=NULL*/
    typeElemCluster *elem = cluster->prim;

    while(elem!=NULL)
    {        
        ////211118
        ///Na subtração, esse termo vai desaparecer, então não tem sentido calcular
        if(   searchVertex( grafo->vList , elem->vert )!=NULL && elem->vert != vertMovido)
        {     
            ///usa modularityIncrease?
            deltaQ += ( searchEdgePound(grafo, vertMovido , elem->vert)/( (long double)grafo->m) ) -
                        grafo->LAMBDA_ML * ( (grafo->vDegree[vertMovido-1] * grafo->vDegreeArrow[elem->vert-1] )/( ((long double)grafo->m)*(long double)grafo->m) );

            deltaQ += ( searchEdgePound(grafo,elem->vert, vertMovido)/( (long double)grafo->m) ) -
                        grafo->LAMBDA_ML * ( (grafo->vDegree[elem->vert-1] * grafo->vDegreeArrow[vertMovido-1] )/( ((long double)grafo->m)*(long double)grafo->m) );

        }
        elem = elem->prox;
    }
    return deltaQ;
}

//f
double calcTermoQ(typeGraph *grafo, int label1, int cluster)
{
    double q1=0, q2=0, q=0;
    
    q1 = (searchEdgePound(grafo, label1 , cluster)/((long double)grafo->m))  -  grafo->LAMBDA_ML *( ( grafo->vDegree[label1-1] * grafo->vDegreeArrow[cluster-1])/( (double)grafo->m * (double)grafo->m  ));
    q2 = (searchEdgePound(grafo,cluster, label1)/((long double)grafo->m))  -  grafo->LAMBDA_ML *( ( grafo->vDegree[cluster-1] * grafo->vDegreeArrow[label1-1])/( (double)grafo->m * (double)grafo->m  ));
    q=q1+q2;

    return q;
}

//f
double calcContribuicao( typeGraph *grafo, typeVertex *vert, typeParticao *particao)
{
    //particao->mapa[vert->label]: cluster do vert
    typeCluster *cluster = particao->mapa[vert->label-1];
    typeElemCluster *elem = cluster->prim;

    vert->contribuicao = 0;

    while(elem!=NULL)
    {
        if(searchVertex(grafo->vList,elem->vert)!= NULL && elem->vert != vert->label)
        {
            //printf("local search elem-vert=%p\n",searchVertex(grafo->vList,elem->vert));
            ///D 21118 printf("..... v=%d, v=%d\n",vert->label,elem->vert);
            vert->contribuicao+= calcTermoQ(grafo, vert->label , elem->vert);
        }

        elem=elem->prox;
    }
    return vert->contribuicao;
}

//f
double calcContribuicaoDest( typeGraph *grafo, typeVertex *vert_move, typeVertex* vert_dest, typeParticao *particao)
{
    //particao->mapa[vert->label]: cluster do vert
    typeCluster *cluster = particao->mapa[vert_dest->label-1];
    typeElemCluster *elem = cluster->prim;

    double contribuicao = 0;

    while(elem!=NULL)
    {
        if(elem->vert != vert_move->label)
        {
        //    printf("local search elem-vert=%p\n",searchVertex(grafo->vList,elem->vert));
            contribuicao+= calcTermoQ(grafo, vert_move->label , elem->vert);
        }

        elem=elem->prox;
    }

    return contribuicao;
}

//f
double calcTermoGanhoClusterIsolado(typeGraph *grafo, typeParticao *particao, int i)
{
    double deltaQ=0, deltaQ0= 0;
    typeVertex *vert = searchVertex(grafo->vList , i);

    ///assert(vert!=NULL);
    //vai para um cluster isolado se a perda estiver muito grande para ficar onde está
    if( verifClusterIsolado(particao->mapa[i-1]) == 0)
    {
        //precisa subtrair a perda!
        //- contribuição do vértice!
        deltaQ = (double)(-1) * vert->contribuicao;

    }
    else
    {
        deltaQ = MENORQ;
    }

    return deltaQ;
}


int * shuffle(int *vet, int n) {
    int i , j , aux;

    for(i=0 ; i< (n) ; i++) {

        srand(clock());
        j = (rand()%n);

        if(j!=i) {
            aux = vet[i];
            vet[i]  = vet[j];
            vet[j] = aux;
        }
    }
    return vet;
}
