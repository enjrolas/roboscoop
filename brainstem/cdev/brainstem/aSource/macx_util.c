/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* file: macx_util.c	                                           */
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

#include <unistd.h>
#include <sys/param.h>
#include <string.h>
#include <stdio.h>

#include "macx_util.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * macx_SetCWD
 *
 * Applications launched by the Finder default to / as the current
 * working directory, this code sets the current working directory
 * to that of the executable based on the calling parameter 0 of
 * the main routine.
 */

aErr macx_SetCWD(
  const int argc, 
  const char* argv[]
)
{
  aErr xuErr = aErrNone;

  if (xuErr == aErrNone) {
    char appDir[MAXPATHLEN];
    char* appExt;
    strcpy(appDir, argv[0]);
    appExt = strstr(appDir, ".app");
    if (appExt)  {
      int len = appExt - appDir;
      while (len-- && *appExt != '/') 
        appExt--;
      if (len) {
        appExt++;
        *appExt = 0;
        chdir(appDir);
      }
    }
  }

  return xuErr;

} /* macx_SetCWD */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * macx_LoadPrivateFrameworkBundle
 */

aErr macx_CreateNibReferenceWithFramework(CFStringRef framework, 
					  CFStringRef nibName,
				          IBNibRef *nibRef)
{
 aErr xuErr = aErrNone;
 OSErr osErr;
 CFURLRef baseURL   = NULL;
 CFURLRef bundleURL  = NULL;
 CFBundleRef myAppsBundle = NULL;
 CFBundleRef bundle = NULL;

 /* Get our application's main bundle from Core Foundation */
 myAppsBundle = CFBundleGetMainBundle();     
 if ( myAppsBundle == NULL ) goto Bail;
 
 baseURL = CFBundleCopySharedFrameworksURL(myAppsBundle);
 if (baseURL == NULL) goto Bail;

 bundleURL = CFURLCreateCopyAppendingPathComponent(kCFAllocatorSystemDefault, 
						   baseURL, framework, 
						   false);
 if (bundleURL == NULL) goto Bail;

 bundle = CFBundleCreate( kCFAllocatorSystemDefault, bundleURL );
 if (bundle == NULL) goto Bail;

 osErr = CreateNibReferenceWithCFBundle(bundle, nibName, nibRef);

Bail:
 if (bundle != NULL) CFRelease(bundle);
 if (bundleURL != NULL) CFRelease(bundleURL);
 if (baseURL != NULL)  CFRelease(baseURL);

  return  xuErr;

} /* macx_CreateNibReferenceWithFramework */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * macx_EnableMenuByID
 */

void
macx_EnableMenuByID(OSType commandID)
{
  MenuRef ref;
  MenuItemIndex menuIndex;
  OSStatus status;
  
  if (commandID) {
    status = GetIndMenuItemWithCommandID(NULL, commandID, 
					 1, &ref, &menuIndex);
    EnableMenuItem(ref, menuIndex);
  }
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * macx_DisableMenuByID
 */

void
macx_DisableMenuByID(OSType commandID)
{
  MenuRef ref;
  MenuItemIndex menuIndex;
  OSStatus status;
  
  if (commandID) {
    status = GetIndMenuItemWithCommandID(NULL, commandID, 
					 1, &ref, &menuIndex);
    DisableMenuItem(ref, menuIndex);
  }
}

#endif /* aMACX */

