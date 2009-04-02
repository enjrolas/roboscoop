/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* file: macx_aConfig.c                                            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* description: PPRK Config Program MacX X entry point.            */
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

#include <Carbon/Carbon.h>
#include <unistd.h>

#include "aAssert.h"
#include "aMemLeakDebug.h"
#include "macx_aConfig.h"
#include "aConfigText.h"
#include "macx_util.h"
#include "aCmd.tea"

#define kPPRKRectSignature		'pprk'
#define kDoneCntlSignature		'done'
#define kServoLCntLeftSignature		'srvl'
#define kServoLCntRightSignature	'srvr'


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * local prototypes
 */

static void sHandleEvent(
  aMacXConfig* pMacXConfig,
  EventRecord* pEvent);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * sQuitAEHandler
 */

OSErr sQuitAEHandler(const AppleEvent *appleEvt, 
                     AppleEvent* reply, 
                     UInt32 refcon)
{
  aMacXConfig* pMacXConfig = (aMacXConfig*)refcon;

  aConfig_Shutdown((aConfig*)pMacXConfig);

  return noErr;

} /* sQuitAEHandler */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * sWindowHandler
 */

OSStatus sWindowHandler(EventHandlerCallRef inCallRef, 
                        EventRef inEvent, 
                        void * inUserData)
{
  aMacXConfig* pMacXConfig = (aMacXConfig*)inUserData;
  OSStatus result = eventNotHandledErr;
  UInt32 class = GetEventClass(inEvent);
  UInt32 kind = GetEventKind(inEvent);

  switch (class) {

  case kEventClassControl:
    switch (kind) {
    case kEventControlHit:
      {
        int val;
        ControlID controlID;
        ControlRef control;

        GetEventParameter(inEvent, 
			  kEventParamDirectObject,
	                  typeControlRef,
			  NULL,
			  sizeof(ControlRef),
			  NULL,
			  &control);
	GetControlID(control, &controlID);
	val = GetControlValue(control);

	switch (controlID.signature) {
	
	case kServoLCntLeftSignature:
          if (pMacXConfig->servos[controlID.id] == 0.5f) {
            aPPRK_SetServoAbs(pMacXConfig->pprkRef, 
      			        controlID.id, 
			        0.0f, NULL);
            pMacXConfig->servos[controlID.id] = 0.0f;
            SetControlValue(control, 0);
          } else {
            aPPRK_SetServoAbs(pMacXConfig->pprkRef, 
      			      controlID.id, 
			      0.5f, NULL);
            pMacXConfig->servos[controlID.id] = 0.5f;
            SetControlValue(control, 1);
          }
          controlID.signature = kServoLCntRightSignature;
          GetControlByID(pMacXConfig->window, 
                           &controlID, &control);
          SetControlValue(control, 0);
	  break;

	case kServoLCntRightSignature:
          if (pMacXConfig->servos[controlID.id] == -0.5f) {
            aPPRK_SetServoAbs(pMacXConfig->pprkRef, 
      			      controlID.id, 
			      0.0f, NULL);
            pMacXConfig->servos[controlID.id] = 0.0f;
            SetControlValue(control, 0);
          } else {
            aPPRK_SetServoAbs(pMacXConfig->pprkRef, 
      			      controlID.id, 
			      -0.5f, NULL);
            pMacXConfig->servos[controlID.id] = -0.5f;
            SetControlValue(control, 1);
          }
          controlID.signature = kServoLCntLeftSignature;
          GetControlByID(pMacXConfig->window, 
                           &controlID, &control);
          SetControlValue(control, 0);
	  break;

	case kDoneCntlSignature:
          aConfig_Shutdown((aConfig*)pMacXConfig);
	  result = noErr;
	  break;

	} /* switch */
      }
      break;
    } /* switch */
    break;

  case kEventClassWindow:
    switch (kind) {

    case kEventWindowActivated:
      break;

    case kEventWindowClose:
      aConfig_Shutdown((aConfig*)pMacXConfig);
      break;
  
    case kEventWindowDrawContent:
      sUpdate(pMacXConfig);
      break;
  
    } /* switch */
    break;

  } /* switch */
    
  return result;

} /* sWindowHandler */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * sHandleEvent
 */

void sHandleEvent(aMacXConfig* pMacXConfig,
                  EventRecord* pEvent)
{
  switch (pEvent->what) {
  
  case osEvt:
printf("OS\n");
    break;

  case kHighLevelEvent:
printf("high level\n");
    break;

  default:
/*printf("other\n");*/
    break;

  } /* pEvent->what switch */

} /* sHandleEvent method */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * sInitialize
 */

aErr sInitialize(aMacXConfig* pMacXConfig)
{
  aErr configErr = aErrNone;
  OSErr osErr;

  // Apple Event handler for quit
  osErr = AEInstallEventHandler(kCoreEventClass, 
                                kAEQuitApplication, 
                                NewAEEventHandlerUPP(
                                  (AEEventHandlerProcPtr)
                                     sQuitAEHandler), 
                                (UInt32)pMacXConfig, false);
  if (osErr != noErr) {
    printf("unable to install AE quit handler");
    ExitToShell();
  }

  return configErr;

} /* sInitialize */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * sBuildUI
 */

aErr sBuildUI(aMacXConfig* pMacXConfig)
{
  aErr configErr = aErrNone;
  OSErr osErr;
  ControlID controlID;
  ControlRef controlRef;

  const EventTypeSpec windowEvents[] = { 
    {kEventClassWindow, kEventWindowDrawContent},
    {kEventClassWindow, kEventWindowClose},
    {kEventClassControl, kEventControlHit}
  };
  EventHandlerUPP  handlerProc;

  CreateNibReference(CFSTR("Config"), &pMacXConfig->nibRef);
  CreateWindowFromNib(pMacXConfig->nibRef, CFSTR("Window"), 
		      &pMacXConfig->window);

  /* set up the window event handler */
  handlerProc = NewEventHandlerUPP(sWindowHandler);
  osErr = InstallWindowEventHandler(pMacXConfig->window, 
                                    handlerProc, 
                                    GetEventTypeCount(windowEvents),
				    windowEvents,
                                    pMacXConfig, 
				    NULL);
  aAssert(!osErr);
  
  /* get the pprk drawing rectangle */
  if (configErr == aErrNone) {
    aRECT drawingRect;
    Rect r;
    int min;
    controlID.signature = kPPRKRectSignature;
    controlID.id = 128;
    GetControlByID(pMacXConfig->window, &controlID, &controlRef);
    GetControlBounds(controlRef, &r);
    min = r.bottom - r.top;
    if (min > (r.right - r.left))
      min = r.right - r.left;
    pMacXConfig->size = (aUIPixelType)min;
    drawingRect.x = r.left;
    drawingRect.y = r.top;
    drawingRect.width = r.right - r.left;
    drawingRect.height = r.bottom - r.top;
    aGD_Create(pMacXConfig->uiRef, 
               &drawingRect,
               pMacXConfig->window, 
	       &pMacXConfig->pprkGD, 
               &configErr);
  }

  /* initialize the drawing now that the pprkRect is set */
  if (configErr == aErrNone)
    configErr = aConfig_SetupDrawing((aConfig*)pMacXConfig);

  /* set up the basic UI colors */
  if (configErr == aErrNone) {
    pMacXConfig->backColor.red = 0xCC00;
    pMacXConfig->backColor.green = 0xCC00;
    pMacXConfig->backColor.blue = 0x9900;
  }

  ShowWindow(pMacXConfig->window);

  return configErr;

} /* sBuildUI */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * sRaiseUI
 */

aErr sRaiseUI(aMacXConfig* pMacXConfig)
{
  aErr configErr = aErrNone;

  if (pMacXConfig->nibRef)
    DisposeNibReference(pMacXConfig->nibRef);

  return configErr;

} /* sRaiseUI */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * sUpdate
 */

aErr sUpdate(aMacXConfig* pMacXConfig)
{
  aErr configErr = aErrNone;
  
  DrawControls(pMacXConfig->window);

  return configErr;

} /* sUpdate */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * aConfig_Create
 */

aErr aConfig_Create(aConfig** ppConfig)
{
  aErr configErr = aErrNone;
  aMacXConfig* pMacXConfig = NULL;

  aAssert(ppConfig);

  /* allocate the application object */  
  pMacXConfig = (aMacXConfig*)aMemAlloc(sizeof(aMacXConfig));
  if (!pMacXConfig)
    configErr = aErrMemory;
  else {
    aBZero(pMacXConfig, sizeof(aMacXConfig));
    configErr = aConfig_Initialize((aConfig*)pMacXConfig);
  }

  /* handle MacOS X application initialization */
  if (configErr == aErrNone)
    configErr = sInitialize(pMacXConfig);

  if (configErr == aErrNone)
    configErr = sBuildUI(pMacXConfig);

  /* assign the result if things worked out or clean up */
  if (configErr == aErrNone)
    *ppConfig = (aConfig*)pMacXConfig;
  else if (pMacXConfig)
    aMemFree(pMacXConfig);

  return configErr;

} /* aConfig_Create */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * aConfig_Run
 */

aErr aConfig_Run(aConfig* pConfig)
{
  aErr configErr = aErrNone;
  aMacXConfig* pMacXConfig = (aMacXConfig*)pConfig;
  EventRecord event;

  /* the message loop */
  while ((!pConfig->bDone) 
	 && (configErr == aErrNone)) {

    /* check for a message */
    if (EventAvail(everyEvent, &event)
        && GetNextEvent(everyEvent, &event)) {
      sHandleEvent(pMacXConfig, &event);
    } else {
      aConfig_TimeSlice(pConfig, &configErr);
    }
  }

  return configErr;

} /* aConfig_Run */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * aConfig_Destroy
 */

aErr aConfig_Destroy(aConfig* pConfig)
{
  aErr configErr = aErrNone;
  aMacXConfig* pMacXConfig = (aMacXConfig*)pConfig;
  
  aAssert(pConfig);

  if (configErr == aErrNone)
    configErr = aConfig_Shutdown(pConfig);

  configErr = sRaiseUI(pMacXConfig);

  aMemFree(pMacXConfig);

  return configErr;

} /* aConfig_Destroy */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * main
 */

int main(
  const int argc, 
  const char* argv[])
{
  aErr configErr = aErrNone;
  aConfig* pConfig;

  if (configErr == aErrNone)
    configErr = macx_SetCWD(argc, argv);

  if (configErr == aErrNone)
    configErr = aConfig_Create(&pConfig);

  if (configErr == aErrNone)
    configErr = aConfig_Run(pConfig);

  if (configErr == aErrNone)
    configErr = aConfig_Destroy(pConfig);
  
  aLeakCheckCleanup();

  return(0);

} /* main */
