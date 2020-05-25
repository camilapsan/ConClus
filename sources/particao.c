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

#include "particao.h"

/** PARTICAO*/

//RECURSAO
void atribuiParticaoRec(typeDendr *dendr, int *particao, int label, int qtdVolta, int iAt)
{
    typeElemDendr *elemAux;
    iAt = iAt - qtdVolta;
    elemAux = dendr->list[iAt];

    //achou a contração "apontada" pelo valor positivo
    if(elemAux!=NULL)
    {
        //particao de elemAux
        ///pode jogar isso no else do elemAux para economizar atribuicao
        particao[elemAux->label1 - 1] = label;
        //  particao[elemAux->label2 - 1] = label;

        if(elemAux->col1 > 0)
        {
            atribuiParticaoRec(dendr , particao, label, elemAux->col1 , iAt);
        }
        if(elemAux->col2 > 0)
        {
            atribuiParticaoRec(dendr , particao, label, elemAux->col2 , iAt);
        }

        else
        {
            //no isolado
            particao[(elemAux->col2 )*(-1) - 1] = label;
        }
    }
    else
    {
    }

}

typeParticao *inicializaParticao(int n)
{
    typeParticao *particao = (typeParticao*) malloc(sizeof(typeParticao));

    particao->mapa = (typeCluster**) malloc(sizeof(typeCluster*) * n);
    particao->clustering = NULL;

    //12012013
    particao->mod = MENORQ;
    return particao;

}
/*  cria uma partição para o grafo base, sem considerar os vértices removidos
    cria uma estrutura de clustering

    Cada posição da partição aponta para a estrutura de cluster

    A partição corresponde aos vértices do grafo base singletons, já que nenhuma busca local foi feita ainda.
    O clustering recebido como parâmetro é um clustering vazio
  */



//f
//essa função obtem a partição completa, ou seja, inclui inclusive os vértices que estão removidos
//Fins de teste
int *particaoTradicional(typeGraph *grafo, typeDendr *dendr, typeBeginGraphNv *beginGraphNv, int nvEscolhido)
{
    int i;
    int *particao = (int*) malloc(sizeof(int)*grafo->nOrig);

    //topo
    typeElemDendr *elemDendr = dendr->list[dendr->nMerge-1] ;

    for(i=0 ; i< grafo->nOrig ; i++ )
    {
        particao[i] = -1;
    }

    for(i = dendr->nMerge- 1 ;  i>= 0 && elemDendr!=NULL; i--)
    {

        elemDendr = dendr->list[i];

        if(particao[elemDendr->label1-1] == -1)
        {
            //  printf("Visitando %d\n", i );
            //nao precisa atribuir para label-1, caso col seja negativo pode usar (col)*(-1) - 1 e caso seja positivo a funcao recursiva cuida
            particao[elemDendr->label1-1] = elemDendr->label1;

            //caso um dos valores col1, col2 seja positivo, precisa buscar a contração que se refere e atribuir aos vértices label 1
            // label 2 nesse caso é inutil? aparentemente sim
            //precisa procurar de forma "recursiva" pelos valores
            if(elemDendr->col1 > 0)
            {
                atribuiParticaoRec(dendr, particao , elemDendr->label1, elemDendr->col1, i );
            }
            if(elemDendr->col2 > 0)
            {
                atribuiParticaoRec(dendr, particao , elemDendr->label1, elemDendr->col2 , i);
            }
            else
            {
                //no isolado
                particao[(elemDendr->col2 )*(-1) - 1] = elemDendr->label1;
            }
        }
        else
        {
        }
    }

    //o que for -1 é singleton agora
    for(i=0; i<grafo->nOrig ; i++)
    {
        if(particao[i]==-1)
        {
            particao[i] = i+1;
        }
    }
    return particao;
}

//f
//FALTA HEADER
int *inicializaMapaInt(int n)
{
    return (int*) calloc(sizeof(int),n);
}

//f
//FALTA HEADER
int *copiaMapaDestinoInt(typeCluster **fonte, int *destino, int n)
{
    int i;

    for(i=0; i<n ; i++)
    {
        if(fonte[i]!=NULL)
        {
            destino[i] = fonte[i]->indice;
        }
    }

    return destino;
}

int *copiaMapaIntDestinoInt(int *fonte, int *destino, int n)
{
    int i;

    for(i=0; i<n ; i++)
    {
        destino[i] = fonte[i];
    }

    return destino;
}

///precisa refazer essas funções para a buscaKL,
///o mapa destino na verdade deve guardar os clusters do clustering destino, por assim dizer

typeCluster** copiaMapaDestinoMapa(typeCluster **fonte, typeCluster **destino, int n)
{
    int i;

    for(i=0; i<n ; i++)
    {
        destino[i] = fonte[i];

    }

    return destino;
}


typeCluster **copiaMapaAloca(typeCluster **fonte, int n)
{
    typeCluster **nova = (typeCluster**) malloc(sizeof(typeCluster*) * n);

    nova = copiaMapaDestinoMapa(fonte , nova, n);

    return nova;
}

typeDendr *copiaDendrAloca(typeDendr *fonte, int maxLevels, int redFac, int nVert)
{
    int i;
    int qtd = (redFac * nVert + 1) *maxLevels;

    typeDendr *novo = (typeDendr*) malloc(  sizeof(typeDendr)   );

    novo->beginGraphNv = createBeginGraphNv();

    for(i=0 ; i < qtd ; i++)
    {
        novo->list = (typeElemDendr**) malloc(   sizeof(typeElemDendr*)   );

    }

    return novo;
}


typeParticao* copiaParticao(typeParticao *fonte, typeParticao *destino, int n)
{
    typeCluster *novoCluster = NULL,
                               *cluster = NULL;
    typeElemCluster *elem = NULL;

    destino->mod = fonte->mod;

    if(destino->clustering!= NULL)
    {
        destino->clustering = destroiClustering(destino->clustering);
    }

    destino->clustering = inicializaClustering();

    destino->clustering->qtdIsolados = fonte->clustering->qtdIsolados;
    //para cada um dos clusters da fonte
    cluster = fonte->clustering->prim;
    while(cluster != NULL)
    {
        //cria uma cópia do cluster no destino
        novoCluster = insereCluster( destino->clustering , cluster->indice);
        copiaCluster( cluster , novoCluster);

        //faz o mapa dos vértices dentro desse cluster apontarem para ele

        elem = novoCluster->prim;
        while(elem!= NULL)
        {
            destino->mapa[elem->vert-1] = novoCluster;

            elem = elem->prox;
        }


        cluster = cluster->prox;
    }


    return destino;
}

typeParticao* copiaParticaoAloca(typeParticao *particao, int n)
{
    int i;
    typeParticao *nova = (typeParticao*) malloc(sizeof(typeParticao));

    nova->mod = particao->mod;
    nova->clustering = NULL;
    nova->mapa = (typeCluster**) malloc(sizeof(typeCluster*) * n);

    for(i=1 ; i<= n; i++ )
    {
        if(  particao->mapa[i-1] == NULL )
        {
            nova->mapa[i-1] = NULL;
        }
    }

    copiaParticao(particao, nova, n);

    return nova;
}


void inicializaParticaoList()
{
    typeParticaoList *particaoList = (typeParticaoList*) malloc(sizeof(typeParticaoList));
    particaoList->prim = NULL;
    particaoList->ult = NULL;
    particaoList->qtd = 0;
}


typeParticao* destroiParticao(typeParticao *particao)
{
    if(particao!=NULL)
    {
        free(particao->mapa);
        particao->mapa = NULL;

        particao->clustering = destroiClustering( particao->clustering);

        free(particao);
        particao = NULL;

        return particao;
    }
}

//f
void printParticaoIntFileName(int* particao, int size, char *nomeArq)
{
    int i;

    FILE *fileP = fopen( nomeArq ,"w");

    if(fileP!=NULL)
    {
        for(i=0; i<size; i++)
        {
            fprintf(fileP, "%d\n" , particao[i]);

        }
    }
    fclose(fileP);
}

void printParticaoInt(int* particao, int size)
{
    int i;

    if(particao!=NULL)
    {
        for(i=0; i<size; i++)
        {
            printf( "%d\n" , particao[i]);

        }

    }
    else{
        printf("particão nula\n");
    }
}

void printParticao(typeParticao* particao, int size)
{
    int i;
    printf("Particao\n");
    for(i=0; i<size; i++)
    {
        if(particao->mapa[i]!= NULL)
        {
            printf("%d\t" , particao->mapa[i]->indice);
        }
        else
        {
            printf( "-1\n");
        }
    }
    printf("\n");
}

typeParticao* criaParticaoFromInt(int* partInt, typeGraph *grafo){
    int i;

    typeParticao *particao = (typeParticao*) malloc(sizeof(typeParticao));

    particao->mapa = (typeCluster**) malloc(sizeof(typeCluster*) * grafo->nOrig);
    particao->clustering  = inicializaClustering();

    typeCluster *cluster = NULL;

    particao->mod = grafo->modularity;

    //aqui está percorrendo todos os vértices, sendo que só acessa o grafo base
    //ideia da LDE com vértices ainda existentes no grafo
    for(i=1 ; i<=grafo->nOrig ; i++ )
    {
            //o índice do cluster que vai inserir é o da particao int
            cluster = insereVerticeNoClusterInd(particao->clustering, i ,partInt[i-1]);
            particao->mapa[i-1] = cluster;
    }

    return particao;
}

