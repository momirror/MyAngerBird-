//
//  global.cpp
//  AngerBird2.1.4
//
//  Created by msp on 13-12-13.
//
//

#include "global.h"
#include "GameScene.h"
static int m_iGameLevel;

int global::getGameLevel()
{
    return m_iGameLevel;
}

void global::setGameLevel(int iLevel)
{
    m_iGameLevel = iLevel;
}


void global::saveGameLevel()
{
    CCUserDefault::sharedUserDefault()->setIntegerForKey("GameLevel", m_iGameLevel);
    CCUserDefault::sharedUserDefault()->flush();//写入到文件中

}

void global::readGameLevel()
{
    m_iGameLevel = CCUserDefault::sharedUserDefault()->getIntegerForKey("GameLevel");
}

void global::setBackgroundMusicState(bool bOpen)
{
    CCUserDefault::sharedUserDefault()->setBoolForKey("BackgroundMusicState", bOpen);
    CCUserDefault::sharedUserDefault()->flush();//写入到文件中
}

bool global::getBackgroundMusicState()
{
    return CCUserDefault::sharedUserDefault()->getBoolForKey("BackgroundMusicState");
}

void global::setGameEffectState(bool bOpen)
{
    CCUserDefault::sharedUserDefault()->setBoolForKey("GameEffectState", bOpen);
    CCUserDefault::sharedUserDefault()->flush();//写入到文件中
}

bool global::getGameEffectState()
{
    return CCUserDefault::sharedUserDefault()->getBoolForKey("GameEffectState");
}