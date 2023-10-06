all: geodesics 

geodesics: main.o Geodesics.o KnuthBendix.o Thue.o
	g++ main.o Geodesics.o KnuthBendix.o Thue.o -g -I. -I.. -o geodesics -lm

main.o: main.cpp Geodesics.h
	g++ -Wall -DCAYLEY_GRAPH -g -O2 -I. -I..  -c main.cpp

Geodesics.o: Geodesics.cpp Geodesics.h
	g++ -Wall -DCAYLEY_GRAPH -g -O2 -I. -I..  -c Geodesics.cpp

KnuthBendix.o: KnuthBendix.cpp KnuthBendix.h
	g++ -Wall -DCAYLEY_GRAPH -g -O2 -I. -I..  -c KnuthBendix.cpp

Thue.o: Thue.cpp Thue.h
	g++ -Wall -DCAYLEY_GRAPH -g -O2 -I. -I..  -c Thue.cpp

clean:
	rm -rf *o geodesics *.core *.stackdump
