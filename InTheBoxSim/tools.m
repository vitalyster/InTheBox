//
//  tools.m
//  InTheBox
//
//  Created by Benoit Lamarche on 16/05/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "tools.h"

const char * appDocumentsPath = NULL;
const char * appBundlePath = NULL;

const char * getAppDocumentsPath ()
{
    if (appDocumentsPath[0]=='\0')
    {
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentsDirectory = [paths objectAtIndex:0];
        appDocumentsPath = [documentsDirectory UTF8String];
    }
    return appDocumentsPath;
}

const char * getAppBundlePath ()
{
    if (appBundlePath[0]=='\0')
    {
        appBundlePath = [[[NSBundle mainBundle] resourcePath] fileSystemRepresentation];
    }
    return appBundlePath;
}

