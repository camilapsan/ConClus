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

#include "vetorAVL.h"

//Aloca todos os vértices e os armazena em uma Hash Table simples, para facilitar o acesso e evitar verificações e criações posteriores

//aloca novo vértice
typeVertex* createVertex(int label)
{
    typeVertex *newVertex = (typeVertex*) malloc(sizeof(typeVertex));
    newVertex->label = label;
    newVertex->adjList = NULL;
    newVertex->adjArrow = NULL; //DIRECTED GRAPH 020115
    newVertex->contribuicao=0;

    return newVertex;
}

typeVertex** createVertexList(int nVert)
{
    typeVertex** vertexList = (typeVertex**) malloc(sizeof(typeVertex* )   *   (nVert+1));

    int i;

    for(i=1;i<=nVert;i++)
    {
        vertexList[i-1] = createVertex(i);
    }

    return vertexList;
}

//f
typeVertex* searchVertex(typeVertex** vertexList,int label)
{
     return vertexList[label-1];
}

//f
//FALTA HEADER
typeEdgeAVL *searchAVL(typeEdgeAVL *avl, typeVertex *vertex)
{
    if(vertex->label > avl->vAdj)
    {
        if(avl->right!= NULL)
        {
            searchAVL(avl->right, vertex);
        }
        else return NULL;
    }
    else
    {
        if(vertex->label < avl->vAdj)
        {
            if(avl->left!=NULL)
            {
                searchAVL(avl->left , vertex);
            }
            else return NULL;
        }
        else
        {
            return avl;
        }
    }
}

//f
typeEdgeAVL* searchEdge(typeVertex** vertexList, int label1, int label2)
{
    typeVertex *v1, *v2;
    typeEdgeAVL *edge = NULL;

    //Procura pelo indice maior na lista do menor

    v1 = searchVertex(vertexList, label1);
    v2 = searchVertex(vertexList, label2);

    //precisa verif. se e NULL pois ja pode ter sido removido do grafo
    if(v1!= NULL && v2!=NULL)
    {
        //procurar por v2->label, não por v2
        ///Pode acontecer da lista ser nula se tiver acabado de remover o Rem e ainda nao adicionado o Targ
        //util deixar essa verificacao
        if(v1->adjList!=NULL) edge = searchAVL(v1->adjList, v2);
    }

    return edge;
}

typeEdgeAVL* searchEdgeArrow(typeVertex** vertexList, int label1, int label2)
{
    typeVertex *v1, *v2;
    typeEdgeAVL *edge = NULL;

    //Procura pelo indice maior na lista do menor

    v1 = searchVertex(vertexList, label1);
    v2 = searchVertex(vertexList, label2);

    //precisa verif. se e NULL pois ja pode ter sido removido do grafo
    if(v1!= NULL && v2!=NULL)
    {
        //procurar por v2->label, não por v2
        ///Pode acontecer da lista ser nula se tiver acabado de remover o Rem e ainda nao adicionado o Targ
        //util deixar essa verificacao
        if(v1->adjArrow!=NULL) edge = searchAVL(v1->adjArrow, v2);
    }

    return edge;

}

//f
double searchEdgePound(typeGraph *graph, int label1, int label2)
{
    typeVertex *v1, *v2;
    typeEdgeAVL *edge = NULL;

//  DIRECTED GRAPH
//    //tanto faz em qual lista procura
        v1 = searchVertex(graph->vList, label1);
        v2 = searchVertex(graph->vList, label2);

//procurar por v2->label, não por v2
    if(v1->adjList!=NULL) edge = searchAVL(v1->adjList, v2);

    if(edge!=NULL) return edge->pound;
    else return 0;

}



///--------------------------------------------------------------------------------------
///-AVL----------------------------------------------------------------------------------
///--------------------------------------------------------------------------------------


//INICIALIZAÇÕES-----------------------------------------------------------------

typeEdgeAVL *createEdgeAVL(int vertex, double pound)
{
    typeEdgeAVL *edge = (typeEdgeAVL*) malloc(sizeof(typeEdgeAVL));

    edge->vAdj = vertex;
    edge->left = NULL;
    edge->right = NULL;
    edge->parent = NULL ;//1209
    edge->pound = pound;
    edge->altura = calculaAltura(edge);
    //26/10/2012 - só aloca quando necessário

    edge->quality = 0; //precisa ser calculado

    return edge;

}

//-----------------------------------------------------------------

int maior(int num1, int num2) //maior valor entre dois numeros
{
    if(num1>num2)
        return num1;
    else
        return num2;
}

//calcula a maior altura a partir do noAux
int calculaAltura(typeEdgeAVL *noAux)
{
    if(noAux==NULL)
         return 0;
    else //noAux!=NULL
        //retorna a maior altura entre a sub arvore da esquerda e da righteita, somando 1 a cada no
        return maior(calculaAltura(noAux->left) , calculaAltura(noAux->right))  +1;
}

//FIM UTILIDADES -------------------------------------------------

//ROTACOES -------------------------------------------------------
typeEdgeAVL *rotaSimplesDir(typeEdgeAVL *k2)
{
    typeEdgeAVL *k1 = k2->left;
    if(k2->left!= NULL) k1->parent = k2->parent;

    k2->left = k1->right ;
    if(k1->right!=NULL) k2->left->parent = k2;

    k1->right = k2;
    if(k2!=NULL) k1->right->parent = k1;


    k1->altura = calculaAltura(k1);
    k2->altura = calculaAltura(k2);

    return k1;
}

typeEdgeAVL *rotaSimplesEsq(typeEdgeAVL *k2)
{
    typeEdgeAVL *k1 = k2->right;
    if(k2->right!=NULL ) k1->parent =  k2->parent;///X

    k2->right = k1->left;
    if(k1->left!=NULL) k2->right->parent = k2;///


    k1->left = k2;
    if(k2!=NULL) k1->left->parent = k1; ///

    k1->altura = calculaAltura(k1);
    k2->altura = calculaAltura(k2);

    return k1;
}

typeEdgeAVL *rotaDuplaDir(typeEdgeAVL *k3)
{
    typeEdgeAVL *ret;
    k3->left = rotaSimplesEsq(k3->left);
    if(k3->left !=NULL ) k3->left->parent = k3; ///balancear quando há o campo pai


    ret = rotaSimplesDir(k3);

    return ret;
}

typeEdgeAVL *rotaDuplaEsq(typeEdgeAVL *k3)
{
    typeEdgeAVL *ret;
    k3->right = rotaSimplesDir(k3->right);
    if(k3->right!=NULL) k3->right->parent = k3;///

    ret = rotaSimplesEsq (k3);

    return ret;
}

//FIM ROTACOES -------------------------------------------------

//BALANCEAMENTO ------------------------------------------------
int calculaFB (typeEdgeAVL *noAux)
{
    int alturaDir=0,alturaEsq=0; //variaveis auxiliares
    //se noAux->left existir alturaEsq sera a altura de noAux->left,
    //se nao existir alturaEsq mantem o valor inicial 0
    if(noAux->left!=NULL) alturaEsq = noAux->left->altura;

    //mesma coisa para noAux->right
    if(noAux->right!=NULL) alturaDir = noAux->right->altura;

    return (alturaEsq - alturaDir);//retorna FB
    //se FB<0 quer dizer que a sub arvore da righteita eh maior
    //se FB>0 quer dizer que a sub arvore da esquerda eh maior
}

typeEdgeAVL *verificaBalanceamento(typeEdgeAVL *noAux)
{
    int fb = calculaFB(noAux); //calcula fb do no

    if (fb>1 || fb<(-1)) //quer dizer que o no esta desbalanceado
        return noAux; //retorna o proprio no
    else //o no esta balanceado
        return NULL;
}

typeEdgeAVL *procuraNoCausa(typeEdgeAVL *noAux)
{
    //procura a folha mais afastada do no, eh ela que esta causando o desbalanceamento

    int fb = calculaFB(noAux);

    if(fb>=1) //arv da esquerda eh maior, continua a busca nela
        {if(noAux->left!=NULL) procuraNoCausa(noAux->left); }

    if (fb<=(-1)) //arv da righteita eh maior, continua a busca nela
        {if(noAux->right!=NULL) procuraNoCausa(noAux->right);}

    if (fb==0) //o no esta balanceado, mas nao necessariamente a arvore.
    {
        //se o no for folha ele eh o no que causou
        if(noAux->right==NULL && noAux->left == NULL ) return noAux;

        //como esta balanceado e nao eh folha, noAux->right e noAux->left existem
        else //MUDEI DEPOIS DE ENTREGAR
        { 
            if( (calculaFB(noAux->right)*(-1) ) < calculaFB(noAux->left) )
                procuraNoCausa(noAux->right); //continua procurando folha na arvore da righteita, definicao
            else
                procuraNoCausa(noAux->left);
        }
    }
}

//define qual rotacao sera utilizada para balancear o no
typeEdgeAVL* rotacionarNo(typeEdgeAVL *noDesb, typeEdgeAVL *noCausa)
{
    if(noDesb != NULL)   //verificacao adicional
    {
        if(noDesb->vAdj > noCausa->vAdj ) //noCausa esta na sub arv left
        {
            //noCausa esta na sub arv right de de noDesb->left
            if(noCausa->vAdj >  noDesb->left->vAdj)
                noDesb = rotaDuplaDir(noDesb);

            //noCausa esta na sub arv left de de noDesb->left
            else if(noCausa->vAdj < noDesb->left->vAdj)
                { noDesb = rotaSimplesDir(noDesb); }
        }
        else if(noCausa->vAdj > noDesb->vAdj) //nocausa esta na sub arv right
        {
            //noCausa esta na sub arv left de de noDesb->right
            if(noCausa->vAdj < noDesb->right->vAdj)
            {
                noDesb = rotaDuplaEsq(noDesb);
            }
            //noCausa esta na sub arv right de de noDesb->right
            else if (noCausa-> vAdj > noDesb->right->vAdj)
            {
                noDesb = rotaSimplesEsq(noDesb);
            }
        }

    }

   //retorna o no que ocupara o lugar do antes noDesb
   return noDesb;
}

typeEdgeAVL *balancear(typeEdgeAVL *noAux)
{
    typeEdgeAVL *noDesb, *noCausa;
    //noDesb - no desbalanceado
    //noCausa - no que causou o desbalanceamento

    //retorna se o no que esta desbalanceado
    noDesb = verificaBalanceamento(noAux);

    if (noDesb!=NULL) //se o no estiver desbalanceado
    {
        noCausa = procuraNoCausa(noAux); //procura o no que causou o desbalanceamento
      //  printf("noDesb %d noCausa %d\n",noDesb->vAdj->label,noCausa->vAdj->label);//teste
        noAux = rotacionarNo(noDesb,noCausa); //aplica as rotacoes necessarias no no
      //  printf("auxNovo %d  left %d  right %d\n",noAux->vAdj->label,noAux->left->vAdj->label,noAux->right->vAdj->label); //teste
        noAux->altura = calculaAltura(noAux); //atualiza a altura
     }

    //retorna o no balanceado
    //se ja estivesse balanceado retorna ele mesmo
     return noAux;

}
//FIM BALANCEAMENTO ----------------------------------------------------

//INSERCAO ------------------------------------------------------
//1209 * agora os nós da avl tem um campo pai p/ func. de atualização na contracao
typeEdgeAVL* insereArvore(typeEdgeAVL *noAux, typeEdgeAVL *vertex)
{
    if(vertex->vAdj < noAux->vAdj)
    {
        if(noAux->left==NULL)
        {
            noAux->left = vertex;
        }
        else
           noAux->left = insereArvore(noAux->left, vertex); //continua procurando inserir a esquerda
     }
    else
    {
        if (vertex->vAdj > noAux->vAdj)
        {
            if(noAux->right==NULL)
            {
                noAux->right = vertex;
            }

            else
                noAux->right = insereArvore(noAux->right,vertex); //continua procurando inserir a righteita
        }
        else
        {
            noAux->pound += vertex->pound;//novo

        }
        //else //Elemento ja existe
    }

    noAux->altura = calculaAltura(noAux); //atualiza a altura do noAux
    //aplica a rotina de balanceamento no no
    //se ja estiver balanceado nao faz nada
    noAux = balancear(noAux);

    return noAux;
}


//retorna a nova raiz após balanceamento
typeEdgeAVL* includeEdgeSimples(typeGraph *graph, typeVertex *vertX, typeVertex *vertY, double pound)
{
      typeEdgeAVL *edge;

    //DIRECTED GRAPH OK
    if(vertX!= NULL && vertY!= NULL)
    {
        edge = createEdgeAVL(vertY->label,pound);

        if(vertX->adjList == NULL)
        {
            vertX->adjList = edge;
        }
        else
        {
           vertX->adjList = insereArvore( vertX->adjList, edge);
        }

   }

   return edge;
}

//retorna a nova raiz após balanceamento
typeEdgeAVL* includeEdgeSimplesArrow(typeGraph *graph, typeVertex *vertX, typeVertex *vertY, double pound) //f
{
      typeEdgeAVL *edge;

    //DIRECTED GRAPH OK

    if(vertX!= NULL && vertY!= NULL)
    {
        edge = createEdgeAVL(vertY->label,pound);

        if(vertX->adjArrow == NULL)
        {
            vertX->adjArrow = edge;
        }
        else
        {
           vertX->adjArrow = insereArvore( vertX->adjArrow, edge);
        }
   }
   return edge;
}

//retorna a nova raiz após balanceamento
void includeEdge(typeGraph *graph, typeVertex *vertX, typeVertex *vertY, double pound)
{
    // vertX -> vertY

      typeEdgeAVL *edge;

    //Insere vertY na lista do vertX

    graph->m+=(double)(pound);

    if(vertX!= NULL && vertY!= NULL)
    {
        edge = createEdgeAVL(vertY->label,pound);


        graph->vDegree[vertX->label -1 ]+= pound ;//pound; //27/10

        if(vertX->adjList == NULL)
        {
            vertX->adjList = edge;
        }
        else
        {
           vertX->adjList = insereArvore( vertX->adjList, edge);
        }

          graph->vDegreeArrow[vertY->label -1] += pound; //DIRECTED GRAPH 31122014

//  DIRECTED GRAPH 020115 - incident vertY <- vertX

        edge = createEdgeAVL(vertX->label,pound);


        if(vertY->adjArrow == NULL)
        {            
            vertY->adjArrow = edge;
        }
        else
        {
           vertY->adjArrow = insereArvore( vertY->adjArrow, edge);
        }
   }
}


//------------------------------------------------------------------
//FIM INSERCAO ---------------------------------------------------------


//REMOCAO---------------------------------------------------------

//retorna o PAI do maior valor da sub arvore da righteita
 typeEdgeAVL *maiorValorPai(typeEdgeAVL *aux)
 {
     //se o neto existir
     if (aux->right!=NULL && aux->right->right != NULL) //o maior valor sera o ultimo no a direita
        maiorValorPai(aux->right);

        //se o neto não existe, então retorna o próprio pai
     else return aux; //aux->right == NULL , encontrou o ultimo no a righteita
 }

//retorna o maior valor da sub arvore da righteita
typeEdgeAVL *maiorValor(typeEdgeAVL *aux)
{
     //se o neto existir
    if (aux->right != NULL) //o maior valor sera o ultimo no a direita
    {   maiorValor(aux->right);    }
        //se o neto não existe
     else return aux; //aux->right == NULL , encontrou o ultimo no a righteita
}

///------------------------------------------------------------

typeEdgeAVL *removeAdjList(typeEdgeAVL *aux, typeGraph *graph)
{
    if(aux->left!=NULL)
    {
        aux->left = removeAdjList(aux->left,graph);
    }
    if(aux->right!=NULL)
    {
        aux->right = removeAdjList(aux->right , graph);
    }
    aux->right = NULL;
    aux->left = NULL;
    free(aux);
    aux=  NULL;

    return NULL;
}

typeGraph* removeVertex(typeGraph *graph, int rem)
{

    graph->nVert--;

    graph->vDegree[rem-1] = (double)0;
    graph->vDegreeIn[rem-1] = (double)0;
    //remove a lista de adjacências inteira

    //se não foi removido do grafo ainda
    if(graph->vList[rem-1]!=NULL)
    {

        if(graph->vList[rem-1]->adjList !=NULL)
        {
        ///PRECISA REMOVER PRECED LIST 26102012
            graph->vList[rem-1]->adjList = removeAdjList(graph->vList[rem-1]->adjList, graph);
        }

        //DIRECTED GRAPH 020115
       if(graph->vList[rem-1]->adjArrow !=NULL)
        {
        ///PRECISA REMOVER PRECED LIST 26102012
            graph->vList[rem-1]->adjArrow = removeAdjList(graph->vList[rem-1]->adjArrow, graph);
        }

        free(graph->vList[rem-1]);
        graph->vList[rem-1]= NULL;
    }

    return graph;
}

typeEdgeAVL *copia(typeEdgeAVL *destino, typeEdgeAVL *fonte)
{
    destino->vAdj = fonte->vAdj; //troca troca->info e temp->info
 
    destino->pound = fonte->pound;
    destino->quality = fonte->quality;

    return destino;
}

void troca(typeEdgeAVL *noAux, typeEdgeAVL *temp)
{
    //es. auxiliar para efetuar a troca
    typeEdgeAVL *troca = (typeEdgeAVL*) malloc(sizeof(typeEdgeAVL));

    copia(troca, noAux);
    copia(noAux, temp);
    copia(temp, troca);

    free(troca);
    troca = NULL;
}

//precisa retornar as duas informacoes

typeEdgeAVL * trocaPonteiros2(typeEdgeAVL *fonte, typeEdgeAVL* destino)
{
   // typeEdgeAVL *copiaDest = destino;
    fonte->right = destino->right;
    fonte->left = destino->left;
    fonte->parent = destino->parent;
    destino = fonte;

    return destino;
}
//precisa manter os pais consistentes!
typeEdgeAVL *removeArvore(typeEdgeAVL *noAux, int info)
{
    typeEdgeAVL *temp ;

    //a cada iteracao

    if (noAux==NULL) // o no nao existe na arvore
        return NULL;

    else
    {
        if (noAux->vAdj > info)
            noAux->left = removeArvore(noAux->left, info); // continua na sub-árvore da leftuerda
   
        else
        {
            if (noAux->vAdj < info)
                noAux->right = removeArvore(noAux->right, info); // continua na sub-árvore da righteita
            else //noAux->info == info
            {
                if ((noAux->left != NULL) && (noAux->right!=NULL))// tem 2 filhos
                {
                    temp = maiorValor(noAux->left); // maior valor da sub-arvore da leftuerda
                    //temp é o PAI do maior elemento
                    //maior = temp->right

                    troca(noAux, temp);//
                    //Como temp é o maior valor da subArvore da esquerda, segue que
                    //TEMP É FOLHA!

                    // remocao a partir da sub arvore da esquerda,
                    //até encontrar a folha que guarda o vértice a ser removido
                    noAux->left = removeArvore(noAux->left, info);
                }
                else
                {
                    // filho a leftuerda, noAux->right ==NULL se nao cairia na condicao anterior
                    if ((noAux->left != NULL))
                    {
                        temp = noAux; //armazena o endereco de noAux, afim de remove-lo depois
                        noAux = noAux->left; //o no passa a ser ocupado pelo seu filho a esquerda
                        free(temp); //remove noAux antigo
                        temp = NULL;
                    }
                    else
                    {
                        if ( noAux->right!=NULL)// caso 2b: só filho a righteita
                        {
                            temp = noAux;
                            noAux = noAux->right; //noAux passa a ser ocupado pelo seu filho a direita

                            free(temp);
                            temp = NULL;
                        }
                        else //no folha
                        {
                            free(noAux); //libera
                            noAux = NULL;

                            return NULL; //retorna NULL para a retursividade

                            //o balanceamento sera tratado nos ancestrais desse no
                        }
                    }
                }
            }
        }
        noAux = balancear(noAux); //rotina de balanceamento no noh

        //obs: a recursividade cuida de conectar o novo noAux com seu respectivo pai
        //que eh o no que chamou a funcao

        return noAux;
    }
}



//sobe a arvore até chegar na raiz e vai balanceando
typeEdgeAVL *balancearAteRaiz(typeEdgeAVL *aux)//, typeEdgeAVL **pontAtual)
{
    while(aux != NULL)
    {
        aux->altura = calculaAltura(aux);
       // printf("aux= %d\n",aux->vAdj);
        //aux2 = balancear(aux);
        aux = balancear(aux);
      //  printf("aux= %d\t",aux->vAdj);
      //  printf("aux2= %d\n",aux2->vAdj);
        if(aux->parent!=NULL)
        {

            if(aux->parent->vAdj > aux->vAdj) aux->parent->left = aux;
            else aux->parent->right = aux;

            aux = aux->parent;
        }
        else
        {
            aux->parent = NULL;
            return aux;
        }
    }
    aux->parent =NULL;
    return aux;
}
///---------------------------------------------------------------------