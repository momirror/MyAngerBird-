//
//  SettingScene.cpp
//  AngerBird2.1.4
//
//  Created by msp on 13-12-14.
//
//

#include "SettingScene.h"
#include "MainScene.h"

SettingScene::SettingScene()
{
    CCSprite * bg = CCSprite::create("setting_bg.png");
    bg->setAnchorPoint(ccp(0,0));
    addChild(bg);
    
    
    CCMenuItemImage * backItem = CCMenuItemImage::create("back.png", "back.png");
    backItem->setTarget(this, menu_selector(SettingScene::backToMainMenu));
    CCMenu * pMenu = CCMenu::create(backItem,NULL);
    pMenu->setPosition(ccp(80,600));
    this->addChild(pMenu,111);
    
    
    ccColor3B color;
    color.r = 255;
    color.g = 0;
    color.b = 0;
    CCLabelTTF * backgroundMusicLabel = CCLabelTTF::create("背景音乐", "Marker Felt", 40);
    backgroundMusicLabel->setAnchorPoint(ccp(0,0));
    backgroundMusicLabel->setColor(color);
    backgroundMusicLabel->setPosition(ccp(40,480));
    addChild(backgroundMusicLabel);
    
    
    CCLabelTTF * gameEffectLabel = CCLabelTTF::create("音效", "Marker Felt", 40);
    gameEffectLabel->setAnchorPoint(ccp(0,0));
    gameEffectLabel->setColor(color);
    gameEffectLabel->setPosition(ccp(40,340));
    addChild(gameEffectLabel);
    
    bool backgroundMusicState = global::getBackgroundMusicState();
    
    
    CCMenuItemImage * backgroundMusicItem = CCMenuItemImage::create(backgroundMusicState?"button_on.png":"button_off.png", backgroundMusicState?"button_on.png":"button_off.png",this,menu_selector(SettingScene::SetBackgroundMusic));
    CCMenu * backgroundMenu = CCMenu::createWithItem(backgroundMusicItem);
    backgroundMenu->setAnchorPoint(ccp(0,0));
    backgroundMenu->setPosition(ccp(800,500));
    addChild(backgroundMenu);
    
    bool bGameEffectState = global::getGameEffectState();
    CCMenuItemImage * gameEffectItem = CCMenuItemImage::create(bGameEffectState?"button_on.png":"button_off.png", backgroundMusicState?"button_on.png":"button_off.png",this,menu_selector(SettingScene::SetGameEffect));
    CCMenu * gameEffectMenu = CCMenu::createWithItem(gameEffectItem);
    gameEffectMenu->setAnchorPoint(ccp(0,0));
    gameEffectMenu->setPosition(ccp(800,360));
    addChild(gameEffectMenu);
    
}

SettingScene::~SettingScene()
{
    
}

void SettingScene::SetBackgroundMusic(CCObject* pSender)
{
    bool state = global::getBackgroundMusicState();
    global::setBackgroundMusicState(!state);
    
    ((CCMenuItemImage*)pSender)->setNormalImage(CCSprite::create(!state?"button_on.png":"button_off.png"));
    
}

void SettingScene::SetGameEffect(CCObject* pSender)
{
    bool state = global::getGameEffectState();
    global::setGameEffectState(!state);
    ((CCMenuItemImage*)pSender)->setNormalImage(CCSprite::create(!state?"button_on.png":"button_off.png"));

}

CCScene* SettingScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // add layer as a child to scene
    CCLayer* layer = new SettingScene();
    scene->addChild(layer);
    layer->release();
    
    return scene;
}

void SettingScene::backToMainMenu()
{
    CCScene * scence = MainScene::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInL ::create(0.5f,scence));
    
}