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

#include "clusteringList.h"

typeElemCluster* criaElemCluster(int ind)
{
    typeElemCluster *novo = (typeElemCluster*) malloc(sizeof(typeElemCluster));
    novo->vert = ind;
    novo->prox = NULL;

    return novo;
}


typeCluster* criaCluster(int ind)
{
    typeCluster *cluster = (typeCluster*) malloc(sizeof(typeCluster));
    cluster->indice = ind;
    cluster->prim = NULL;    //inicialmente não há nenhum elem;
    cluster->ult = NULL;
    cluster->prox = NULL;
    cluster->qtd = 0;

    //261212
    cluster->dist = INF;
    cluster->corresp = NULL;

    return cluster;
}

typeClustering* inicializaClustering()
{
    typeClustering *clustering = (typeClustering*) malloc(sizeof(typeClustering));
    clustering->prim = NULL;
    clustering->ult = NULL;
    clustering->qtd = 0;
    clustering->qtdIsolados= 0;
    return clustering;

}

//alterar para inserir em ordem
void insereVerticeNoClusterPtr(typeCluster *list , int vertice)
{
    /*  cluster de origem: C(vertice)
    cluster de desitno: destino
    movimento: C(vertice) <- destino  */

    typeElemCluster *novo = criaElemCluster(vertice);

    typeElemCluster *anterior = NULL;

    if(list->prim == NULL)
    {
        list->prim = novo;
        list->ult = novo;
    }
    else
    {
        //achar a posição certa

        //26122012
        if( vertice < list->prim->vert)
        {
            anterior = NULL;
              //insere antes do primeiro

            novo->prox = list->prim;
            list->prim = novo;
        }
        else
        {
            anterior = list->prim;

            while (anterior->prox != NULL && vertice > anterior->prox->vert)
            {
                anterior = anterior->prox;
            }


            //insere depois do último
            if(anterior == list->ult)
            {
                anterior->prox = novo;
                list->ult = novo;

                //   printf("Ultimo inserido em %p\n",elem);
            }
            else
            {
                //insere depois do anterior
                novo->prox = anterior->prox;
                anterior->prox = novo;
            }
        }
    }

    list->qtd++;
}


typeElemCluster *removeElemClusterInd(typeClustering* clustering, int indCluster, int elem)
{
    return removeElemClusterPtr(clustering, procuraCluster(clustering, indCluster),  elem);
}

typeElemCluster *removeElemClusterPtr(typeClustering* clustering, typeCluster *cluster, int elem)
{

    typeElemCluster *anterior = NULL;

    typeElemCluster *elemC = NULL;

    //enquanto a qualidade do elemento que procuro for menor que a do atual
    //precisa continuar procurando


    if(cluster->prim != NULL)
    {
        if( elem == cluster->prim->vert)
        {
            cluster->prim = cluster->prim->prox;
        }
        else
        {
            //procura o elemento anterior, de forma que o elemento é anterior->prox !!
            anterior = procuraElemClusterAnt( cluster , elem);

            elemC = anterior->prox;
            //remove ultimo

            //RELEASE APAGAR
            //Assert if(cluster!=NULL)

            if(elemC  == cluster->ult)
            {
                //anterior do ultimo é o anterior do cluster
                cluster->ult = anterior;
                anterior->prox = NULL;
            }
            //intermediario
            else
            {
                anterior->prox = elemC->prox;

            }
        }

        free(elemC);
        elemC = NULL;

        cluster->qtd--;

        //cluster vazio
        if(cluster->prim == NULL)
        {
  
        }

        //Limpeza
         //retorna o que acabou de ser removido
        //ou achou ou não existe!
    }
    return elemC;
}

void insereClusterExistente(typeClustering *list, typeCluster *cluster)
{
    if(list->prim == NULL)
    {
        list->prim = cluster;
        list->ult = cluster;
    }
    else
    {
        list->ult->prox = cluster;
        list->ult = cluster;

    }
    list->qtd++;

}

typeCluster *insereCluster(typeClustering *list , int ind)
{
    /*  cluster de origem: C(vertice)
        cluster de desitno: destino
        movimento: C(vertice) <- destino  */
        typeCluster *novo = criaCluster(ind);

        if(list->prim == NULL)
        {
            list->prim = novo;
            list->ult = novo;
        }
        else
        {
            list->ult->prox = novo;
            list->ult = novo;

        }
        list->qtd++;

        return novo;
}

//essa função é chamada qdo não houverem mais elementos no cluster
//para esse caso é útil uma lista duplamente encadeada
typeCluster *removeCluster(typeClustering* clustering, typeCluster *cluster)
{
    //RELEASE APAGAR
    /**Assert     cluster->qtd != 0 */
//    typeCluster *aux = clustering->prim;
    typeCluster *anterior = NULL;
    //enquanto a qualidade do elemento que procuro for menor que a do atual
    //precisa continuar procurando
    if(clustering->prim != NULL)
    {
        if( cluster == clustering->prim)
        {
         //   printf("rem prim");
            clustering->prim = clustering->prim->prox;
        }
        else
        {    
            //já sei qual é o cluster que quero remover

            anterior = procuraClusterAnt(clustering , cluster->indice);
            //remove ultimo
            //RELEASE APAGAR
            //Assert if(cluster!=NULL)

            if(cluster == clustering->ult)
            {
                //anterior do ultimo é o anterior do cluster
                clustering->ult = anterior;
                anterior->prox = NULL;
            }
            //intermediario
            else
            {
                anterior->prox = cluster->prox;
            }
        }

        free(cluster);
        cluster = NULL;

        clustering->qtd--;
        //Limpeza
        //retorna o que acabou de ser removido
        //ou achou ou não existe!
    }
    return cluster;
}

typeElemCluster *procuraElemClusterAnt(typeCluster *cluster , int ind)
{
    typeElemCluster *elem = cluster->prim;

    if(ind == cluster->prim->vert)
    {
        return NULL;
    }
    else
    {
        while(elem->prox !=NULL && elem->prox->vert != ind)
        {
            elem = elem->prox;
        }

        return elem;

    }
}

typeCluster *procuraIesimoCluster(typeClustering *clustering, int iesimo)
{
    typeCluster *cluster = clustering->prim;
    int i;

    if(iesimo <= clustering->qtd)
    {
        for( i = 1  ; i < iesimo && cluster!=NULL; i++)
        {
            cluster = cluster->prox;
        }

        return cluster;
    }
    else
    {
        return NULL;
    }
}

typeCluster *procuraCluster(typeClustering *clustering, int ind)
{
    typeCluster *cluster = clustering->prim;

    while(cluster!=NULL && cluster->indice!=ind)
    {
        cluster = cluster->prox;
    }
    return cluster;
}

typeCluster *procuraClusterAnt(typeClustering *clustering, int ind)
{
    typeCluster *cluster = clustering->prim;

    /** Assert clustering->prim->indice != ind */

    while(cluster->prox!=NULL && cluster->prox->indice!=ind)
    {
        cluster = cluster->prox;
    }
    return cluster;
}

typeCluster* insereVerticeClusterIsolado(typeClustering *clustering, typeCluster *cluster,  int vert, int nVert)
{
    //pode usar o cluster do parâmetro: é só arrumar o índice dele
    int ind;

    clustering->qtd++;
    clustering->qtdIsolados++;
    //define um índice que ainda não foi utilizado
    ind = nVert + clustering->qtdIsolados;
    cluster->indice = ind;
    //printf("cluster indice=%d\n",cluster->indice);

    insereClusterExistente(clustering , cluster);///271212
    insereVerticeNoClusterPtr( cluster  , vert );


    return cluster;

}

/*
    insere o vértice no cluster
    caso o cluster ainda não exista, cria ele
    retorna o cluster no qual o vértice foi inserido
*/
typeCluster* insereVerticeNoClusterInd(typeClustering *clustering , int vert, int indCluster)
{
    //procura o cluster particao[i]
    typeCluster *cluster = procuraCluster(clustering, indCluster);
    //se o cluster ainda não existir cria,
    if(cluster==NULL)
    {
        cluster = insereCluster(clustering, indCluster);
    }

    insereVerticeNoClusterPtr(cluster, vert);

    return cluster;
}

/*
    1 = true : o cluster é isolado
    0 = false: o cluster não é isolado
*/
int verifClusterIsolado(typeCluster *cluster)
{
    //RELEASE APAGAR
   /// assert(cluster!=NULL);
   /// assert(cluster->qtd != 0);
    /* if (cluster->qtd > 1) , porém sabe-se que cluster->qtd>0
        então pode-se fazer simplismente (cluster->qtd!=1) */
    if(cluster->qtd != 1)
    {
        return 0;
    }
    else
    {
        return 1;
    }

}


/** COPIA */


typeCluster *copiaCluster(typeCluster *fonte, typeCluster *destino)
{
    typeElemCluster *elemFonte=NULL;

    destino->indice = fonte->indice;
    //quando insere o elemento já incrementa
    // destino->qtd = fonte->qtd;
    destino->qtd = 0;

    elemFonte = fonte->prim;
    while(elemFonte !=NULL)
    {
        insereVerticeNoClusterPtr(destino, elemFonte->vert);
        elemFonte = elemFonte->prox;
    }

    destino->corresp = NULL;
    destino->dist = INF;

    return destino;
}


typeClustering *copiaClustering(typeClustering *fonte)
{
    typeClustering *novoClustering = inicializaClustering();
    typeCluster *cluster=NULL;
    typeCluster *novoCluster=NULL;

    // novoClustering->qtd = fonte->qtd;
    //quando insere o cluster já adiciona isso
    novoClustering->qtdIsolados = fonte->qtdIsolados;

    cluster = fonte->prim;
    while(cluster!=NULL)
    {
        novoCluster = insereCluster(novoClustering, cluster->indice);
        copiaCluster( cluster , novoCluster);

        cluster = cluster->prox;
    }

    return novoClustering;
}

typeCluster *destroiCluster(typeCluster *cluster)
{
    typeElemCluster *aux1 = cluster->prim;
    typeElemCluster *aux2 = NULL;

    while(aux1!=NULL)
    {
        aux2 = aux1->prox;

        free(aux1);
        aux1 = NULL;

        aux1 = aux2;
    }

    cluster->prim = NULL;
    cluster->ult = NULL;

    free(cluster);
    cluster = NULL;

    return cluster;
}

typeClustering *destroiClustering(typeClustering *clustering)
{
    typeCluster *aux1 = clustering->prim;
    typeCluster *aux2 = NULL;

    while(aux1!=NULL)
    {
        aux2 = aux1->prox;

        aux1->prox = NULL;
        aux1 = destroiCluster(aux1);

        free(aux1);
        aux1 = NULL;

        aux1 = aux2;
    }


    clustering->prim = NULL;
    clustering->ult = NULL;

    free(clustering);
    clustering = NULL;

    return clustering;
}

void printCluster(typeCluster *cluster)
{
    typeElemCluster *elem = cluster->prim;

    while(elem!=NULL)
    {
        printf("%d ", elem->vert);
        elem = elem->prox;
    }
    printf("\n");
}

void printClustering(typeClustering *clustering)
{
    typeCluster *cluster = clustering->prim;

    while(cluster!=NULL)
    {
        printf("Cluster %d: (qtd: %d)\t",cluster->indice,cluster->qtd);
        printCluster(cluster);

        cluster = cluster->prox;
    }
    printf("\n");
}