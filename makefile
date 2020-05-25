all: ConClus_release
    
ConClus_release: obj/bestNeigh.o obj/coarsening.o obj/dendogramaVet.o obj/grafo.o obj/leituraGrafo.o obj/main.o obj/maxList.o obj/buscaLocal.o obj/memoria.o obj/particao.o obj/clusteringList.o obj/preProcessamento.o obj/vetorAVL.o sources/defines.h
		g++ -O3 -o ConClus_release.out obj/bestNeigh.o obj/coarsening.o obj/dendogramaVet.o obj/grafo.o obj/leituraGrafo.o obj/main.o obj/maxList.o obj/buscaLocal.o obj/memoria.o obj/particao.o obj/clusteringList.o obj/preProcessamento.o obj/vetorAVL.o -I$(AREIG_DIR) -I$(CSCMAT_DIR) $(SUPERLU_LIB) $(ALL_LIBS) -std=c++11 -lm 

obj/bestNeigh.o: sources/bestNeigh.c sources/bestNeigh.h
	g++ -O3 -o obj/bestNeigh.o -c sources/bestNeigh.c -std=c++11 -lm 

obj/buscaLocal.o: sources/buscaLocal.c sources/buscaLocal.h
	g++ -O3 -o obj/buscaLocal.o -c sources/buscaLocal.c -std=c++11 -lm 

obj/coarsening.o: sources/coarsening.cpp sources/coarsening.h sources/coarsening.h 
	g++ -O3 -o obj/coarsening.o -c sources/coarsening.cpp -std=c++11 -lm 		


obj/dendogramaVet.o: sources/dendogramaVet.c sources/dendrogramaVet.h
	g++ -O3 -o obj/dendogramaVet.o -c sources/dendogramaVet.c -std=c++11 -lm 		

obj/grafo.o: sources/grafo.c sources/grafo.h
	g++ -O3 -o obj/grafo.o -c sources/grafo.c -std=c++11 -lm 		

obj/main.o: sources/main.c sources/defines.h sources/coarsening.h
	g++ -O3 -o obj/main.o -c sources/main.c -std=c++11 -lm 		

obj/leituraGrafo.o: sources/leituraGrafo.c
	g++ -O3 -o obj/leituraGrafo.o -c sources/leituraGrafo.c -std=c++11 -lm 		

obj/maxList.o: sources/maxList.c sources/maxList.h
	g++ -O3 -o obj/maxList.o -c sources/maxList.c -std=c++11 -lm 		

obj/memoria.o: sources/memoria.cpp sources/memoria.h
	g++ -O3 -o obj/memoria.o -c sources/memoria.cpp -std=c++11 -lm 	

obj/particao.o: sources/particao.c sources/particao.h
	g++ -O3 -o obj/particao.o -c sources/particao.c -std=c++11 -lm 	

obj/clusteringList.o: sources/clusteringList.c sources/clusteringList.h
	g++ -O3 -o obj/clusteringList.o -c sources/clusteringList.c -std=c++11 -lm 

obj/preProcessamento.o: sources/preProcessamento.c sources/preProcessamento.h
	g++ -O3 -o obj/preProcessamento.o -c sources/preProcessamento.c -std=c++11 -lm 	

obj/vetorAVL.o: sources/vetorAVL.c sources/vetorAVL.h
	g++ -O3 -o obj/vetorAVL.o -c sources/vetorAVL.c -std=c++11 -lm 	


.PHONY:	clean
clean:
	rm -f *~ *.o *g.out ConClus_release/*.o obj/*.o

# defining pattern rules.
%.o:	%.cc
	$(CPP) -I$(AREIG_DIR) -I$(CSCMAT_DIR) -I$(SUPERLU_DIR) $<
