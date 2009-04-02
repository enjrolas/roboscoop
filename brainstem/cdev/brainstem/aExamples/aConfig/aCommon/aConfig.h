/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* file: aConfig.h                                                 */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* description: Definition of a platform-independent PPRK library  */
/*		configuration object.				   */
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

#ifndef _aConfig_H_
#define _aConfig_H_

#include "aIO.h"
#include "aUI.h"
#include "aErr.h"
#include "aPPRK.h"

#define nFRAMEPOINTS		7

#define kControllerSV203	0
#define kControllerStem		1


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Here we define the basic header elements that need to 
 * be in all os-versions of the config structure.  This 
 * header needs to be the first element in the structure 
 * before any os-specific fields.  This is sort of like 
 * sub-classing without the C++ overhead.
 */

#define aConfigStructHeader				   	   \
  aIOLib		ioRef;					   \
  aUILib		uiRef;					   \
  aPPRKLib		pprkRef;				   \
  aBool			bDone;					   \
  aBool			bInited;				   \
  float			rangers[3];				   \
  int			curRanger;				   \
  aRECT			textRect;				   \
  aRECT			lblRect[3];				   \
  aGDRef		pprkGD;					   \
  aPT			framePoints[nFRAMEPOINTS];		   \
  aPT			rangerPoints[3];			   \
  float			servos[3];                                 \
  int			nControllerType;			   \
  aUIPixelType		size;					   \
  int			check


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * UI colors
 */

#define aCONFIG_PANE_COLOR_RED		0xCC
#define aCONFIG_PANE_COLOR_GREEN	0xCC
#define aCONFIG_PANE_COLOR_BLUE		0x99

#define aCONFIG_STATUSONLED_RED		0x00
#define aCONFIG_STATUSONLED_GREEN	0xFF
#define aCONFIG_STATUSONLED_BLUE	0x00

#define aCONFIG_STATUSOFFLED_RED	0x00
#define aCONFIG_STATUSOFFLED_GREEN	0x50
#define aCONFIG_STATUSOFFLED_BLUE	0x00

#define aCONFIG_POWERLED_RED		0xFF
#define aCONFIG_POWERLED_GREEN		0x00
#define aCONFIG_POWERLED_BLUE		0x00


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * UI dimensions
 */

#define aCONFIG_WIDTH		240
#define aCONFIG_HEIGHT		270
#define aCONFIG_SERVOBUT_WIDTH	30
#define aCONFIG_SERVOLBL_WIDTH	60
#define aCONFIG_SERVO_HEIGHT	16
#define aCONFIG_SERVO_SPACE	3


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This is the basic os-independent console structure
 */

typedef struct aConfig {
  aConfigStructHeader;
} aConfig;

#define aCONFIGCHECK  0xFADE

#define aVALIDCONFIG(p) if(((p) == NULL) || 			   \
		(((aConfig*)p)->check != aCONFIGCHECK)) {	   \
		  configErr = aErrParam; }

			
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * "virtual methods" that need to be created for each operating 
 * system.
 */

aErr aConfig_Create(aConfig** ppConfig);
aErr aConfig_Destroy(aConfig* pConfig);
aErr aConfig_Run(aConfig* pConfig);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * These are the os-independent routines that can be called
 * from any operating system.
 */

  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   * aConfig_Initialize is typically called from the 
   * os-specific aConfig_Create routine.
   */

aErr aConfig_Initialize(aConfig* pConfig);

  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   * aConfig_TimeSlice is typically called from the 
   * os-specific aConfig_Run routine to handle periodic tasks.
   */

aBool aConfig_TimeSlice(aConfig* pConfig,
		        aErr* pErr);

  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   * this routine prepares the drawing to be done based on the
   * pprkRect in the pConfig structure.
   */

aErr aConfig_SetupDrawing(aConfig* pConfig);

  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   * this routine draws the PPRK based on the dirty bits field
   */

void aConfig_Draw(aConfig* pConfig);

  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   * this routine turns the bNotDone flag to aFalse and
   * cleans up the os-independent structure elements.
   */

aErr aConfig_Shutdown(aConfig* pConfig);

#endif /* _aConfig_H_ */
