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

#include "maxList.h"

//NAO USADO CONF
typeElemMaxList* removeMaxListElem(typeMaxList *maxList, typeElemMaxList *elem)
{
    typeElemMaxList *aux = maxList->first;

    //enquanto a qualidade do elemento que procuro for menor que a do atual
    //precisa continuar procurando
   // printf("i=%d, first=%d\n",i, maxList->first->i);
    //remoção do primeiro
    if(maxList->first != NULL)
    {

        if( elem == maxList->first)
        {
         //   printf("rem first");
            aux = maxList->first;
            if(maxList->first->next != NULL)
            {
                maxList->first = maxList->first->next;
                maxList->first->prev = NULL;
            }
            else
            {
                maxList->first = NULL;
                maxList->last = NULL;
            }

        }
        else
        {
            /** Assert: Aux!= NULL */
            //remove ultimo
            if(aux == maxList->last)
            {
                maxList->last = maxList->last->prev;
                aux->prev->next = NULL;
            }
            //intermediario
            else
            {
                aux->prev->next = aux->next;
                aux->next->prev = aux->prev;
            }

        }
        free(aux);
        aux = NULL;


           //retorna o que acabou de ser removido

        //ou achou ou não existe!
    }
    return aux;
}

//f
typeMaxList* destroiMaxList(typeMaxList *maxList)
{
    typeElemMaxList *aux1 = maxList->first;
    typeElemMaxList *aux2 = NULL;
    while(aux1!=NULL)
    {
        ///ponteiro aux1->prox ainda existe?
        aux2 = aux1->next;
        aux1->next = NULL;
        free(aux1);
        aux1=NULL;
        aux1 = aux2;
    }

    maxList->first = NULL;
    maxList->last = NULL;

    free(maxList);
    maxList = NULL;

    return maxList;
}

//NAO USADO OK
void printIMaxList(typeMaxList *maxList)
{
    typeElemMaxList *elem = maxList->first;
    printf("Índices Max List\n");
    while(elem!= NULL)
    {
        printf("i = %d\n", elem->i);
        elem = elem->next;
    }
}
