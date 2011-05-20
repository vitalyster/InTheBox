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

// CUSTOMIZE THIS
const char * MAIN_JAVA_CLASS_NAME = "Foo";
const char * JAR_NAME = "foo.jar";

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
    strcpy(foopath, path);
    strcat(foopath, "/");
    strcat(foopath, JAR_NAME);
    argv2[2] = foopath;
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
    argv2[4] = MAIN_JAVA_CLASS_NAME;
    androidMain(argc2, argv2);
    int retVal = UIApplicationMain(argc, argv, nil, nil);
    [pool release];
    return retVal;
}
