#ifndef DEFINES_H_INCLUDED
#define DEFINES_H_INCLUDED



//-----------------------------------

//UNWEIGHTED DIGRAPHS: POND 0
//WEIGHTED DIGRAPHS: POND 1
//#define POND 0 //for unweighted digraphs
//#define POND 1 //for weighted digraphs

//-----------------------------------



#define DEF_OVERLAPPING 0
#define DEF_SPEC 1
#define DEF_LOCAL_SEARCH 1

#define MEMORY_SIZE 10
#define PSPEC 0.3

#define PARALELO 0

#define ADICIONAL 1
#define TXTADICIONAL (const char *) ("")

#define EXPCONTRDEF 1

//REL = 0 (não gera relatório)
#define REL 1


#define FATREDUCAO (double)0.3

#define MINMELHORA 0.0000000000001
#define MAXITER 30

/*  SEED /in [ran{0,1]
    -1 to random SEED */

#define SEED (double)0.1 

#define PRECONTR 0
#define PR  0
#define CONTRDEF 1//1
#define FILTRO 0
#define PERTURBA 0

/** ELITESET  -----------------------------*/
#define TAMPR 5    //tamanho do pool do PR //caso do erdos : PR = 3

/** CONTRDEF ----------------------*/

#define INICIOCONTRDEF  15 //20 //15 //iter que a contr. definitiva inicia
#define INTERVCONTRDEF  3   //intervalo de iter. entre as contr. definitivas       3
#define PROBCONTRDEF 0.5 //prob. fixa de contrair um par


/**--------------------------------*/
#define ITERSEMMELHORA 3000

/**VALORES FIXOS ------------------ */
#define INF 999999
#define NEGINF -999999
#define TRUE 1
#define FALSE 0
#define MENORQ -9

#define PRINTALG FALSE
#define PRINTVERB TRUE

#define PERCLIMQUAL0 0


#endif // DEFINES_H_INCLUDED

