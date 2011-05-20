//
//  main.m
//  InTheBoxSim
//
//  Created by Benoit Lamarche on 27/04/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "Main.c"

#include "CoreFoundation/CoreFoundation.h"

int main(int argc, char *argv[])
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
    {
        fprintf(stderr, "Path error \n");
    }
    CFRelease(resourcesURL);
    
    int argc2 = 5;
    char *argv2[5];
    argv2[0] = argv[0];
    argv2[1] = "-cp";
    char foopath[PATH_MAX];
    char * foopath2;
    strcpy(foopath, path);
    foopath2 = strcat(foopath, "/foo.jar");
    argv2[2] = foopath2;
    char bootclasspath[5*PATH_MAX + 17 + 10 + 9 + 15 + 20 + 14];
    strcpy(bootclasspath, "-Xbootclasspath:");
    strcat(bootclasspath, path);
    strcat(bootclasspath, "/core.jar:");
    strcat(bootclasspath, path);
    strcat(bootclasspath, "/ext.jar:");
    strcat(bootclasspath, path);
    strcat(bootclasspath, "/framework.jar:");
    strcat(bootclasspath, path);
    strcat(bootclasspath, "/android.policy.jar:");
    strcat(bootclasspath, path);
    strcat(bootclasspath, "/services.jar");
    argv2[3] = bootclasspath;
    argv2[4] = "Foo";
    androidMain(argc2, argv2);
    int retVal = UIApplicationMain(argc, argv, nil, nil);
    [pool release];
    return retVal;
}
