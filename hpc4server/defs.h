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
// // FILE     : defs.h
// // AUTHOR   : Waruna Ranasinghe (warunapww@gmail.com)
// // DESC     : The header file defines the constants and other required definitions 
// 		 for achieve interoperability amoung different operating systems
// // TODO     : 
// // HISTORY  : Date of Creation: 5-Oct-2008
//               Modified: 6-Nov-2008 : added #ifdef _REQUESTHANDLER_H_
// //
*/

#ifndef _DEFS_H_
#define _DEFS_H_

#ifdef _WIN32
#define snprintf _snprintf
#define index strchr
#define rindex strrchr
#define SIGPIPE SIGABRT
#endif //_WIN32
#define bzero(a, b) memset((a),0,(b))

#define MAX_QUEUE_LEN 128
#define IP_ADDR_LEN 17
#define MAX_READ_BUF_LEN 4096
#define MAX_TIMEOUT 5
#define HPC4F_MAX_BUF 8192
#define INVALID_REQUEST "-1"
#define	ANY_ADDR NULL

#ifdef _REQUESTHANDLER_H_
#define P_NOWAIT _P_NOWAIT
#define READ_HANDLE 0
#define WRITE_HANDLE 1
#define BUF_SIZE 512
#endif //_REQUESTHANDLER_H_

//#ifdef _HPC4FSERVER_H_
#define HPC4FSERVER_MAX_THREADS 100      
#define HPC4FSERVER_MIN_THREADS 50
#define HPC4FSERVER_MAX_QUEUE 10
//#endif //_HPC4FSERVER_H_
#endif //_DEFS_H_
