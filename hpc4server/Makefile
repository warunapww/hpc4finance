all: HPC4FServer

HPC4FServer: TestHPC4FServer.o HPC4FServer.o RequestHandler.o tpool.o
	g++ -g -lpthread TestHPC4FServer.o HPC4FServer.o RequestHandler.o tpool.o -o HPC4FServer

TestHPC4FServer.o: TestHPC4FServer.cpp HPC4FServer.h
	g++ -g -c TestHPC4FServer.cpp

HPC4FServer.o: HPC4FServer.cpp HPC4FServer.h RequestHandler.h defs.h tpool.h
	g++ -g -c HPC4FServer.cpp

RequestHandler.o: RequestHandler.cpp RequestHandler.h defs.h
	g++ -g -c RequestHandler.cpp

tpool.o: tpool.h tpool.cpp RequestHandler.h
	g++ -g -c tpool.cpp -o tpool.o
clean:
	rm -rf *o HPC4FServer HPC4FServerS

HPC4FServerS: TestHPC4FServerS.o HPC4FServerS.o RequestHandlerS.o tpoolS.o
	g++ -O3 -lpthread TestHPC4FServerS.o HPC4FServerS.o RequestHandlerS.o tpoolS.o -o HPC4FServerS

TestHPC4FServerS.o: TestHPC4FServer.cpp HPC4FServer.h
	g++ -O3 -c TestHPC4FServer.cpp -o TestHPC4FServerS.o

HPC4FServerS.o: HPC4FServer.cpp HPC4FServer.h RequestHandler.h defs.h tpool.h
	g++ -O3 -c HPC4FServer.cpp -o HPC4FServerS.o

RequestHandlerS.o: RequestHandler.cpp RequestHandler.h defs.h
	g++ -O3 -c RequestHandler.cpp -o RequestHandlerS.o

tpoolS.o: tpool.h tpool.cpp RequestHandler.h
	g++ -O3 -c tpool.cpp -o tpoolS.o
cleanS:
	rm -rf *o HPC4FServerS
