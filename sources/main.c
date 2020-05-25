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

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "../fontes_ref/expansao_base.h" //OP REFINEMENT
#include "vetorAVL.h"
#include "estruturas.h"
#include "coarsening.h"
#include "grafo.h"
#include "dendrogramaVet.h"
#include "preProcessamento.h"
//#include "clusteringList.h"
#include "memoria.h"


typeGraph* resetaNovaIteracao(typeGraph* grafo, typeGraph* grafoOrig , typeDendr* dendr , int *lastMergeNv, typeParticao** particao,
                              typeNeigh** bestNeigh ,typeNeigh**  bestNeighOrig, typeMove **melhorMove );
char *defineInputName(int numGrafo1, int numGrafo2);
char *defineResName();
char *defineResNameLambdas(int numGrafo1, int numGrafo2);
char *defineInputNormal();
char *defineResNormalLambdas();


typeGraph *leituraGrafo(int POND, const char * inputname);

char* GRAFONAME;
char* GRAFOCAMINHO;
char* RESCAMINHO;

double MENORLAMBDA;
double MAIORLAMBDA;

int main(int argc,char *argv[]) {
    
    /** INICIO DECLARACAO VARIAVEIS */
    int NUM_OV = 8;
    int* numGroupsSpec=(int*) calloc(sizeof(int),1);
    *numGroupsSpec=-1;

    /*** GRAPH NAME AND RES DIR AS PARAMETER ***/    
    GRAFOCAMINHO = (char*) calloc(sizeof(char),400);


	GRAFOCAMINHO = argv[1];
	printf("Folder_input: %s\n", GRAFOCAMINHO);


    GRAFONAME = argv[2];    
    RESCAMINHO = argv[3];

    MENORLAMBDA = atoi(argv[4]);
    MAIORLAMBDA = atoi(argv[4]);

    int POND = atoi(argv[5]);

    int OP_CODIGO = 0;
    int OP_OV = 0;

    int OP_REF=0; 
    int OP_SPEC=0;

	int PAR_SPEC_SIZE=0;
  
    printf("File_input: %s\n", GRAFONAME);

    int i, iExt, qtdClusters, maxNiveis;
    typeParticao *particao = NULL,
                  *maxParticao = NULL;
    int *maxMapa = NULL;
    int *mapaAnt1 = NULL,
         *mapaAnt2 = NULL;
    int *lastMergeNv = NULL;
    typeGraph   *grafo = NULL,
                 *graphOrig = NULL,
                  *conf = NULL;
    typeDendr* dendr = NULL;
    typeNeigh   **bestNeighOrig= NULL ,
                  **bestNeigh = NULL;
    typeMove **melhorMove = NULL;
    typeSinglVList *listSinglV = NULL;
    typeEliteSet *eliteSet = NULL;

    FILE    *consoleContr = NULL ,
             *relatorioRes = NULL;

    double menorSol = -1.0 * MENORQ;
    double melhorSol = MENORQ;
    double media = 0;

    double menorLacoExt =2 ,
           melhorLacoExt = -2,
           mediaLacoExt = 0,
           tMedioLacoExt = 0;
    int ultimaMelhora = -1;
    char *resName = NULL;
    char *inputName = NULL;
    double tTotal = 0 , tMelhorSol = 0;
    clock_t start , end , endMelhorSol;
    int itMelhorSol = 0;
    int th_id;
    double gapAtual = 0 , gapAnt = 0, solConstr, numAceitas;
    int numGrafo1 = 1 , numGrafo2 = 3;
    double lambdaIni = 2.6;
    double numGrafo1Ini = 8 ;
    /** FIM DECLARACAO VARIAVEIS */

    int** memory = NULL;
    int* memory_last_index = NULL;
    int** memory_max = NULL;
    int* memory_max_last_index = NULL;

    int v;

    inputName = defineInputNormal();
    printf("input = %s\n", inputName);
    resName = defineResNormalLambdas();
    char *resPName = (char*) calloc(sizeof(char),400);

    double LAMBDA_ML=0;
    
    for(LAMBDA_ML = MENORLAMBDA; LAMBDA_ML <= MAIORLAMBDA+0.05 ; LAMBDA_ML+=0.1) {
        printf("Lambda=%lf\n",LAMBDA_ML);
        strcpy( resPName , "");
        sprintf ( resPName, "%spart_%s_%lf.com", RESCAMINHO , GRAFONAME, LAMBDA_ML);

        menorSol = 2;
        melhorSol = -2;
        media = 0;
        ultimaMelhora = -1;
        tTotal = 0 , tMelhorSol = 0;
        clock_t start , end , endMelhorSol;
        itMelhorSol = 0;

        /**Starting counting time for this lambda  ------------------------------ */
        start = clock();

        graphOrig =  leituraGrafo(POND, inputName); //DIRECTED GRAPH

        if(graphOrig!=NULL) {            
            graphOrig->LAMBDA_ML = LAMBDA_ML;
            conf = copyGraph(graphOrig);

            maxNiveis = calcMaxNiveis(graphOrig, FATREDUCAO);
            
            maxMapa =  inicializaMapaInt(graphOrig->nOrig);
            //a lista é sempre igual para todas as fases construtivas
            listSinglV = procSinglVertices(graphOrig);

            //Calculate initial  modularity
            graphOrig->modularity = calcGraphModularitySingl(graphOrig);

            dendr = createDendr(maxNiveis, FATREDUCAO, graphOrig->nOrig);
            lastMergeNv = createLastMergeNv( graphOrig->nOrig);

            /** ---- contração definitiva 140313 ---- */
            dendr->beginGraphNv = insertBeginGraphNv(dendr->beginGraphNv, dendr->nMerge);
        
            bestNeighOrig = createBestNeigh(graphOrig);
            bestNeigh = copyBestNeighNew(bestNeighOrig , graphOrig->nOrig);

            eliteSet = inicializaEliteSet(TAMPR);      
            
            /** GRASP ----------------------------------------------------------------------------  */
            srand(time(NULL));
            //para se não encontrar uma solução melhor em 8 iterações
	        printf("MaxIter=%d, seed=%lf\n",MAXITER, SEED);
            for(i = 0; i< MAXITER && ( i - itMelhorSol) <= ITERSEMMELHORA ; i++) {
                printf("------ Iteration %d ---------\n",i);
                grafo = copyGraph(graphOrig); 

                dendr = coarsening(grafo, FATREDUCAO,   maxNiveis, bestNeigh, dendr, lastMergeNv, OP_SPEC, resPName, numGroupsSpec, PAR_SPEC_SIZE);//
                solConstr = grafo->modularity;

                int* partInt = particaoTradicional(grafo, dendr, dendr->beginGraphNv, 1000);
                int ind;

                #if PRINTALG == TRUE
                    for(ind=0; ind<grafo->nOrig; ind++){
                        printf("%d, ",partInt[ind]);
                    }
                    printf("\n");
                #endif

                particao = criaParticaoFromInt(partInt,grafo);

                printf("Mod after coarsening=%lf\n",grafo->modularity);             
                printf("confere Q = %lf\n", calculaModularidadeGrafo(conf , particao));
                printf("confere2 Q = %lf\n\n", calculaModularidadeGrafoInt(conf , partInt));                
                
                double mod_after_coarsening=grafo->modularity;
                
                free(partInt);
                partInt = NULL;
    
                incluiEliteSet(eliteSet , particao, grafo);
                
                media += grafo->modularity;

                if( getMelhorEliteSet(eliteSet)->mod > melhorSol ) {
                    melhorSol =getMelhorEliteSet(eliteSet)->mod;
                    qtdClusters = particao->clustering->qtd;
                    endMelhorSol = clock();
                    itMelhorSol = i;
                }
                if(grafo->modularity < menorSol) {
                    menorSol = grafo->modularity;
                }

                grafo = resetaNovaIteracao( grafo, graphOrig , dendr , lastMergeNv, &particao, bestNeigh , bestNeighOrig , melhorMove);
                
                /** -------------------------------------------------------------------------------------  */
                /** Contrai os pares que estavam nos mesmos clusters nas ultimas 3 iterações ------------  */
                /** -------------------------------------------------------------------------------------  */
                //não deixa fazer na última iteração
                if( (i+1)!= MAXITER) {
                    printf("............\n");
                    if(CONTRDEF == TRUE  && i > INICIOCONTRDEF  && (i+1)%INTERVCONTRDEF == 0 ) {
                        contraiParesRecorrentes5EliteSet(eliteSet , graphOrig, bestNeighOrig, dendr, lastMergeNv);
                        printf("contract recurrent ok\n"); //APAGAR RELEASE
                    }
                }

                bestNeigh = copyBestNeighDest(bestNeighOrig, bestNeigh, graphOrig->nOrig);
                grafo = destroiGrafo(grafo);

                /**--------------------------------------------------------------------------------------------*/

            }//fim grasp
    
            maxMapa = copiaMapaDestinoInt( eliteSet->lista[eliteSet->indMelhor]->mapa , maxMapa, graphOrig->nOrig);

            //os vértices desconexos que foram removidos inicialmente são adicionados ao seu próprio cluster na maxMapa
            maxMapa = recuperaSinglVMaxMapa(listSinglV, maxMapa);
            melhorSol = getMelhorEliteSet(eliteSet)->mod;

            /** Fim contagem de tempo ----------------------------------------*/
            end = clock();

            /** RELATÓRIO ---------------------------------------------- */

            tTotal = (double)(end-start)/CLOCKS_PER_SEC;
            tMelhorSol = (double)(endMelhorSol - start)/CLOCKS_PER_SEC;

            media = media/(i+1);


            printf("totalw = %lf,%lf",conf->m,graphOrig->m);
            printf("input = %s\n", inputName);
            printf("Tempo tTotal = %lf\n", tTotal);                      
            printf("com o (graph->LAMBDA_ML=%lf) = %lf\n", graphOrig->LAMBDA_ML, getMelhorEliteSet(eliteSet)->mod);
            printf("..conf_lamb=%lf\n",conf->LAMBDA_ML);
            printf("confere Q = %lf\n", calculaModularidadeGrafo(conf , getMelhorEliteSet(eliteSet)));
            printf("confere2 Q = %lf\n\n", calculaModularidadeGrafoInt(conf , maxMapa));

            relatorioRes = fopen(  resName , "a");
            printf("RES: %s\n", resName);            
	        if(relatorioRes!=NULL && REL == 1) {            
                fprintf(relatorioRes, "graph->LAMBDA_ML = %lf\tSolucao = %lf\nTempo = %lf\n", graphOrig->LAMBDA_ML , getMelhorEliteSet(eliteSet)->mod, tTotal);            
                fclose(relatorioRes);                
            }

            char *resNameMod = (char*) calloc(sizeof(char),400); //011118
            sprintf ( resNameMod, "%smod_%s.txt", RESCAMINHO , GRAFONAME);
            printf("MODULARITY IN %s\n", resNameMod);
             relatorioRes = fopen(  resNameMod , "a");
            printf("RES MOD: %s\n", resNameMod);            
            if(relatorioRes!=NULL && REL == 1) {            
                fprintf(relatorioRes, "%lf,%lf\n", getMelhorEliteSet(eliteSet)->mod, tTotal);            
                fclose(relatorioRes);                
            }


            printf("RES_PART: %s\n", resPName);
            printParticaoIntFileName(maxMapa, graphOrig->nOrig , resPName);     

            
            /** LIMPEZA ----------------------------------------------*/
            bestNeigh = destroiBestNeigh(bestNeigh, graphOrig->nOrig);
            bestNeighOrig = destroiBestNeigh(bestNeighOrig, graphOrig->nOrig);
            dendr = destroiDendr(dendr, maxNiveis);
            listSinglV= destroiSinglVList(listSinglV);
            grafo = destroiGrafo(grafo);
            graphOrig = destroiGrafo(graphOrig);
            conf = destroiGrafo(conf);

            free(maxMapa);
            maxMapa = NULL;

            free(lastMergeNv);
            lastMergeNv = NULL;

            eliteSet = destroiEliteSet(eliteSet);

            //fim de cada execucao
            tMedioLacoExt += tTotal;
            mediaLacoExt += melhorSol;

            if(melhorSol  > melhorLacoExt) {
                melhorLacoExt = melhorSol;
            }
            if(melhorSol <  menorLacoExt) {
                menorLacoExt = melhorSol;
            }
            printf("\n");
        } // fim graphOrig!=NULL


    } //fim laço externo
	
    free(inputName);
    inputName = NULL;

    free(resName);
    resName = NULL;

    free(resPName);
    resPName = NULL;

    return 0;
}

//f
typeDendr* resetaDendr(typeDendr *dendr) {
    int limInfL,l,j;
    typeBeginGraphNv *aux = NULL;
    //nivel 1 são as contrações definitivas

    limInfL = 1;

    for(l = dendr->nNv ; l > limInfL ; l-- ) {
        for(j = dendr->nMerge- 1 ; j>= dendr->beginGraphNv->i; j--) {
            if(l != 1 ) {
                //libera a lista de arestas modif.
                dendr->list[j]->changeEdgeList =destroiChangedList( dendr->list[j]->changeEdgeList);
                dendr->list[j]->changeEdgeList = NULL;

                //191118:
                dendr->list[j]->changeEdgeArrow =destroiChangedList( dendr->list[j]->changeEdgeArrow);
                dendr->list[j]->changeEdgeArrow = NULL;


                free(dendr->list[dendr->nMerge-1]);
                dendr->list[dendr->nMerge-1]=NULL;

                dendr->nMerge--;
            }

        }

        aux = dendr->beginGraphNv->ant;
        free(dendr->beginGraphNv);
        dendr->beginGraphNv = aux;


    }
    return dendr;
}

typeGraph* resetaNovaIteracao(typeGraph* grafo, typeGraph* grafoOrig , typeDendr* dendr , int *lastMergeNv, typeParticao** particao,
                              typeNeigh** bestNeigh ,typeNeigh**  bestNeighOrig, typeMove **melhorMove ) {
    int i ;


    for(i=0 ; i<grafo->nOrig ; i++) {
        lastMergeNv[i] = -1;
    }

    //apaga tudo, menos as definitivas
    dendr = resetaDendr(dendr);

    //seta o lastMergeNv das definitivas...
    for(i=0; i<dendr->nMerge ; i++) {
        //é para guardar a posição!
        lastMergeNv[  dendr->list[i]->label1 - 1]  = i;
        lastMergeNv[  dendr->list[i]->label2 - 1]  = i;
    }

    *particao = destroiParticao(*particao);
    return grafo;
}

//f
char *defineInputNormal() {
    char *inputName = (char*) calloc(sizeof(char),400);

    inputName = strcpy( inputName , GRAFOCAMINHO );
    inputName = strcat( inputName , GRAFONAME);

    inputName = strcat( inputName , ".net");

    return inputName;

}

//f
char *defineResNormalLambdas() {
    char* resName = (char*) calloc(sizeof(char),400);
    sprintf( resName, "%ssolucao_%s.txt" , RESCAMINHO, GRAFONAME );

    return resName;
}