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

