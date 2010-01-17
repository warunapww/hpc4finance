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
// // MODULE   : HPC4FServer which runs in Master Node
// // FILE     : tpool.h
// // AUTHOR   : Waruna Ranasinghe (warunapww@gmail.com)
// // DESC     : This header file defines the Tpool class which is used to create thread pools
// // TODO     : 
// // HISTORY  : Date of Creation: 20-Nov-2008
//               Modified: 14-Jan-2009 : added GetNextReferenceID
// //
*/

#ifndef _TPOOL_H_
#define _TPOOL_H_

#include <stdio.h>
#include <pthread.h>

typedef struct stRequest 
{
	void (*routine)(void *);	
	void *arg;	
 	struct stRequest *next;	
} tstRequest;	

class Tpool
{
public:
	int *piReferences;
        unsigned int i_NoThreads;	
	bool b_QueueClosed;	
        unsigned int i_MaxQueueSize;	
	bool b_DoNotBlock;	
	bool b_Shutdown;	
	/* pool state */	
	pthread_mutex_t mutex_QueueLock;	
	pthread_cond_t  cond_QueueNotEmpty;	
	pthread_cond_t  cond_QueueNotFull;	
	pthread_cond_t  cond_QueueEmpty;	
	pthread_t *p_threads;	
	int i_CurQueueSize;	
	tstRequest *pst_ReqQueueHead;	
	tstRequest *pst_ReqQueueTail;	

	Tpool(int iNoThreads, int iMaxQueueSize, bool bDoNotBlock);
	~Tpool();
	int AddWork(void *routine, void *arg);	
	int Destroy(int iFinish);
	int GetNextReferenceID();
};	
		
//void *tpool_thread(void *tpool_);

#endif //_TPOOL_H_
