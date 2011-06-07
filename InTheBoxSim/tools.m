//
//  tools.m
//  InTheBox
//
//  Created by Benoit Lamarche on 16/05/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "tools.h"


char * getAppDocumentsPath ()
{
    char cpath2 [PATH_MAX];
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    const char * cpath = [documentsDirectory UTF8String];
    strcpy(cpath2, cpath);
    [pool release];
    return cpath2;
}

char * getAppBundlePath ()
{
    char cpath2 [PATH_MAX];
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)cpath2, PATH_MAX))
    {
        fprintf(stderr, "getAppBundlePath Path error \n");
    }
    CFRelease(resourcesURL);
    [pool release];
    return cpath2;
}

