//
//  tools.m
//  InTheBox
//
//  Created by Benoit Lamarche on 16/05/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "toolsCinterface.h"


char * getAppDocumentsPath ()
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    const char * cpath = [documentsDirectory UTF8String];
    [pool release];
    return cpath;
}

