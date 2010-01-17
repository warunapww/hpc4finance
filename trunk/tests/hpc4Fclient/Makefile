all: HPC4FClient

HPC4FClient: HPC4FClient.cpp
	g++ -Wall HPC4FClient.cpp -o HPC4FClient
HPC4FClientT: HPC4FClientT.cpp
	g++ -Wall -lpthread HPC4FClientT.cpp -o HPC4FClientT
clean:
	rm -rf *.o HPC4FClient HPC4FClientT abort

abort: HPC4FClientTA.cpp
	g++ -Wall -lpthread HPC4FClientTA.cpp -o abort
