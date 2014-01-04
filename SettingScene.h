//
//  SettingScene.h
//  AngerBird2.1.4
//
//  Created by msp on 13-12-14.
//
//

#ifndef __AngerBird2_1_4__SettingScene__
#define __AngerBird2_1_4__SettingScene__

#include <iostream>
#include "cocos2d.h"
#include "global.h"

class SettingScene : public cocos2d::CCLayer {
public:
    SettingScene();
    ~SettingScene();
    void SetBackgroundMusic(CCObject* pSender);
    void SetGameEffect(CCObject* pSender);
    static cocos2d::CCScene* scene();
    void backToMainMenu();
};

#endif /* defined(__AngerBird2_1_4__SettingScene__) */
