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

/**
Armazenamento do grafo (Vetor + AVL)

Implementação de vetor de vértices e AVL para lista de adjacências

v3 = estruturas mais simples
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "grafo.h"
#include "vetorAVL.h"
#include "estruturas.h"
#include "defines.h"


//f
typeGraph *leituraGrafo(int POND, const char * inputName)
{
    ///arquivos

    FILE *input,
        *timeRep;

    clock_t start,
            end;

    int nVert,
        labelX,
        labelY;

    double pound;

    typeGraph *grafo;

    int i;
    char c,
        text[10];


    input = fopen(inputName,"r");

    if(input!=NULL )
    {
        for(i=0;i<9;i++)
        {
            fscanf(input,"%c",&c);
        }

        fscanf(input,"%d",&nVert);

        grafo= createGraph(nVert);

        //procurar por *
        while(text[0]!='*')
        {
            fgets(text, 10, input);
        }

        start = clock();
        pound = 1;

        //Grafo ponderado
        if(POND == 1 )
        {
            while(fscanf(input,"%d %d %lf", &labelX, &labelY, &pound) == 3)
            {
                includeEdge(grafo, searchVertex(grafo->vList,labelX) , searchVertex(grafo->vList,labelY), pound);
            }
        }
        else
        {
            while(fscanf(input,"%d %d", &labelX, &labelY) == 2)
            {
                includeEdge(grafo, searchVertex(grafo->vList,labelX) , searchVertex(grafo->vList,labelY), pound);
            }
        }

        printf("total pound= %lf\n",grafo->m);
        end = clock();

        fclose(input);

       return grafo;
    }
    else{
        return NULL;
    }
}

