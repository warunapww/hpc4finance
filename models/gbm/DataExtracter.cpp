/*
 Copyright (c) 2008 by contributors:

 * Damitha Premadasa
 * Nilendra Weerasinghe
 * Thilina Dampahala
 * Waruna Ranasinghe - (http://warunapw.blogspot.com)

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
/*
// //
// // PROJECT  : HPC4Finance
// // MODULE   : DataExtracter class Implimentation
// // FILE     : DataExtracter.cpp
// // AUTHOR   : Damitha Premadasa (damitha38@gmail.com)
// // DESC     : The file consist of database related function in GBM. 
//	         This is satisfactorily tested with UNIX environment 
//               and WINDOWS environment.
// // TODO     : 
// // HISTORY  : Date of Creation: 5-Oct-2008
//               Modified:
// //
*/

#include "DataExtracter.h"


#ifdef WIN32
#include <windows.h>
#include <winsock.h>
#pragma warning (disable: 4514 4786)
#pragma warning( push, 3 )
#endif


#include "def.h"
#include <stdio.h>
#include <iostream>
#include <mysql.h>
#include <stdlib.h>
#include <list>


using namespace std;

DataExtracter::DataExtracter(char* pzDName, char* pzUserName, char* pzPW, char* pzTName, char* pzIP)
{
	pz_DBName = strdup(pzDName);
	pz_UserName = strdup(pzUserName);
	pz_Password = strdup(pzPW);
	pz_TableName = strdup(pzTName);
	pz_IPAddress = strdup(pzIP);
}


DataExtracter::~DataExtracter(void)
{
	if (pz_DBName)
		delete pz_DBName;
	if (pz_UserName)
		delete pz_UserName;
	if (pz_Password)
		delete pz_Password;
	if (pz_TableName)
		delete pz_TableName;
	if (pz_IPAddress)
		delete pz_IPAddress;
}

// Get required data set for parameter calculation
//vector<double> *DataExtracter::getData(char *coloumName)
double *DataExtracter::getData(char *coloumName, int &iLen)
{
//	vector<double> *pdData;
	double *pdData = NULL;
	MYSQL_RES *pmsResult;
	MYSQL_ROW msRow;

	MYSQL *pMysql = mysql_init(NULL);

	if(!mysql_real_connect(pMysql,pz_IPAddress,pz_UserName,pz_Password,pz_DBName,0,NULL,0))
	{		
		//printf("error,Incorrect Parameters\n");
		//fflush(stdout);
		return NULL;
	}
	else
	{
		//cout << coloumName << " : " << pz_TableName << endl;
		char query[250];
		sprintf(query,"SELECT %s FROM %s ORDER BY ID", coloumName ,pz_TableName );

		if(mysql_real_query(pMysql, query, strlen(query)))
		{
			//printf("error,Incorrect Parameters\n");
			//fflush(stdout);
			mysql_close(pMysql);
			return NULL;
		}
		else
		{
			pmsResult = mysql_store_result (pMysql);// DONE: check errors here
			int iRows = pmsResult->row_count;
			iLen = iRows;

			if( iRows == 0 )
			{
				mysql_close(pMysql);
				return NULL;
			}
			
//			pdData = new vector<double>;
			pdData = new double[iRows];

			int i =0;

			while ((msRow = mysql_fetch_row(pmsResult))) 
			{ 	
//				pdData->push_back(atof(*msRow));
				pdData[i] = atof(*msRow);
				i++;					
			}
			mysql_free_result(pmsResult);
		}
	}
	mysql_close(pMysql);

	return pdData;
}

// Get required data set for parameter calculation parallel
//vector<double> *DataExtracter::getDataParellel( int iStartingRow, int iEndingRow, char *coloumName )
double *DataExtracter::getDataParellel( int iStartingRow, int iEndingRow, char *coloumName )
{
//	vector<double> *pdData; 
	double *pdData = NULL;

	MYSQL_RES *pmsResult;
	MYSQL_ROW msRow;

	MYSQL *pMysql = mysql_init(NULL);

	if(!mysql_real_connect(pMysql,pz_IPAddress,pz_UserName,pz_Password,pz_DBName,0,NULL,0))
	{
		return 0;
	}
	else
	{
		if(mysql_select_db(pMysql, pz_DBName))
		{
			return 0;
		}
		
		else
		{
			char query[250];
			sprintf(query,"SELECT %s FROM %s where ID >= %d and ID <= %d ORDER BY ID", 
							coloumName,pz_TableName ,iStartingRow,iEndingRow);
			
			if(mysql_real_query(pMysql, query, strlen(query)))
			{

				return 0;
			}
			else
			{			
				pmsResult = mysql_store_result (pMysql);

				int iRows = pmsResult->row_count;
//				pdData = new vector<double>;
				pdData = new double[iRows];
				
				int i =0;

				while ((msRow = mysql_fetch_row(pmsResult))) 
				{ 	
//					pdData->push_back(atof(*msRow));
					pdData[i] = atof(*msRow);
					i++;
				}
				mysql_free_result(pmsResult);
			}
		}
	}
	mysql_close(pMysql);

	return pdData;
}

// Get the number of records in the table
int DataExtracter::getNoOfRows( void )
{
	int iRows; 

	MYSQL_RES *pmsResult;
	MYSQL *pMysql = mysql_init(NULL);

	if(!mysql_real_connect(pMysql,pz_IPAddress , pz_UserName, pz_Password ,pz_DBName,0,NULL,0))
	{
		return 0;
	}
	else
	{
		//const char* query = "SELECT Close FROM ibm";
		
		char query[250];
		sprintf(query, "SELECT * FROM %s", pz_TableName );	
		
		if(mysql_real_query(pMysql, query, strlen(query)))
		{
			mysql_close(pMysql);
			return 0;
		}
		else
		{
			pmsResult = mysql_store_result (pMysql);
			
			if(pmsResult != NULL)
			{
				iRows = pmsResult->row_count;
				
				if(iRows == 0)
				{
					return 0;
				}
				mysql_free_result(pmsResult);
			}
			else
			{
									
				if(mysql_errno(pMysql) == 0)
				{
					return 0;
				}
				else
				{
					return 0;	
				}
				
				return 0;
			}
		}
	}


	mysql_close(pMysql);

	return iRows;
}

// Get data from local machines independently
//vector<double> *DataExtracter::getReplicatedDataParellel( int iStartingRow, int iEndingRow, char *coloumName )
double *DataExtracter::getReplicatedDataParellel( int iStartingRow, int iEndingRow, char *coloumName )
{
//	vector<double> *pdData; 
	double *pdData = NULL;

	MYSQL_RES *pmsResult;
	MYSQL_ROW msRow;

	MYSQL *pMysql = mysql_init(NULL);

	if(!mysql_real_connect(pMysql,"localhost",pz_UserName,pz_Password,pz_DBName,0,NULL,0))
	{
		return 0;
	}
	else
	{
		if(mysql_select_db(pMysql, pz_DBName))
		{
			return 0;
		}
		else
		{	
			char query[250];
			sprintf(query,"SELECT %s FROM %s where ID >= %d and ID <= %d ORDER BY ID",coloumName, pz_TableName, 													iStartingRow,iEndingRow);	
			if(mysql_real_query(pMysql, query, strlen(query)))
			{
				return 0;
			}
			else
			{
				pmsResult = mysql_store_result (pMysql);

				int iRows = pmsResult->row_count;

			//	pdData = new vector<double>;
				pdData = new double[iRows];
				int i = 0;

				while ((msRow = mysql_fetch_row(pmsResult))) 
				{ 	
//					pdData->push_back(atof(*msRow));
					pdData[i] = atof(*msRow);
					i++;
				}
				mysql_free_result(pmsResult);
			}
		}
	}
	mysql_close(pMysql);

	return pdData;
}

// Get the last item of the table
double DataExtracter::getLastData(char* coloumName)
{
	MYSQL_RES *pmsResult;
	MYSQL_ROW msRow;

	double result;

	MYSQL *pMysql = mysql_init(NULL);

	if(!mysql_real_connect(pMysql,"localhost",pz_UserName,pz_Password,pz_DBName,0,NULL,0))
	{
		return 0;
	}
	else
	{
		char query[250];
		sprintf(query,"select %s from %s where ID = (select max(ID) from %s )", coloumName, pz_TableName, pz_TableName );
		//const char* query = "select Close from ibm where ID = (select max(ID) from ibm)";
		
		mysql_select_db(pMysql, pz_DBName );

		//int iQuery = mysql_real_query(pMysql, query, strlen(query));
		
		if(mysql_real_query(pMysql, query, strlen(query)))
		{
			return 0;
		}
		else
		{	
			pmsResult = mysql_store_result (pMysql);

			while ((msRow = mysql_fetch_row(pmsResult))) 
			{ 	
				result = atof(*msRow);
			}

			mysql_free_result(pmsResult);
		}
	}
	mysql_close(pMysql);

	return result;
}

// Get duration between first and last data element 
int DataExtracter::getDuration( void )
{
	MYSQL_RES *pmsResult;
	MYSQL_ROW msRow;

	string cStartDate, cEndDate;
	int iDateDifference;

	MYSQL *pMysql = mysql_init(NULL);

	if(!mysql_real_connect(pMysql,pz_IPAddress,pz_UserName,pz_Password,pz_DBName,0,NULL,0))
	{
		return 0;
	}
	else
	{
		char query1[250];
		sprintf(query1,"select Date from %s where ID = (select max(ID) from %s )", pz_TableName, pz_TableName );
		char query2[250];
		sprintf(query2,"select Date from %s where ID = (select min(ID) from %s)", pz_TableName , pz_TableName);
			
		if(mysql_select_db(pMysql, pz_DBName))
		{
			mysql_close(pMysql);
			return 0;
		}
		else
		{			
			if(mysql_real_query(pMysql, query1, strlen(query1)))
			{
				mysql_close(pMysql);
				return 0;
			}			
			else
			{			
				pmsResult = mysql_store_result (pMysql);

				while ((msRow = mysql_fetch_row(pmsResult))) 
				{ 	
						cEndDate = *msRow;
				}		
				mysql_free_result(pmsResult);
			}
						
			if(mysql_real_query(pMysql, query2, strlen(query2)))
			{
				mysql_close(pMysql);
				return 0;
			}			
			else
			{			
				pmsResult = mysql_store_result (pMysql);

				while ((msRow = mysql_fetch_row(pmsResult))) 
				{ 	
						cStartDate =  (*msRow);
				}		
				mysql_free_result(pmsResult);
			}
			
			char query[250];
			
			sprintf(query,"SELECT DATEDIFF( \'%s\' , \'%s\' )",cEndDate.c_str(),cStartDate.c_str());
					
			if(mysql_real_query(pMysql, query, strlen(query)))
			{
				mysql_close(pMysql);
				return 0;
			}			
			else
			{		
				pmsResult = mysql_store_result (pMysql);
				
				if(pmsResult == NULL)
				{
					mysql_close(pMysql);
					return 0;
				}
				else
				{				
					while ((msRow = mysql_fetch_row(pmsResult))) 
					{ 	
						iDateDifference = atoi(*msRow);
					}
				}		
				mysql_free_result(pmsResult);
			}

			
		}
	}

	mysql_close(pMysql);

	return iDateDifference;
}
