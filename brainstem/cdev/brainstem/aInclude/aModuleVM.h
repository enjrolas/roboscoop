/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* file: aModuleVM.h						   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* description: Definition of BrainStem module vm access routines. */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Copyright 1994-2008. Acroname Inc.                              */
/*                                                                 */
/* This software is the property of Acroname Inc.  Any             */
/* distribution, sale, transmission, or re-use of this code is     */
/* strictly forbidden except with permission from Acroname Inc.    */
/*                                                                 */
/* To the full extent allowed by law, Acroname Inc. also excludes  */
/* for itself and its suppliers any liability, wheither based in   */
/* contract or tort (including negligence), for direct,            */
/* incidental, consequential, indirect, special, or punitive       */
/* damages of any kind, or for loss of revenue or profits, loss of */
/* business, loss of information or data, or other financial loss  */
/* arising out of or in connection with this software, even if     */
/* Acroname Inc. has been advised of the possibility of such       */
/* damages.                                                        */
/*                                                                 */
/* Acroname Inc.                                                   */
/* www.acroname.com                                                */
/* 720-564-0373                                                    */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef _aModuleVM_H_
#define _aModuleVM_H_

#include "aStem.h"
#include "aTEA.h"

#ifdef __cplusplus
extern "C" {
#endif

aErr aModuleVM_LaunchProcess(aStemLib stemRef,
			     const unsigned char module,
			     const unsigned char fileSlot,
			     const char* pProgramData,
			     const unsigned char nProgramDataLen,
			     const unsigned char flags,
			     aTEAProcessID* pProcID);

aErr aModuleVM_Launch(aStemLib stemRef,
		      const unsigned char module,
		      const unsigned char slot,
		      const unsigned char nParamBytes,
		      const char* pParamBytes);

aErr aModuleVM_WaitForExit(aStemLib stemRef,
			   int* pReturnBytes,
			   int* pReturnValue);

aErr aModuleVM_Execute(aStemLib stemRef,
		       const unsigned char module,
		       const unsigned char slot,
		       const unsigned char nParamBytes,
		       const char* pParamBytes,
		       int* pReturn);

#ifdef __cplusplus 
}
#endif

#endif /* _aModuleVM_H_ */
