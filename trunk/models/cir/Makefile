cir: DataExtracterO.o CalibrateO.o CIRModelO.o CIRTestO.o CIRO.o newran1.o newran2.o myexcept.o simpstr.o extreal.o DurationO.o LogsO.o
	mpicxx -O3 -Wall -L/usr/local/mysql/lib/mysql -lmysqlclient -lz DataExtracterO.o CalibrateO.o CIRModelO.o CIRTestO.o CIRO.o newran1.o newran2.o myexcept.o simpstr.o extreal.o DurationO.o LogsO.o -o cir
CIRO.o: CIR.cpp CIR.h DataExtracter.h Calibrate.h CIRModel.h def.h Logs.h
	mpicxx -O3 -Wall -I/home/cig4/mpich2-install/include -I/home/cig4/Desktop/newran03 -c CIR.cpp -o CIRO.o
DataExtracterO.o: DataExtracter.cpp DataExtracter.h def.h
	g++ -O3 -Wall -I/usr/local/mysql/include/mysql -c DataExtracter.cpp -o DataExtracterO.o
CalibrateO.o: Calibrate.cpp Calibrate.h DataExtracter.h duration.h
	mpicxx -O3 -Wall -I/usr/local/mysql/include/mysql -I/home/cig4/mpich2-install/include -c Calibrate.cpp -o CalibrateO.o
DurationO.o: duration.cpp duration.h
	g++ -O3 -Wall -c duration.cpp -o DurationO.o
LogsO.o: Logs.h Logs.cpp duration.h
	g++ -O3 -Wall -c Logs.cpp -o LogsO.o
CIRModelO.o: CIRModel.cpp CIRModel.h duration.h
	g++ -O3 -Wall -I/home/cig4/Desktop/newran03 -c CIRModel.cpp -o CIRModelO.o
CIRTestO.o: CIRTest.cpp CIR.h Calibrate.h DataExtracter.h duration.h
	mpicxx -O3 -Wall -I/home/cig4/mpich2-install/include -c CIRTest.cpp -o CIRTestO.o
	
clean: 
	rm DataExtracterO.o CalibrateO.o CIRModelO.o CIRTestO.o CIRO.o DurationO.o LogsO.o cir
