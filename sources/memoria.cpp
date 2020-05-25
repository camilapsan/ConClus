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
        /*  Como n�o est� cheio, pelo menos uma posi��o est� livre
            As elite->qtd posi��es iniciais est�o ocupadas, j� que as posi��es vazias est�o sempre nas �ltimas posi��es.
            A posi��o vazia � (elite->qtd-1) + 1    = elite->qtd
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
        else { //primeira solu��o inserida       
            elite->indMelhor = i;
        }
    }
    else{
        //printEliteSet(elite);
        //ve qual a solu��o do conj. mais parecida com a ser inserida. Se a nova solu��o (a ser inserida) for melhor que a do conjunto, substitui essa
        for(i=0; i<elite->qtd ; i++)
        {
            //calcula a dist�ncia m�nima entre as duas parti��es.
            //O proc. � feito encontrando a melhor correspond�ncia entre os clusters das 2 parti��es e retornando a qtd de elementos que est�o em clusters diferentes
            //dist = encontraDistanciaClusterings(novaP , elite->lista[i] , grafo);
            dist = elite->lista[i]->mod;

            if(dist < minDist)
            {
                minDist = dist;
                indMinDist = i;
            }
        }


        //ve se a nova solu��o � melhor com a mais parecida do conjunto
        if(novaP->mod > elite->lista[indMinDist]->mod )
        {
            //apaga a parti��o atual ent�o

            ///010513
            melhor = elite->lista[elite->indMelhor]->mod;
            elite->lista[indMinDist] = destroiParticao(elite->lista[indMinDist]);

            //insere no lugar
            //se n�o copiar, pode acontecer da refer�ncia para essa parti��o atual ser apagada no reset da main
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
    Inclui uma parti��o no conjunto elite.
    Se ainda houverem posi��es dispon�veis s� insere.
    O vetor de parti��es � mantido de forma que as posi��es vazias estejam sempre nas �ltimas posi��es.

    Caso contr�rio, ve qual a solu��o do conj. mais parecida com a nova. Se a nova for melhor que essa do conj., substitui ela.

    *De acordo com essas condi��es, a melhor solu��o encontrada nas itera��es SEMPRE vai estar dentro desse conjunto.

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
        /*  Como n�o est� cheio, pelo menos uma posi��o est� livre
            As elite->qtd posi��es iniciais est�o ocupadas, j� que as posi��es vazias est�o sempre nas �ltimas posi��es.
            A posi��o vazia � (elite->qtd-1) + 1    = elite->qtd
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
        else //primeira solu��o inserida
        {
            elite->indMelhor = i;
        }

    }
    else
    {
        //printEliteSet(elite);
        //ve qual a solu��o do conj. mais parecida com a ser inserida. Se a nova solu��o (a ser inserida) for melhor que a do conjunto, substitui essa
        for(i=0; i<elite->qtd ; i++)
        {
            //calcula a dist�ncia m�nima entre as duas parti��es.
            //O proc. � feito encontrando a melhor correspond�ncia entre os clusters das 2 parti��es e retornando a qtd de elementos que est�o em clusters diferentes
            dist = encontraDistanciaClusterings(novaP , elite->lista[i] , grafo);

            ///Correspondencias de elite->lista[i] n�o s�o alteradas

            limpaCorrespondencias(novaP);

            if(dist < minDist)
            {
                minDist = dist;
                indMinDist = i;
            }
        }


        //ve se a nova solu��o � melhor com a mais parecida do conjunto
        if(novaP->mod > elite->lista[indMinDist]->mod )
        {
            //apaga a parti��o atual ent�o

            ///010513
            melhor = elite->lista[elite->indMelhor]->mod;
            elite->lista[indMinDist] = destroiParticao(elite->lista[indMinDist]);

            //insere no lugar
            //se n�o copiar, pode acontecer da refer�ncia para essa parti��o atual ser apagada no reset da main
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

     //ve qual a solu��o do conj. mais parecida com a ser inserida. Se a nova solu��o (a ser inserida) for melhor que a do conjunto, substitui essa
      //ve qual a solu��o do conj. mais parecida com a ser inserida. Se a nova solu��o (a ser inserida) for melhor que a do conjunto, substitui essa
    for(i=0; i < elite->qtd ; i++)
    {
        //calcula a dist�ncia m�nima entre as duas parti��es.
        //O proc. � feito encontrando a melhor correspond�ncia entre os clusters das 2 parti��es e retornando a qtd de elementos que est�o em clusters diferentes
        dist = encontraDistanciaClusterings(part , elite->lista[i] , grafo);

        ///Correspondencias de elite->lista[i] n�o s�o alteradas
        limpaCorrespondencias(part);

        if(dist > maxDist)
        {
            maxDist = dist;
            indMaxDist = i;
        }
    }

    //colocar uma distancia minima entre as solu��es para fazer o PR?
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
  Destroi o conjunto elite "recursivamente", eliminando as parti��es armazenadas
  Ser� chamada apenas na se��o limpeza da main, logo antes do fim.
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

    Escolhe a melhor parti��o do conjunto elite e outras 2 aleat�rias. Os pares que tiverem na mesma comunidade nessas parti��es s�o contra�dos.

    Pode escolher uma das parti��es como a mais distante do elite set, para garantir que o par a ser contra�do realmente � recorrente!

    precisa "evaporar" essas contra��es

    140313 IMPORTANTE: Essas contra��es eram guardadadas no nv 1,
    as contra��es do primeiro n�vel de contra��o tamb�m...
    Dessa forma, quando n�o expandir as contra��es do nv 1, a fim
    de n�o expandir as contr. def., tamb�m n�o vai expandir o 1o
    nv do grafo...

    guarda essas contr. no nv 0 ou come�a as contra��es no nv 2 e
    reserva o 1 para as def.
--------------------------------------------------------*/
void contraiParesRecorrentes5EliteSet(typeEliteSet *eliteSet, typeGraph *grafo, typeNeigh **melhorVizinho,  typeDendr *dendr, int *lastMergeNv) {
    //melhor parti��o
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
        //O(n�)
        a = cluster->prim;
        while(a!=NULL) {
            b = cluster->prim;
            while(b!=NULL) {
                vTarg = searchVertex(grafo->vList, a->vert);
                vRem = searchVertex(grafo->vList, b->vert);

                if(vTarg!= NULL && vRem!= NULL && a->vert != b->vert) {
                    //por constru��o, a e b j� est�o no mesmo cluster em part3
                    //ent�o, se a e b estiverem no mesmo cluster, nas parti��es anteriores, pode contrair.
                    if(part2->mapa[(a->vert)-1]->indice  ==  part2->mapa[ (b->vert)-1]->indice && part1->mapa[ (a->vert)-1]->indice == part1->mapa[ (b->vert)-1]->indice
                            && part3->mapa[ (a->vert)-1]->indice == part3->mapa[ (b->vert)-1]->indice&& part4->mapa[ (a->vert)-1]->indice == part4->mapa[ (b->vert)-1]->indice) {
                        ///eles j� tem que estar em parti��es diferentes antes
                        //se eles ja tiverem sido contra�dos, n�ao aparecer�o de novo aqui

                        ///poderia colocar a restri��o de que eles tem que ter um ganho m�nimo de modularidade

                        //sorteia um n�mero aleat�rio de 0 a 1, se esse n�mero for maior que a PROBCONTR, ent�o o par � contra�do
                        random = (double)rand() / (double)RAND_MAX  ;//aleatorio de [0,1]

                        edge = searchEdge(grafo->vList , vTarg->label , vRem->label);
                        //v�rtices tem que ter liga��o
                        if(edge != NULL && random >= PROBCONTRDEF ) {
                       //  houveContr = TRUE;
                            /**  contrai a e b ------------------------ */
                            #if PRINTALG == TRUE
                                printf("*** DEF MERGE %d %d\n", vTarg->label, vRem->label);
                            #endif
                            //nesse ponto, o dendrograma deve estar vazio, s� com as pr� contra��es iniciais

                            merge=includeMergeDendr(dendr, lastMergeNv, vTarg->label,vRem->label);
                            edge = searchEdge(grafo->vList, vRem->label , vRem->label) ;

                            //exceto pelas pr� contra��es, considerar que cada v�rtice est� em seu pr�prio cluster

                            //esse � o ganho para contrair clusters!!!
                            grafo->modularity += modularityIncreaseCluster(grafo, vTarg->label , vRem->label);

                            //a � o alvo
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


//Path Relinking fun��es

/*--------------------------------------------------------
    Encontra uma correspond�ncia entre as parti��es.
    Baseado na correspond�ncia encontrada, retorna a dist�ncia entre essas parti��es, definida pela qtd de v�rtices que est�o em clusters diferentes.
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

    //custo: O(n�)
    while(Ci1 != NULL)
    {
        //se Cj2 ainda n�o foi escolhido
        Cj2 = particao2->clustering->prim;
        while(Cj2 != NULL)
        {
            //nunca vai ser NULL pois n�o � atualizado aqui
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