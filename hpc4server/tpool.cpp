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
// // FILE     : tpool.cpp
// // AUTHOR   : Waruna Ranasinghe (warunapww@gmail.com)
// // DESC     : This file implements the Tpool class which is used to create thread pools and
// 		 the tpoll_thread function which is run by the threads.
// // TODO     : 
// // HISTORY  : Date of Creation: 20-Nov-2008
//               Modified: 14-Jan-2009 : added GetNextReferenceID
// //
*/

#include "tpool.h"
#include <stdlib.h>
#include <string.h>
#include "RequestHandler.h"

//**********************************************************************************************************
void *tpool_thread(void *tpool_)	
{	
	Tpool *tpool = (Tpool *)tpool_;
	tstRequest *my_workp;	
	for (;;) 
	{	
		pthread_mutex_lock(&(tpool->mutex_QueueLock));	
		while ((tpool->i_CurQueueSize == 0) && (!tpool->b_Shutdown)) 
		{	
			pthread_cond_wait(&(tpool->cond_QueueNotEmpty), &(tpool->mutex_QueueLock));	
		}	
		if (tpool->b_Shutdown) 
		{	
	                pthread_mutex_unlock(&(tpool->mutex_QueueLock));	
		        pthread_exit(NULL);	
		}	

		my_workp = tpool->pst_ReqQueueHead;	
		tpool->i_CurQueueSize--;	
		
		if (tpool->i_CurQueueSize == 0)	
			tpool->pst_ReqQueueHead = tpool->pst_ReqQueueTail = NULL;	
		else	
		        tpool->pst_ReqQueueHead = my_workp->next;	
		if ((!tpool->b_DoNotBlock) && (tpool->i_CurQueueSize== (tpool->i_MaxQueueSize - 1)))
			pthread_cond_broadcast(&(tpool->cond_QueueNotFull));	
		if (tpool->i_CurQueueSize == 0)	
		        pthread_cond_signal(&(tpool->cond_QueueEmpty));	
		pthread_mutex_unlock(&(tpool->mutex_QueueLock));	
		RequestHandler *pRequestHandler = (RequestHandler *)my_workp->arg;
		if (!pRequestHandler->ProcessRequest())
		printf("Error While processing request\n");
		pRequestHandler->EndRequest();
		printf("Request Ended\n\n");
		//(*(my_workp->routine))(my_workp->arg);	
		if (!pRequestHandler->IsAbortRequest())
		{
			pthread_mutex_lock(&(tpool->mutex_QueueLock));	
			tpool->piReferences[atoi(pRequestHandler->pzReferenceID)] = -1;
			pthread_mutex_unlock(&(tpool->mutex_QueueLock));	
		}
		free(my_workp);	
	}	
}

//**********************************************************************************************************
Tpool::Tpool(int iNoThreads, int iMaxQueueSize, bool bDoNotBlock)
{	
	int iReturn;	
	/* initialize the fields */	
	i_NoThreads = iNoThreads;	
	i_MaxQueueSize = iMaxQueueSize;	
	b_DoNotBlock = bDoNotBlock;	
	if ((p_threads = (pthread_t *)malloc(sizeof(pthread_t)*iNoThreads)) == NULL)
		perror("malloc"), exit(-1);	
	i_CurQueueSize = 0;	
	pst_ReqQueueHead = NULL;	
	pst_ReqQueueTail = NULL;	
	b_QueueClosed = 0;	
	b_Shutdown = 0;	
	if ((iReturn = pthread_mutex_init(&(mutex_QueueLock), NULL)) != 0)	
	        fprintf(stderr,"pthread_mutex_init %s",strerror(iReturn)), exit(-1);
	if ((iReturn = pthread_cond_init(&(cond_QueueNotEmpty), NULL)) != 0)	
	        fprintf(stderr,"pthread_cond_init %s",strerror(iReturn)), exit(-1);	
	if ((iReturn = pthread_cond_init(&(cond_QueueNotFull), NULL)) != 0)	
	        fprintf(stderr,"pthread_cond_init %s",strerror(iReturn)), exit(-1);	
	if ((iReturn = pthread_cond_init(&(cond_QueueEmpty), NULL)) != 0)	
	        fprintf(stderr,"pthread_cond_init %s",strerror(iReturn)), exit(-1);	
	
	piReferences = new int[iNoThreads];
	/* create threads */	
	for (int i = 0; i != iNoThreads; i++) 
	{	
		if ((iReturn = pthread_create( &(p_threads[i]), NULL, tpool_thread, (void *)this)) != 0)
			fprintf(stderr,"pthread_create %d",iReturn), exit(-1);	
		piReferences[i] = -1;
	}
	printf("\n"), fflush(stdout);
}

//**********************************************************************************************************
int Tpool::AddWork(void *routine, void *arg)
{
	tstRequest *workp;
	pthread_mutex_lock(&mutex_QueueLock);
	if ((i_CurQueueSize == i_MaxQueueSize) && b_DoNotBlock)
	{
		pthread_mutex_unlock(&mutex_QueueLock);
		return -1;
	}
	while ((i_CurQueueSize == i_MaxQueueSize) && (!(b_Shutdown || b_QueueClosed)))
	{
		pthread_cond_wait(&cond_QueueNotFull, &mutex_QueueLock);
	}	
	if (b_Shutdown || b_QueueClosed)
	{
		pthread_mutex_unlock(&mutex_QueueLock);
		return -1;
	}
	/* allocate work structure */
	workp = (tstRequest *)malloc(sizeof(tstRequest));
	workp->routine = (void (*)(void*))routine;
	workp->arg = arg;
	workp->next = NULL;
	if (i_CurQueueSize == 0) 
	{
		pst_ReqQueueTail = pst_ReqQueueHead = workp;
		pthread_cond_broadcast(&cond_QueueNotEmpty);
	} 
	else 
	{
		(pst_ReqQueueTail)->next = workp;
		pst_ReqQueueTail = workp;
	}
	i_CurQueueSize++;
	pthread_mutex_unlock(&mutex_QueueLock);
	return 1;
}

//**********************************************************************************************************
int Tpool::Destroy(int iFinish)	
{
	int iReturn;
	tstRequest *cur_nodep;
        if ((iReturn = pthread_mutex_lock(&(mutex_QueueLock))) != 0)
		fprintf(stderr,"pthread_mutex_lock %d",iReturn), exit(-1);
	/* Is a shutdown already in progress? */
	if (b_QueueClosed || b_Shutdown) 
	{
		if ((iReturn = pthread_mutex_unlock(&(mutex_QueueLock))) != 0)
			fprintf(stderr,"pthread_mutex_unlock %d",iReturn), exit(-1);
		return 0;
	}

	b_QueueClosed = 1;
        /* If the iFinish flag is set, wait for workers to drain queue */
        if (iFinish == 1) 
	{
		while (i_CurQueueSize != 0)
		{
			if ((iReturn = pthread_cond_wait(&(cond_QueueEmpty), &(mutex_QueueLock))) != 0)
				fprintf(stderr,"pthread_cond_wait %d",iReturn), exit(-1);
		}
	}
   	b_Shutdown = 1;

      	if ((iReturn = pthread_mutex_unlock(&(mutex_QueueLock))) != 0)
		fprintf(stderr,"pthread_mutex_unlock %d",iReturn), exit(-1);
	/* Wake up any workers so they recheck shutdown flag */
	if ((iReturn = pthread_cond_broadcast(&(cond_QueueNotEmpty))) != 0)
		fprintf(stderr,"pthread_cond_broadcast %d",iReturn), exit(-1);
	if ((iReturn = pthread_cond_broadcast(&(cond_QueueNotFull))) != 0)
		fprintf(stderr,"pthread_cond_broadcast %d",iReturn), exit(-1);

	/* Wait for workers to exit */
	for(int i = 0; i < i_NoThreads; i++) 
	{
		if ((iReturn = pthread_join(p_threads[i],NULL)) != 0)
			fprintf(stderr,"pthread_join %d",iReturn), exit(-1);
	}

  	/* Now free pool structures */
   	free(p_threads);
      	while(pst_ReqQueueHead != NULL) 
	{
		cur_nodep = pst_ReqQueueHead->next;
		pst_ReqQueueHead = pst_ReqQueueHead->next;
		free(cur_nodep);
	}
	
	delete []piReferences;
      	return 0;
}

//**********************************************************************************************************
Tpool::~Tpool()
{
	
}

//**********************************************************************************************************
int Tpool::GetNextReferenceID()
{
	pthread_mutex_lock(&mutex_QueueLock);
	int i = 0;
	while (piReferences[i] != -1 && i < i_NoThreads )
	{
		i++;
		if (i == i_NoThreads)
			i = 0;
	}

	// if i == i_NoThreads send error msg
	piReferences[i] = i;
	pthread_mutex_unlock(&mutex_QueueLock);

	return i;
}

//**********************************************************************************************************
