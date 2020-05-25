#ifndef MAXLIST_H_INCLUDED
#define MAXLIST_H_INCLUDED

#include "estruturas.h"
#include "vetorAVL.h"
#include "coarsening.h"
#include "defines.h"
#include "dendrogramaVet.h"


typeElemMaxList* removeMaxListElem(typeMaxList *maxList, typeElemMaxList *elem); //f

//f
typeMaxList* destroiMaxList(typeMaxList *maxList);

//NAO USADO OK
void printIMaxList(typeMaxList *maxList);


#endif // MAXLIST_H_INCLUDED
