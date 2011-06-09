//
//  tools.h
//  InTheBox
//
//  Created by Benoit Lamarche on 16/05/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef _TOOLS
#define _TOOLS

#ifdef __OBJC__
    #import <Foundation/Foundation.h>
#endif

const char * getAppDocumentsPath (void);

const char * getAppBundlePath (void);

#endif