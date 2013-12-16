
//
//  File.cpp
//  AngerBird
//
//  Created by msp on 13-11-12.
//
//

#include "MainScene.h"
#include "GameScene.h"
#include "global.h"
#include "SettingScene.h"

MainScene::~MainScene()
{
    
}

MainScene::MainScene()
{
    CCSprite * bg = CCSprite::create("mainSenceBg.png");
    bg->setAnchorPoint(ccp(0,0));
    this->addChild(bg,-1);
//
//    CCSprite * itemSprite = CCSprite::create("ball.png");
//    CCSprite * itemSprite2 = CCSprite::create("ball.png");
//    CCSprite * itemSprite3 = CCSprite::create("ball.png");
//    CCMenuItemSprite * item = CCMenuItemSprite::itemFromNormalSprite(itemSprite, itemSprite2, itemSprite3, this, menu_selector(MainScene::StartGame));
//    
//    CCMenu * menu = CCMenu::menuWithItem(item);
//    menu->alignItemsHorizontally();
//    menu->setPosition(ccp(100,100));
//    this->addChild(menu,0);
//    CCLog("%f,%f",menu->getPosition().x,menu->getPosition().y);
    
//    CCLabelTTF * pStartLabel = CCLabelTTF::create("开始", "Marker Felt", 20);
//    pStartLabel->setColor(ccc3(22,22,22));
//    CCMenuItemLabel * pStartItem = CCMenuItemLabel::create(pStartLabel, this, menu_selector(MainScene::StartGame));
//    
//    CCLabelTTF * pContinuteLabel = CCLabelTTF::create("继续", "Marker Felt", 20);
//    pContinuteLabel->setColor(ccc3(22,22,22));    
//    CCMenuItemLabel * pContinuteItem = CCMenuItemLabel::create(pContinuteLabel, this, menu_selector(MainScene::ContinuteGame));
//    
//    CCLabelTTF * pSettingLabel = CCLabelTTF::create("设置", "Marker Felt", 20);
//    pSettingLabel->setColor(ccc3(22,22,22));    
//    CCMenuItemLabel * pSettingItem = CCMenuItemLabel::create(pSettingLabel, this, menu_selector(MainScene::Setting));
//    
//    CCMenu * pMenu = CCMenu::create(pStartItem,pContinuteItem,pSettingItem,NULL);
//    pMenu->alignItemsVertically();
//    this->addChild(pMenu);
    
//    CCSprite * sprite = CCSprite::create("button_Start.png");
//    sprite->setPosition(ccp(100,200));
//    this->addChild(sprite);
//    
//    CCSprite * sprite2 = CCSprite::create("button_Setting.png");
//    sprite2->setPosition(ccp(100,300));
//    this->addChild(sprite2);
    
//    CCMenuItemImage * pStartItem = CCMenuItemImage::create("button_Start.png", "button_Start.png");
    CCMenuItemImage * pStartItem = CCMenuItemImage::create("button_Start.png", "button_Start__Selected.png", this, menu_selector(MainScene::StartGame));
    CCMenu * StartMenu = CCMenu::createWithItem(pStartItem);
    StartMenu->setPosition(ccp(400,260));
    StartMenu->setTouchPriority(1);
    this->addChild(StartMenu);
    
    
    CCMenuItemImage * pContinuteItem = CCMenuItemImage::create("button_Continute.png", "button_Continute_Selected.png", this, menu_selector(MainScene::ContinuteGame));
    CCMenu * pContinuteMenu = CCMenu::createWithItem(pContinuteItem);
    pContinuteMenu->setPosition(ccp(370,180));
    pContinuteMenu->setTouchPriority(1);
    this->addChild(pContinuteMenu);
    
    
    CCMenuItemImage * pSettingItem = CCMenuItemImage::create("button_Setting.png", "button_Setting_Selected.png", this, menu_selector(MainScene::Setting));
    CCMenu * pSettingMenu = CCMenu::createWithItem(pSettingItem);
    pSettingMenu->setPosition(ccp(420,120));
    pSettingMenu->setTouchPriority(1);
    this->addChild(pSettingMenu);
    
    
}

CCScene* MainScene::scene()
{
    CCScene * scene = CCScene::create();
    CCLayer * layer = new MainScene();
    scene->addChild(layer);
    layer->release();
    
    return scene;
}

void MainScene::StartGame()
{
    if(global::getGameLevel() != 0)
    {
        m_pPopView = new PopUpView("popUpBGg.png", "", "你真的是从头开始吗？这将会覆盖之前的游戏进度？", "开始",menu_selector(MainScene::newGame), "取消",menu_selector(MainScene::cancel),this);
        this->addChild(m_pPopView);
    }
    else
    {
        global::setGameLevel(0);
        CCScene * scene = GameScene::scene();
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, scene)); //淡出淡入切换
    }
    
}


void MainScene::ContinuteGame()
{
    global::readGameLevel();
    CCScene * scene = GameScene::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, scene)); //淡出淡入切换
}


void MainScene::Setting()
{
    CCScene * scene = SettingScene::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInR::create(0.5, scene));//从左进入推出切换
}

void MainScene::cancel()
{
    this->removeChild(m_pPopView, true);
}

void MainScene::newGame()
{
    this->removeChild(m_pPopView, true);
    
    global::setGameLevel(0);
    CCScene * scene = GameScene::scene();
    //    CCDirector::sharedDirector()->replaceScene(scene);//正常切换
    
    //    CCDirector::sharedDirector()->replaceScene(CCTransitionJumpZoom::transitionWithDuration(2,scene));//弹跳切换
    
    //    CCDirector::sharedDirector()->replaceScene(CCTransitionFlipX::transitionWithDuration(2, scene));//X轴翻转切换
    
    //    CCDirector::sharedDirector()->replaceScene(CCTransitionFlipY::transitionWithDuration(2, scene));//Y轴翻转切换
    
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, scene)); //淡出淡入切换
    
    //    CCDirector::sharedDirector()->replaceScene(CCTransitionShrinkGrow::transitionWithDuration(2, scene));//交错切换
    
    //     CCDirector::sharedDirector()->replaceScene(CCTransitionMoveInL::transitionWithDuration(2, scene));//从左进入覆盖切换
    
    //    CCDirector::sharedDirector()->replaceScene(CCTransitionSlideInL::transitionWithDuration(2, scene));//从左进入推出切换
}