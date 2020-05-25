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

#include "memoria.h"

/*-------------------------------------------------------
Elite Set
*-------------------------------------------------------*/


//f
typeEliteSet* inicializaEliteSet(int max)
{
    int i;
    typeEliteSet *elite = (typeEliteSet*) calloc(sizeof(typeEliteSet),1);
    elite->qtd = 0;
    elite->max = max;
    elite->lista = (typeParticao**) calloc(sizeof(typeParticao*), max+1);
    elite->indMelhor = -1;



    return elite;

}


//f
void incluiEliteSet_best(typeEliteSet* elite , typeParticao *novaP, typeGraph *grafo)
{
    int i;
    int dist,
        minDist = INF,
        indMinDist;
    double melhor;

        typeParticao *novaPCopia = NULL;

    if(elite->qtd  < elite->max)
    {
        /*  Como não está cheio, pelo menos uma posição está livre
            As elite->qtd posições iniciais estão ocupadas, já que as posições vazias estão sempre nas últimas posições.
            A posição vazia é (elite->qtd-1) + 1    = elite->qtd
            */

        i = elite->qtd;

        novaPCopia = copiaParticaoAloca(novaP, grafo->nOrig );
        elite->lista[i] = novaPCopia;

        elite->qtd++;

        //indice da melhor sol.
        if(elite->indMelhor != -1){
            if(novaPCopia->mod > elite->lista[elite->indMelhor]->mod){
                elite->indMelhor = i;
            }
        }
        else { //primeira solução inserida       
            elite->indMelhor = i;
        }
    }
    else{
        //printEliteSet(elite);
        //ve qual a solução do conj. mais parecida com a ser inserida. Se a nova solução (a ser inserida) for melhor que a do conjunto, substitui essa
        for(i=0; i<elite->qtd ; i++)
        {
            //calcula a distância mínima entre as duas partições.
            //O proc. é feito encontrando a melhor correspondência entre os clusters das 2 partições e retornando a qtd de elementos que estão em clusters diferentes
            //dist = encontraDistanciaClusterings(novaP , elite->lista[i] , grafo);
            dist = elite->lista[i]->mod;

            if(dist < minDist)
            {
                minDist = dist;
                indMinDist = i;
            }
        }


        //ve se a nova solução é melhor com a mais parecida do conjunto
        if(novaP->mod > elite->lista[indMinDist]->mod )
        {
            //apaga a partição atual então

            ///010513
            melhor = elite->lista[elite->indMelhor]->mod;
            elite->lista[indMinDist] = destroiParticao(elite->lista[indMinDist]);

            //insere no lugar
            //se não copiar, pode acontecer da referência para essa partição atual ser apagada no reset da main
            novaPCopia = copiaParticaoAloca(novaP, grafo->nOrig );
            elite->lista[indMinDist] = novaPCopia;
                    //indice da melhor sol.
            if(novaPCopia->mod > melhor)
            {
                elite->indMelhor = indMinDist;
            }

        }
        else {

        }
    }
}

//f
/*---------------------------------------------------------
    Inclui uma partição no conjunto elite.
    Se ainda houverem posições disponíveis só insere.
    O vetor de partições é mantido de forma que as posições vazias estejam sempre nas últimas posições.

    Caso contrário, ve qual a solução do conj. mais parecida com a nova. Se a nova for melhor que essa do conj., substitui ela.

    *De acordo com essas condições, a melhor solução encontrada nas iterações SEMPRE vai estar dentro desse conjunto.

----------------------------------------------------------*/
void incluiEliteSet(typeEliteSet* elite , typeParticao *novaP, typeGraph *grafo)
{
    int i;
    int dist,
        minDist = INF,
        indMinDist;
    double melhor;

        typeParticao *novaPCopia = NULL;

    if(elite->qtd  < elite->max)
    {
        /*  Como não está cheio, pelo menos uma posição está livre
            As elite->qtd posições iniciais estão ocupadas, já que as posições vazias estão sempre nas últimas posições.
            A posição vazia é (elite->qtd-1) + 1    = elite->qtd
            */

        i = elite->qtd;

        novaPCopia = copiaParticaoAloca(novaP, grafo->nOrig );
        elite->lista[i] = novaPCopia;

        elite->qtd++;

        //indice da melhor sol.
        if(elite->indMelhor != -1)
        {
            if(novaPCopia->mod > elite->lista[elite->indMelhor]->mod)
            {
                elite->indMelhor = i;
            }
        }
        else //primeira solução inserida
        {
            elite->indMelhor = i;
        }

    }
    else
    {
        //printEliteSet(elite);
        //ve qual a solução do conj. mais parecida com a ser inserida. Se a nova solução (a ser inserida) for melhor que a do conjunto, substitui essa
        for(i=0; i<elite->qtd ; i++)
        {
            //calcula a distância mínima entre as duas partições.
            //O proc. é feito encontrando a melhor correspondência entre os clusters das 2 partições e retornando a qtd de elementos que estão em clusters diferentes
            dist = encontraDistanciaClusterings(novaP , elite->lista[i] , grafo);

            ///Correspondencias de elite->lista[i] não são alteradas

            limpaCorrespondencias(novaP);

            if(dist < minDist)
            {
                minDist = dist;
                indMinDist = i;
            }
        }


        //ve se a nova solução é melhor com a mais parecida do conjunto
        if(novaP->mod > elite->lista[indMinDist]->mod )
        {
            //apaga a partição atual então

            ///010513
            melhor = elite->lista[elite->indMelhor]->mod;
            elite->lista[indMinDist] = destroiParticao(elite->lista[indMinDist]);

            //insere no lugar
            //se não copiar, pode acontecer da referência para essa partição atual ser apagada no reset da main
            novaPCopia = copiaParticaoAloca(novaP, grafo->nOrig );
            elite->lista[indMinDist] = novaPCopia;
                    //indice da melhor sol.
            if(novaPCopia->mod > melhor)
            {
                elite->indMelhor = indMinDist;
            }

        }
        else
        {

        }

    }
}


//f
typeParticao *getMelhorEliteSet(typeEliteSet *elite)
{
    return elite->lista[elite->indMelhor];
}

typeParticao *escolheParticaoDistanteEliteSet(typeEliteSet *elite, typeGraph *grafo, typeParticao *part)
{
    int i, dist  , maxDist = -1.0 *INF, indMaxDist;

     //ve qual a solução do conj. mais parecida com a ser inserida. Se a nova solução (a ser inserida) for melhor que a do conjunto, substitui essa
      //ve qual a solução do conj. mais parecida com a ser inserida. Se a nova solução (a ser inserida) for melhor que a do conjunto, substitui essa
    for(i=0; i < elite->qtd ; i++)
    {
        //calcula a distância mínima entre as duas partições.
        //O proc. é feito encontrando a melhor correspondência entre os clusters das 2 partições e retornando a qtd de elementos que estão em clusters diferentes
        dist = encontraDistanciaClusterings(part , elite->lista[i] , grafo);

        ///Correspondencias de elite->lista[i] não são alteradas
        limpaCorrespondencias(part);

        if(dist > maxDist)
        {
            maxDist = dist;
            indMaxDist = i;
        }
    }

    //colocar uma distancia minima entre as soluções para fazer o PR?
    if(maxDist > 0 )
    {
        return elite->lista[indMaxDist];
    }
    else
    {
        return NULL;
    }
}

typeParticao *escolheParticaoEliteSet(typeEliteSet *eliteSet)
{
    if(eliteSet->qtd > 0)
    {
        int i = rand()%(eliteSet->qtd);
        return eliteSet->lista[i];
    }
    else
    {
        return NULL;
    }
}


typeParticao *escolheParticaoEliteSet_notChoosen(typeEliteSet *eliteSet, int* choosen, int num)
{
    int i=-1, ic=0, rep=1;

    if(eliteSet->qtd > 0 && num < eliteSet->qtd)   {
        while(rep==1){
            rep=0;
            i = rand()%(eliteSet->qtd);
            for(ic=0;ic<num;ic++){
                if( i==choosen[ic]){
                    rep=1;
                }
            }
        }
        #if PRINTALG == TRUE
            printf("*** SELECT P: %d\n", i);
        #endif

        choosen[num] = i;

        return eliteSet->lista[i];
    }
    else
    {
        return NULL;
    }

}

//f
/*---------------------------------------------------------
  Destroi o conjunto elite "recursivamente", eliminando as partições armazenadas
  Será chamada apenas na seção limpeza da main, logo antes do fim.
----------------------------------------------------------*/
typeEliteSet* destroiEliteSet(typeEliteSet *elite)
{
    int i;

    for(i=0 ; i < elite->qtd ; i++)
    {
        elite->lista[i] = destroiParticao(elite->lista[i]);
    }
    free(elite->lista);
    elite->lista = NULL;

    free(elite);
    elite = NULL;

    return elite;
}

void printEliteSet(typeEliteSet *elite)
{
    int i;
    printf("Elite Set\n");
    for(i=0; i<elite->qtd ; i++)
    {
        printf("particao %d \tQ = %lf\n", i, elite->lista[i]->mod);
    }

    if(elite->indMelhor!= -1)
    {
        printf("Melhor sol, ind = %d ,val = %lf \n", elite->indMelhor ,elite->lista[elite->indMelhor]->mod);
    }
}




//f
/*-------------------------------------------------------

    Escolhe a melhor partição do conjunto elite e outras 2 aleatórias. Os pares que tiverem na mesma comunidade nessas partições são contraídos.

    Pode escolher uma das partições como a mais distante do elite set, para garantir que o par a ser contraído realmente é recorrente!

    precisa "evaporar" essas contrações

    140313 IMPORTANTE: Essas contrações eram guardadadas no nv 1,
    as contrações do primeiro nível de contração também...
    Dessa forma, quando não expandir as contrações do nv 1, a fim
    de não expandir as contr. def., também não vai expandir o 1o
    nv do grafo...

    guarda essas contr. no nv 0 ou começa as contrações no nv 2 e
    reserva o 1 para as def.
--------------------------------------------------------*/
void contraiParesRecorrentes5EliteSet(typeEliteSet *eliteSet, typeGraph *grafo, typeNeigh **melhorVizinho,  typeDendr *dendr, int *lastMergeNv) {
    //melhor partição
    typeParticao *partM = getMelhorEliteSet(eliteSet);

    //without repeating..
    int* choosen=(int*) calloc(sizeof(int),5);

    choosen[0] = eliteSet->indMelhor;
    choosen[1] = -1;
    choosen[2] = -1;
    choosen[3] = -1;
    choosen[4] = -1;

        #if PRINTALG == TRUE
            printf("*** SELECT P: %d\n", eliteSet->indMelhor);
        #endif

    typeParticao *part2 = escolheParticaoEliteSet_notChoosen(eliteSet,choosen,1);
    typeParticao *part1 = escolheParticaoEliteSet_notChoosen(eliteSet,choosen,2);
    typeParticao *part3 = escolheParticaoEliteSet_notChoosen(eliteSet,choosen,3);
    typeParticao *part4 = escolheParticaoEliteSet_notChoosen(eliteSet,choosen,4);
    
    #if PRINTALG == TRUE
        int ind;
        printf("*** CONTR DEF\n");
        for(ind=0; ind<grafo->nOrig; ind++){
            printf("%d, ",partM->mapa[ind]->indice);            
        }
        printf("\n");
        for(ind=0; ind<grafo->nOrig; ind++){
            printf("%d, ",part1->mapa[ind]->indice);
        }
        printf("\n");
        for(ind=0; ind<grafo->nOrig; ind++){
            printf("%d, ",part2->mapa[ind]->indice);
        }
        printf("\n");
        for(ind=0; ind<grafo->nOrig; ind++){
            printf("%d, ",part3->mapa[ind]->indice);
        }
        printf("\n");
        for(ind=0; ind<grafo->nOrig; ind++){
            printf("%d, ",part4->mapa[ind]->indice);
        }
        printf("\n");
    #endif              

    typeCluster *cluster = partM->clustering->prim;
    typeElemCluster *a = NULL,
                         *b = NULL;

    typeVertex *vTarg = NULL,
                        *vRem = NULL;

    typeElemDendr *merge = NULL;

    typeEdgeAVL *edge = NULL;


    double random;

    ///140313
    //para cada C \in clustering
    while(cluster!=NULL) {
        //para cada (a,b) \in C
        //O(n²)
        a = cluster->prim;
        while(a!=NULL) {
            b = cluster->prim;
            while(b!=NULL) {
                vTarg = searchVertex(grafo->vList, a->vert);
                vRem = searchVertex(grafo->vList, b->vert);

                if(vTarg!= NULL && vRem!= NULL && a->vert != b->vert) {
                    //por construção, a e b já estão no mesmo cluster em part3
                    //então, se a e b estiverem no mesmo cluster, nas partições anteriores, pode contrair.
                    if(part2->mapa[(a->vert)-1]->indice  ==  part2->mapa[ (b->vert)-1]->indice && part1->mapa[ (a->vert)-1]->indice == part1->mapa[ (b->vert)-1]->indice
                            && part3->mapa[ (a->vert)-1]->indice == part3->mapa[ (b->vert)-1]->indice&& part4->mapa[ (a->vert)-1]->indice == part4->mapa[ (b->vert)-1]->indice) {
                        ///eles já tem que estar em partições diferentes antes
                        //se eles ja tiverem sido contraídos, nçao aparecerão de novo aqui

                        ///poderia colocar a restrição de que eles tem que ter um ganho mínimo de modularidade

                        //sorteia um número aleatório de 0 a 1, se esse número for maior que a PROBCONTR, então o par é contraído
                        random = (double)rand() / (double)RAND_MAX  ;//aleatorio de [0,1]

                        edge = searchEdge(grafo->vList , vTarg->label , vRem->label);
                        //vértices tem que ter ligação
                        if(edge != NULL && random >= PROBCONTRDEF ) {
                       //  houveContr = TRUE;
                            /**  contrai a e b ------------------------ */
                            #if PRINTALG == TRUE
                                printf("*** DEF MERGE %d %d\n", vTarg->label, vRem->label);
                            #endif
                            //nesse ponto, o dendrograma deve estar vazio, só com as pré contrações iniciais

                            merge=includeMergeDendr(dendr, lastMergeNv, vTarg->label,vRem->label);
                            edge = searchEdge(grafo->vList, vRem->label , vRem->label) ;

                            //exceto pelas pré contrações, considerar que cada vértice está em seu próprio cluster

                            //esse é o ganho para contrair clusters!!!
                            grafo->modularity += modularityIncreaseCluster(grafo, vTarg->label , vRem->label);

                            //a é o alvo
                            updateVTarg(grafo,  vTarg , vRem ,  merge);

                            //sem a maxList
                            updateAdjs(grafo, vTarg, vRem, melhorVizinho , NULL, merge);
                            updateAdjsArrow(grafo, vTarg, vRem,melhorVizinho, NULL, merge);

                            if(melhorVizinho[vTarg->label-1] != NULL){
                                    melhorVizinho[vTarg->label-1]->quality = -2;
                            }

                            calcTargAVLModularity(grafo, melhorVizinho , NULL, vTarg, vRem, vTarg->adjList);
                            updateAllKtoTarg(grafo, melhorVizinho , NULL, vTarg, vRem, vTarg->adjArrow);

                            vRem = removeVRem(grafo, vRem, melhorVizinho, NULL);
                        }
                    }
                }
                b = b->prox;
            }
            a = a->prox;
        }
        cluster = cluster->prox;
    }
}


//Path Relinking funções

/*--------------------------------------------------------
    Encontra uma correspondência entre as partições.
    Baseado na correspondência encontrada, retorna a distância entre essas partições, definida pela qtd de vértices que estão em clusters diferentes.
    zerar correspondencias

    CORRESPONDENCIAS DA PARTICAO2 NAO SAO ALTERADAS
--------------------------------------------------------*/
int encontraDistanciaClusterings(typeParticao *particao1 , typeParticao *particao2, typeGraph *grafo)
{
    typeCluster *Ci1 = particao1->clustering->prim,
                *Cj2 = NULL;
    int dist = INF;
    int distParticoes = 0;

    limpaCorrespondencias(particao2);
    limpaCorrespondencias(particao1);

    //custo: O(n²)
    while(Ci1 != NULL)
    {
        //se Cj2 ainda não foi escolhido
        Cj2 = particao2->clustering->prim;
        while(Cj2 != NULL)
        {
            //nunca vai ser NULL pois não é atualizado aqui
            if(Cj2->corresp == NULL)
            {
                dist = calcDistanciaClusters(Ci1, Cj2);

                if(dist < Ci1->dist)
                {
                    //guarda duplicado =(

                    Ci1->dist = dist;
                    Ci1->corresp = Cj2;
                }

            }
            Cj2 = Cj2->prox;

          /*  printf("durante corresp\nparticao1\n");
            printCorrespondencias(particao1);
            printf("particao2\n");
            printCorrespondencias(particao2);*/
        }

        if(Ci1->corresp != NULL)
        {
            //Cj2 = Ci1->corresp
/*            Ci1->corresp->corresp = Ci1;
            Ci1->corresp->dist = Ci1->dist; */

            distParticoes += Ci1->dist; //120213
        }



        Ci1 = Ci1->prox;

    }
    return distParticoes;
}


//f
void limpaCorrespondencias(typeParticao *particao2)
{
    typeCluster *elem = particao2->clustering->prim;

    while(elem != NULL)
    {
        elem->corresp = NULL;
        elem->dist = INF;

        elem = elem->prox;
    }
}


int calcDistanciaClusters(typeCluster *Ci1 , typeCluster *Cj2)
{
    typeElemCluster *ptrCi1 = Ci1->prim,
                    *ptrCj2 = Cj2->prim;
    int dist = 0, i ;

    while( ptrCi1 != NULL && ptrCj2 != NULL)
    {
        if(ptrCi1->vert == ptrCj2->vert)
        {
            ptrCi1 = ptrCi1->prox;
            ptrCj2 = ptrCj2->prox;
        }
        else if(ptrCi1->vert < ptrCj2->vert)
        {
            dist++;
            ptrCi1 = ptrCi1->prox;
        }
        else    //ptrCi1->vert > ptrCj2->vert
        {
            dist++;
            ptrCj2 = ptrCj2->prox;
        }

        i++;

    }


    while( ptrCi1 != NULL)
    {
        dist++;
        ptrCi1 = ptrCi1->prox;
    }

    while( ptrCj2 != NULL)
    {
        dist++;
        ptrCj2 = ptrCj2->prox;
    }


    return dist;
}