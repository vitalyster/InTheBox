//
//  tools.m
//  InTheBox
//
//  Created by Benoit Lamarche on 16/05/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "tools.h"


@implementation tools

- (void) getDocumentsPath {
     NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
     NSString *documentsDirectory = [paths objectAtIndex:0];
     NSLog(@"*** FlexyCore");
     NSLog(documentsDirectory);
     //return documentsDirectory;
}

char * getDocumentsPath2 ()
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    const char * cpath = [documentsDirectory cString];
    return cpath;
}

@end
