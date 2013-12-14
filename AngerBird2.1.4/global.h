//
//  global.h
//  AngerBird2.1.4
//
//  Created by msp on 13-12-13.
//
//

#ifndef __AngerBird2_1_4__global__
#define __AngerBird2_1_4__global__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class global
{
public:
    static int getGameLevel();
    static void setGameLevel(int iLevel);
    static void saveGameLevel();
    static void readGameLevel();
};

#endif /* defined(__AngerBird2_1_4__global__) */
