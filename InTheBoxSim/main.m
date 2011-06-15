//
//  main.m
//  InTheBoxSim
//
//  Created by Benoit Lamarche on 27/04/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

#include "tools.h"

#include "AndroidMain.h"

// CUSTOMIZE THIS
char * MAIN_JAVA_CLASS_NAME = "Foo";
const char * JAR_NAME = "foo.jar";

int main(void)
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    const char * path = getAppBundlePath();
    
    int argc = 5;
    char *argv[argc];
    argv[1] = "-cp";
    char foopath[PATH_MAX];
    snprintf(foopath, sizeof(foopath), "%s/%s", path, JAR_NAME);
    
    argv[2] = foopath;
    
    const char * bcpHeader = "-Xbootclasspath:";
    
    char bootclasspath[5*PATH_MAX + strlen(bcpHeader) + 5];
    snprintf(bootclasspath, sizeof(bootclasspath),
             "%s%s/core.jar:%s/ext.jar:%s/framework.jar:%s/android.policy.jar:%s/services.jar",
             bcpHeader, path, path, path, path, path);
    
    argv[3] = bootclasspath;
    argv[4] = MAIN_JAVA_CLASS_NAME;
    
    androidMain(argc, argv);
    int retVal = UIApplicationMain(0, NULL, nil, nil);
    [pool release];
    return retVal;
}
