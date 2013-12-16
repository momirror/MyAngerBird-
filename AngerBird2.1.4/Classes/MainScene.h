//
//  File.h
//  AngerBird
//
//  Created by msp on 13-11-12.
//
//

#ifndef __AngerBird__File__
#define __AngerBird__File__

#include <iostream>
#include "cocos2d.h"
#include "PopUpView.h"
using namespace cocos2d;

class MainScene:cocos2d::CCLayer {
    
    
public:
    ~MainScene();
    MainScene();
    
    static CCScene * scene();
    void StartGame();
    void ContinuteGame();
    void Setting();
    void cancel();
    void newGame();
private:
    PopUpView * m_pPopView;


};

#endif /* defined(__AngerBird__File__) */
