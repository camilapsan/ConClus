#ifndef ESTRUTURAS_H_INCLUDED
#define ESTRUTURAS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <time.h>


/*----------------------------------------------------------*/
///VETOR + AVL
///armazena o grafo temporário
struct vertex
{
    //lista de ganhos
    int label;
    struct edgeAVL *adjList;    //uma lista de vértices adjacentes para cada vértice da lista
    struct edgeAVL *adjArrow;    //DIRECTED GRAPH 010115: uma lista de vértices que tem o vértice como adjacente (
    double contribuicao;        //contribuicao do vértice no seu cluster atual
};
typedef struct vertex typeVertex;

struct edgeAVL
{
    double pound;
    double quality;
    int vAdj;
    struct edgeAVL *right;
    struct edgeAVL *left;
    struct edgeAVL *parent; //120912
    int altura;
};
typedef struct edgeAVL typeEdgeAVL;


struct graph
{
    typeVertex **vList;
    int nVert;
    int nOrig;
    double m;
    double *vDegree;
    double *vDegreeIn;
    double *vDegreeArrow;
    double modularity;

    double LAMBDA_ML;

    int nDesconex; //200113
};
typedef struct graph typeGraph;


/*----------------------------------------------------------*/

struct neigh
{
    int label2;

    double quality; //duplicado, pensar em como nao armazenar

};
typedef struct neigh typeNeigh;


/*----------------------------------------------------------*/

/* 27/10/2012 , mapeamento de arestas que foram modificadas na contração*/
struct kChanged
{
    struct kChanged *ant;
    //int *vPreced;

    int kChanged;
    double peso;

};
typedef struct kChanged typeKChanged;


struct elemDendr
{
    int label1;
    int label2;

    int col1;
    int col2;

    struct kChanged *changeEdgeList;

    //191118
    struct kChanged *changeEdgeArrow;
};
typedef struct elemDendr typeElemDendr;


struct dendr
{
   typeElemDendr **list;
    //onde começa cada nível do grafo
    struct beginNv *beginGraphNv;
    //número de merges
    int nMerge;
    //quantidade de niveis
    int nNv;


};
typedef struct dendr typeDendr;

//pilha, onde inicia o nível i?
struct beginNv
{
    int i;
    struct beginNv *ant;
};
typedef struct beginNv typeBeginGraphNv;


/*----------------------------------------------------------*/

struct elemMax
{
    //i guarda o indice i da estrut. bestNeigh
    int i;
    struct elemMax *next;
    struct elemMax *prev;

};
typedef struct elemMax typeElemMaxList;

//lista duplamente encadeada
struct maxList
{
    struct  elemMax *first;
    struct  elemMax *last;
    int size;
    struct move **melhorMove;

};
typedef struct maxList typeMaxList;


struct move
{
    struct cluster *cluster;

    double quality;  //duplicado, pensar em como nao armazenar

};
typedef struct move typeMove;


/*----------------------------------------------------------*/
struct singleVertex
{
    int label;
    struct singleVertex *next;
};
typedef struct singleVertex typeSinglV;

struct listSingleVertex
{
    struct singleVertex *first;
    struct singleVertex *last;

};
typedef struct listSingleVertex typeSinglVList;


/*----------------------------------------------------------*/

struct particao
{
    struct cluster **mapa;
    struct clusteringList *clustering;
    //precisa atualizar esse valor pelo do grafo toda hora
    double mod;
};
typedef struct particao typeParticao;

struct elemCluster
{
    int vert;
    struct elemCluster *prox;
};
typedef struct elemCluster typeElemCluster;

struct cluster
{
    int indice;
    struct elemCluster *prim;   //armazena a raiz do primeiro elem
    struct elemCluster *ult;    // ...               último
    int qtd;                    //quantidade de elems

    //261212
    struct cluster *corresp;    //cluster do Clustering2 correspondente
    int dist;                  //distancia dele

//posso usar como auxiliar e também guardar o gnaho


    struct cluster *prox;
};
typedef struct cluster typeCluster;

struct clusteringList
{
    struct cluster *prim;       //armazena a raiz do primeiro cluster
    struct cluster *ult;        // ...               último
    int qtd;
    int qtdIsolados;
};
typedef struct clusteringList typeClustering;



/*----------------------------------------------------------*/

//guarda várias partições
//boas soluções, diferentes entre si
//propor uma medida para avaliar a diferença?


struct elite
{
    struct particao **lista;
    int max;
    int qtd;
    int indMelhor;

};
typedef struct elite typeEliteSet;


struct particaoList
{
    struct particao *prim;
    struct particao *ult;
    int qtd;

};
typedef struct particaoList typeParticaoList;


struct transfCluster{
    struct cluster *cluster2;
    double ganho;
};
typedef struct transfCluster typeTransfCluster;

#endif // ESTRUTURAS_H_INCLUDED
