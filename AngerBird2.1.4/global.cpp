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