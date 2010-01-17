gbm: DataExtracterO.o CalibrateO.o DurationO.o GBMTestO.o GBMO.o GBMotionO.o LogsO.o
	mpicxx -O3 -ffast-math -Wall -L/usr/local/mysql/lib/mysql -lmysqlclient -lz DataExtracterO.o CalibrateO.o DurationO.o LogsO.o GBMMotionO.o GBMO.o GBMTestO.o -o gbm
DataExtracterO.o: DataExtracter.cpp DataExtracter.h def.h 
	g++ -O3 -ffast-math -Wall -I/usr/local/mysql/include/mysql -c DataExtracter.cpp -o DataExtracterO.o
CalibrateO.o: Calibrate.cpp Calibrate.h DataExtracter.h duration.h
	mpicxx -O3 -ffast-math -Wall -I/usr/local/mysql/include/mysql -I/home/cig4/mpich2-install/include -c Calibrate.cpp -o CalibrateO.o
DurationO.o: duration.cpp duration.h
	g++ -O3 -ffast-math -Wall -c duration.cpp -o DurationO.o
LogsO.o: Logs.h Logs.cpp duration.h
	g++ -O3 -ffast-math -Wall -c Logs.cpp -o LogsO.o
GBMotionO.o: GBMotion.cpp GBMotion.h duration.h
	g++ -O3 -ffast-math -Wall -c GBMotion.cpp -o GBMMotionO.o
GBMO.o: GBM.cpp GBM.h DataExtracter.h Calibrate.h GBMotion.h def.h Logs.h
	mpicxx -O3 -ffast-math -Wall -I/home/cig4/mpich2-install/include -I/usr/local/mysql/include/mysql -c GBM.cpp -o GBMO.o  
GBMTestO.o: GBMTest.cpp GBM.h def.h duration.h 
	g++ -O3 -ffast-math -Wall -c GBMTest.cpp -o GBMTestO.o
clean:
	rm *.o gbm
