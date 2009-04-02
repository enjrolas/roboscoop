/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* file: aConfig.c						   */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* description: Implementation of a platform-independent PPRK	   */
/*		config object.					   */
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

#include "aOSDefs.h"
#include "aConfig.h"
#include "aConfigText.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * aConfig_Initialize
 */

aErr aConfig_Initialize(aConfig* pConfig)
{
  aErr configErr = aErrNone;
  aSettingFileRef settings;

  /* make sure we have a pointer to work with */
  if (pConfig == NULL)
    configErr = aErrParam;

  if (configErr == aErrNone) {
    /* first, blast any fields in the config struct and set 
     * the check byte 
     */
    aBZero(pConfig, sizeof(aConfig));
    pConfig->check = aCONFIGCHECK;
    /* some fake initial values */
    pConfig->rangers[0] = 1.0f;
    pConfig->rangers[1] = 0.5f;
    pConfig->rangers[2] = 0.2f;
  }

  /* get the IO lib ref */
  if (configErr == aErrNone)
    aIO_GetLibRef(&pConfig->ioRef, &configErr);

  /* get the UI lib ref */
  if (configErr == aErrNone)
    aUI_GetLibRef(&pConfig->uiRef, &configErr);

  /* go find out what type of controller is being used */
  if (configErr == aErrNone)
    aSettingFile_Create(pConfig->ioRef,
    			32,
    			"aPPRK.config",
    			&settings,
    			&configErr);

  /* find out what type of controller we are using 
   * with a default of BrainStem */
  if (configErr == aErrNone) {
    char* controller;
    aSettingFile_GetString(pConfig->ioRef, 
    			   settings,
    			   "controller", 
    			   &controller,
    			   "BrainStem",
    			   &configErr);

    if (configErr == aErrNone) {
      if (!aStringCompare(controller, "BrainStem")) {
        pConfig->nControllerType = kControllerStem; 
      } else if (!aStringCompare(controller, "SV203")) {
        pConfig->nControllerType = kControllerSV203; 
      }
    }

    /* we don't need the settings file anymore */   
    if (configErr == aErrNone)
      aSettingFile_Destroy(pConfig->ioRef,
      			   settings,
      			   &configErr);
  }

  /* get the PPRK lib ref */
  if (configErr == aErrNone) {
    aPPRK_GetLibRef(&pConfig->pprkRef, &configErr);
    switch (configErr) {

    case aErrBusy:
      aDialog_Message(pConfig->uiRef,
      		      aCONFIG_PORT_BUSY,
      		      NULL, NULL,
      		      &configErr);
      break;

    case aErrNotFound:
      aDialog_Message(pConfig->uiRef,
      		      aCONFIG_PORT_NOT_FOUND,
      		      NULL, NULL,
      		      &configErr);
      break;

    case aErrNone:
      break;

    default:
      break;

    } /* switch */
  }

  return(configErr);

} /* aConfig_Initialize routine */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * aConfig_Shutdown
 */

aErr aConfig_Shutdown(aConfig* pConfig)
{
  aErr configErr = aErrNone;

  aVALIDCONFIG(pConfig);

  if (configErr == aErrNone) {
    pConfig->bDone = aTrue;
    pConfig->bInited = aFalse;
  }

  /* release the PPRK library reference */
  if ((configErr == aErrNone) &&
      (pConfig->pprkRef != NULL) &&
      !aPPRK_ReleaseLibRef(pConfig->pprkRef, &configErr))
    pConfig->pprkRef = NULL;

  /* release the GD that was created */
  if ((pConfig->pprkGD != NULL) &&
      !aGD_Destroy(pConfig->uiRef, pConfig->pprkGD, &configErr))
    pConfig->pprkGD = NULL;

  /* release the UI library reference */
  if ((pConfig->uiRef != NULL) &&
      !aUI_ReleaseLibRef(pConfig->uiRef, &configErr))
    pConfig->uiRef = NULL;

  /* release the IO library reference */
  if ((pConfig->ioRef != NULL) &&
      !aIO_ReleaseLibRef(pConfig->ioRef, &configErr))
    pConfig->ioRef = NULL;

  return(configErr);
  
} /* aConfig_Shutdown routine */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * aConfig_TimeSlice
 */

aBool aConfig_TimeSlice(aConfig* pConfig,
		        aErr* pErr)
{
  aErr configErr = aErrNone;
  aBool bChanged = aFalse;
  static aBool bInitialDraw = aFalse;

  /* draw once at the start and then only redraw below when things
   * have changed */
  if (!bInitialDraw) {
    aConfig_Draw(pConfig);
    bInitialDraw = aTrue;
  }

  if ((pConfig->bDone == aFalse) && (pConfig->bInited == aTrue)) {

    if (configErr == aErrNone) {
      aPPRK_TimeSlice(pConfig->pprkRef, &configErr);
    }

    if (configErr == aErrNone) {
      float fOld = pConfig->rangers[pConfig->curRanger];
      aPPRK_GetRange(pConfig->pprkRef, 
    		     (unsigned int)pConfig->curRanger, 
  		     &pConfig->rangers[pConfig->curRanger], 
  		     &configErr);

      if ((fOld != pConfig->rangers[pConfig->curRanger])
          && (configErr == aErrNone)) {

        bChanged = aTrue;
        aConfig_Draw(pConfig);

        /* move on to the next ranger */
        if (pConfig->curRanger >= 2)
          pConfig->curRanger = 0;
        else
          pConfig->curRanger++;
      }
    }
  }

  /* stall here if nothing happened above to avoid swamping the
   * CPU */
  if ((configErr == aErrNone) && !bChanged)
    aIO_MSSleep(pConfig->ioRef, 1, &configErr);

  return bChanged;

} /* aConfig_TimeSlice */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * aConfig_SetupDrawing
 */

aErr aConfig_SetupDrawing(aConfig* pConfig)
{
  aErr configErr = aErrNone;
  int centerX, centerY, radius, i;
  static float a1[nFRAMEPOINTS * 2] = {
    1.0000f,  0.0000f,
    0.5000f,  0.8660f,
   -0.5000f,  0.8660f,
   -1.0000f,  0.0000f,
   -0.5000f, -0.8660f,
    0.5000f, -0.8660f,
    1.0000f,  0.0000f
  };
  static float a2[6] = {
    0.0000f,  1.0000f,
   -0.8660f, -0.5000f,
    0.8660f, -0.5000f
  };
  static float a3[6] = {
    0.0000f, -1.0000f,
    0.8660f,  0.5000f,
   -0.8660f,  0.5000f
  };
  float* p;

  /* first, set up the points for the frame drawing */
  radius = pConfig->size / 4;
  centerX = pConfig->size / 2;
  centerY = pConfig->size / 2;

  /* the points are radially organized around the center */
  p = a1;
  for (i = 0; i < nFRAMEPOINTS; i++) {
    pConfig->framePoints[i].x = 
    		(aUIPixelType)(centerX + radius * *p++);
    pConfig->framePoints[i].y = 
    		(aUIPixelType)(centerY + radius * *p++);
  }

  /* the ranger starting points */
  radius = (int)(radius * 0.9);
  p = a2;
  for (i = 0; i < 3; i++) {
    pConfig->rangerPoints[i].x =
    		(aUIPixelType)(centerX + radius * *p++);
    pConfig->rangerPoints[i].y =
    		(aUIPixelType)(centerY + radius * *p++);
  }

  /* the controller type text bounding rectangle */
  pConfig->textRect.x = 0;
  pConfig->textRect.y = 0;
  pConfig->textRect.width = pConfig->size;
  pConfig->textRect.height = pConfig->size;

  /* the servo label bounding text boxes */
  p = a3;
  radius += 10;
  for (i = 0; i < 3; i++) {
    pConfig->lblRect[i].x = (aUIPixelType)((centerX + radius * *p++) - 10);
    pConfig->lblRect[i].y = (aUIPixelType)((centerY + radius * *p++) - 8);
    pConfig->lblRect[i].width = 20;
    pConfig->lblRect[i].height = 16;
  }

  if (configErr == aErrNone)
    pConfig->bInited = aTrue;

  return configErr;

} /* aConfig_SetupDrawing */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * aConfig_Draw
 */

void aConfig_Draw(aConfig* pConfig)
{
  aErr e = aErrNone;
  int i;
  static float angles[12] = {
      0.1736f,  0.9848f, /* ranger 1 */
     -0.1736f,  0.9848f,
     -0.9397f, -0.3420f, /* ranger 2 */
     -0.7660f, -0.6428f,
      0.7660f, -0.6428f, /* ranger 3 */
      0.9397f, -0.3420f
  };
  aRECT bounds;
  bounds.x = 0;
  bounds.y = 0;
  bounds.width = pConfig->size;
  bounds.height = pConfig->size;

  if ((pConfig->bDone == aTrue) || (pConfig->bInited == aFalse))
    return;

  if (e == aErrNone)
    aGD_StartDrawing(pConfig->uiRef, pConfig->pprkGD, &e);

  aAssert(e == aErrNone);

  if (e == aErrNone)
    aGD_Erase(pConfig->uiRef, pConfig->pprkGD, &e);

  aAssert(e == aErrNone);

  if (e == aErrNone)
    aGD_SetColor(pConfig->uiRef, pConfig->pprkGD, 0xCCCC99, &e);

  aAssert(e == aErrNone);
    
  if (e == aErrNone)
    aGD_Rect(pConfig->uiRef, pConfig->pprkGD, &bounds, aFalse, &e);

  aAssert(e == aErrNone);

  if (e == aErrNone)
    aGD_SetColor(pConfig->uiRef, pConfig->pprkGD, 0x999999, &e);

  aAssert(e == aErrNone);

  if (e == aErrNone)
    aGD_Line(pConfig->uiRef, pConfig->pprkGD,
             pConfig->framePoints, nFRAMEPOINTS, &e);
  aAssert(e == aErrNone);

  if (e == aErrNone)
    aGD_SetColor(pConfig->uiRef, pConfig->pprkGD, 0x000000, &e);

  aAssert(e == aErrNone);

  if (e == aErrNone) {
    switch (pConfig->nControllerType) {

      case kControllerSV203:
        aGD_Text(pConfig->uiRef, 
                 pConfig->pprkGD,
                 &pConfig->textRect,
                 aUIALIGNCENTER,
                 16,
                 aCONFIG_CNTL_SV203, 
                 &e);
        break;

      case kControllerStem:
        aGD_Text(pConfig->uiRef, 
                 pConfig->pprkGD,
                 &pConfig->textRect,
                 aUIALIGNCENTER,
                 16,
                 aCONFIG_CNTL_STEM, 
                 &e);
        break;

    } /* switch */

    aAssert(e == aErrNone);
  }

  if (e == aErrNone)
    aGD_SetColor(pConfig->uiRef, pConfig->pprkGD, 0xFF0000, &e);
  aAssert(e == aErrNone);

  for (i = 0; (i < 3) && (e == aErrNone); i++) {
    int len, p;
    aPT points[4];

    p = i * 4;
    points[0] = points[3] = pConfig->rangerPoints[i];

    len = (int)(pConfig->rangers[i] 
                * pConfig->size / 4);
    points[1].x = (aUIPixelType)(points[0].x + angles[p++] * len);
    points[1].y = (aUIPixelType)(points[0].y + angles[p++] * len);
    points[2].x = (aUIPixelType)(points[0].x + angles[p++] * len);
    points[2].y = (aUIPixelType)(points[0].y + angles[p] * len);
    aGD_Line(pConfig->uiRef, pConfig->pprkGD, points, 4, &e);  
    aAssert(e == aErrNone);

  } /* for */

  /* the servos */
  if (e == aErrNone)
    aGD_SetColor(pConfig->uiRef, pConfig->pprkGD, 0x0000FF, &e);
  aAssert(e == aErrNone);
  for (i = 0; ((e == aErrNone) && (i < 3)); i++) {
    char num[2];
    num[0] = (char)(i + 1 + '0');
    num[1] = 0;
    aGD_Rect(pConfig->uiRef, pConfig->pprkGD, 
    	     &pConfig->lblRect[i], aFalse, &e);
    aAssert(e == aErrNone);
    aGD_Text(pConfig->uiRef, pConfig->pprkGD, 
    	     &pConfig->lblRect[i], 
    	     aUIALIGNCENTER, 
    	     14,
    	     num,
    	     &e);
    aAssert(e == aErrNone);
  }

  if (e == aErrNone)
    aGD_EndDrawing(pConfig->uiRef, pConfig->pprkGD, &e);
  aAssert(e == aErrNone);

} /* aConfig_Draw */
