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
    char * bootclasspath2;
    bootclasspath2 = strcat(bootclasspath, "-Xbootclasspath:");
    bootclasspath2 = strcat(bootclasspath2, path);
    bootclasspath2 = strcat(bootclasspath2, "/core.jar:");
    bootclasspath2 = strcat(bootclasspath2, path);
    bootclasspath2 = strcat(bootclasspath2, "/ext.jar:");
    bootclasspath2 = strcat(bootclasspath2, path);
    bootclasspath2 = strcat(bootclasspath2, "/framework.jar:");
    bootclasspath2 = strcat(bootclasspath2, path);
    bootclasspath2 = strcat(bootclasspath2, "/android.policy.jar:");
    bootclasspath2 = strcat(bootclasspath2, path);
    bootclasspath2 = strcat(bootclasspath2, "/services.jar");
    argv2[3] = bootclasspath2;
    argv2[4] = "Foo";
    androidMain(argc2, argv2);
    int retVal = UIApplicationMain(argc, argv, nil, nil);
    [pool release];
    return retVal;
}
