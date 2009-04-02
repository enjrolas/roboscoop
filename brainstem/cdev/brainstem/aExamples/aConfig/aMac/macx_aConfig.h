/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* file: macx_aConfig.h     					   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* description: Resource definition for MacOS X Config             */
/*              application.					   */
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

#ifndef _macx_aConfig_H_
#define _macx_aConfig_H_

#include "aConfig.h"

typedef struct aMacXConfig {
  aConfigStructHeader;
  IBNibRef 		nibRef;
  WindowRef		window;
  RGBColor		backColor;
} aMacXConfig;

static pascal OSErr sQuitAEHandler(const AppleEvent *appleEvt, 
				   AppleEvent* reply, 
				   UInt32 refcon);
static pascal OSStatus sWindowHandler(EventHandlerCallRef inCallRef, 
				      EventRef inEvent, 
				      void * inUserData);

static aErr sInitialize(aMacXConfig* pMacXGP);
static aErr sBuildUI(aMacXConfig* pMacXGP);
static aErr sRaiseUI(aMacXConfig* pMacXGP);
static aErr sUpdate(aMacXConfig* pMacXGP);

#endif /* _macx_aConfig_H_ */

