/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* file: macx_util.h	                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* description: Utility routines useful for MacX code. 		   */
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

#ifdef aMACX

#ifndef _macx_util_H_
#define _macx_util_H_

#include <Carbon/Carbon.h>

#include "aErr.h"

#define MACXCOLOR(c)		(c << 8)

#define aRECT_2_MacRect(mr, ar)  mr.left = (ar).x,                  \
			         mr.top = (ar).y,                   \
				 mr.right = (ar).x + (ar).width,    \
				 mr.bottom = (ar).y + (ar).height

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

aErr macx_SetCWD(
  const int argc, 
  const char* argv[]
);

aErr macx_CreateNibReferenceWithFramework(CFStringRef framework, 
					  CFStringRef nibName,
				          IBNibRef *nibRef);

void macx_EnableMenuByID(OSType commandID);
void macx_DisableMenuByID(OSType commandID);
  
#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif /* _macx_util_H_ */

#endif /* aMACX */
