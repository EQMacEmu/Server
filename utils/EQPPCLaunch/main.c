//
// EQPPCLauncher
// solar@heliacal.net
//
// This program serves the same purpose as the EverQuestLP.app - EQ Mac Launchpad which was used in the original PowerPC version of EQ for Macintosh
// It will launch game/EverQuest and pass the session ticket over to it.  For EQEmu we use this ticket as username/password.
//

#include <stdio.h>
#include <libgen.h>
#include <sys/param.h>
#include <stdlib.h>
#include <mach-o/dyld.h>
#include <Carbon/Carbon.h>

void usage(const char *programName)
{
	fprintf(stderr, "Usage: %s /ticket:USERNAME/PASSWORD /app:/path/to/game/EverQuest\n", programName);
	fprintf(stderr, "\nIf /app: is not specified, this program will attempt to run 'EverQuest' in the same location as this program.\n");
	fprintf(stderr, "Make sure you edit game/eqhost.txt with the right login server address:port.  Registration server doesn't matter.\n");
}

int main (int argc, const char *argv[])
{
    char *appPath = NULL;
    char *PSID = NULL; // session id ticket, format as username/password for our login server
    char *GHAD = "eqworld-52.989studios.com"; // this makes our login server treat this as 'OSX', it's not really going to connect to this
	
	if(argc < 2)
	{
		usage(argv[0]);
		return 1;
	}
	
	for(int i=1; i < argc; i++)
	{
		if(argv[i] != NULL && strlen(argv[i]) > 8 && strncmp("/ticket:", argv[i], 8) == 0)
		{
			PSID = strdup((char *)(argv[i] + 8));
		}
		if(argv[i] != NULL && strlen(argv[i]) > 5 && strncmp("/app:", argv[i], 5) == 0)
		{
			appPath = strdup((char *)(argv[i] + 5));
		}
	}
	
	if(PSID == NULL)
	{
		usage(argv[0]);
		return 1;
	}
	if(appPath == NULL)
	{
		char path[1024], resolvedpath[1024];
		uint32_t size = sizeof(path);
		if (_NSGetExecutablePath(path, &size) != 0)
		{
			fprintf(stderr, "_NSGetExecutablePath: buffer too small; need size %u\n", size);
			return 1;
		}
		if(realpath(path, resolvedpath) == NULL)
		{
			fprintf(stderr, "realpath error resolving %s : %s", path, realpath);
			return 1;
		}
		strcpy(path, dirname(resolvedpath));
		if(strlen(path) > 1013)
		{
			fprintf(stderr, "path too long");
			return 1;
		}
		strcat(path, "/EverQuest");
		appPath = path;
		printf("using app path: %s\n", appPath);
	}
	
    OSStatus err = noErr;
    
    // Launch application and retrieve the ProcessSerialNumber so we can target it with an event
    FSRef appRef;
    CFURLRef appURL = CFURLCreateWithBytes(kCFAllocatorDefault, (const UInt8 *)appPath, strlen(appPath), kCFStringEncodingUTF8, NULL);
    CFURLGetFSRef(appURL, &appRef);
    CFRelease(appURL);    
    FSSpec appSpec;
    FSGetCatalogInfo(&appRef, kFSCatInfoNone, NULL, NULL, &appSpec, NULL);
    LaunchParamBlockRec lpb;
    memset(&lpb, 0, sizeof(LaunchParamBlockRec));
    lpb.launchAppSpec = &appSpec;
    lpb.launchControlFlags = launchContinue | launchNoFileFlags;
    err = LaunchApplication (&lpb);
    if(err != noErr)
    {
        fprintf(stderr, "LaunchApplication error: %ld %s %s\n", err, GetMacOSStatusErrorString(err), GetMacOSStatusCommentString(err));
        return 1;
    }
    printf("Launched '%s' with PSN %lu\n", appPath, lpb.launchProcessSN.lowLongOfPSN);
    
    // prepare Apple Event
    AEDesc aeDesc;
    AppleEvent ae;
    err = AECreateDesc(typeProcessSerialNumber, &lpb.launchProcessSN, sizeof(ProcessSerialNumber), &aeDesc);
    if(err != noErr)
    {
        fprintf(stderr, "AECreateDesc error: %ld %s %s\n", err, GetMacOSStatusErrorString(err), GetMacOSStatusCommentString(err));
        return 1;
    }
    err = AECreateAppleEvent('MLPD', 'PARM', &aeDesc, kAutoGenerateReturnID, kAnyTransactionID, &ae);
    if(err != noErr)
    {
        fprintf(stderr, "AECreateAppleEvent error: %ld %s %s\n", err, GetMacOSStatusErrorString(err), GetMacOSStatusCommentString(err));
        return 1;
    }
    
    // add parameters to the apple event
    err = AEPutParamPtr(&ae, 'PSID', typeChar, PSID, strlen(PSID));
    if(err != noErr)
    {
        fprintf(stderr, "AEPutParamPtr error: %ld %s %s\n", err, GetMacOSStatusErrorString(err), GetMacOSStatusCommentString(err));
        return 1;
    }
    err = AEPutParamPtr(&ae, '----', typeChar, PSID, strlen(PSID));
    if(err != noErr)
    {
        fprintf(stderr, "AEPutParamPtr error: %ld %s %s\n", err, GetMacOSStatusErrorString(err), GetMacOSStatusCommentString(err));
        return 1;
    }
    err = AEPutParamPtr(&ae, 'GHAD', typeChar, GHAD, strlen(GHAD));
    if(err != noErr)
    {
        fprintf(stderr, "AEPutParamPtr error: %ld %s %s\n", err, GetMacOSStatusErrorString(err), GetMacOSStatusCommentString(err));
        return 1;
    }
    
    // send the apple event to the process we launched
    err = AESend(&ae, NULL, kAENoReply, kAENormalPriority, kAEDefaultTimeout, NULL, NULL);
    if(err != noErr)
    {
        fprintf(stderr, "AESend error: %ld %s %s\n", err, GetMacOSStatusErrorString(err), GetMacOSStatusCommentString(err));
        return 1;
    }
    
    // release resources
    AEDisposeDesc(&ae);
    AEDisposeDesc(&aeDesc);
	free(PSID);
	
    printf("Successfully sent Apple Event.\n");
    
    return 0;
}

