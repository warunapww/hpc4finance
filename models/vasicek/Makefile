vasicek: DataExtracterO.o CalibrateO.o VasicekModelO.o VasicekTestO.o VasicekO.o DurationO.o LogsO.o
	mpicxx -O3 -Wall -L/usr/local/mysql/lib/mysql -lmysqlclient -lz DataExtracterO.o CalibrateO.o VasicekModelO.o VasicekTestO.o VasicekO.o DurationO.o LogsO.o -o vasicek
VasicekO.o: Vasicek.cpp Vasicek.h DataExtracter.h Calibrate.h VasicekModel.h def.h Logs.h duration.h
	mpicxx -O3 -Wall -I/home/cig4/mpich2-install/include -I/usr/local/mysql/include/mysql -c Vasicek.cpp -o VasicekO.o  
DataExtracterO.o: DataExtracter.cpp DataExtracter.h def.h
	g++ -O3 -Wall -I/usr/local/mysql/include/mysql -c DataExtracter.cpp -o DataExtracterO.o
CalibrateO.o: Calibrate.cpp Calibrate.h DataExtracter.h duration.h
	mpicxx -O3 -Wall -I/usr/local/mysql/include/mysql -I/home/cig4/mpich2-install/include -c Calibrate.cpp -o CalibrateO.o
DurationO.o: duration.cpp duration.h
	g++ -O3 -Wall -c duration.cpp -o DurationO.o
LogsO.o: Logs.h Logs.cpp duration.h
	g++ -O3 -Wall -c Logs.cpp -o LogsO.o
VasicekModelO.o: VasicekModel.cpp VasicekModel.h
	g++ -O3 -Wall -c VasicekModel.cpp -o VasicekModelO.o
VasicekTestO.o: VasicekTest.cpp Vasicek.h Calibrate.h DataExtracter.h duration.h 
	mpicxx -O3 -Wall -I/home/cig4/mpich2-install/include -c VasicekTest.cpp -o VasicekTestO.o
clean:
	rm -f *.o vasicek
