//
//  PopUpView.cpp
//  PopUpMenu
//
//  Created by msp on 13-12-11.
//
//

#include "PopUpView.h"

PopUpView::PopUpView(const char * bgImageName,const char * title,const char * message,const char * firstButtonTitle,SEL_MenuHandler  firstSel,const char * secondButtonTitle,SEL_MenuHandler secondSel,CCObject * delegate)
{
    float fTitleHeight = 2*20.0f;
    float fBtnHeight = 2*25.0f;
    this->setTouchEnabled(true);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    m_pBgSprite = CCSprite::create(bgImageName);
    m_pBgSprite->setAnchorPoint(ccp(0.5,0.5));
    m_pBgSprite->setPosition(ccp(winSize.width/2,winSize.height/2));
    this->addChild(m_pBgSprite);
    
    CCRect bgRect = m_pBgSprite->getTextureRect();
//    m_pTitleLabel = CCLabelTTF::create(title, "Arial", 15);
    CCSize titleSize = CCSizeMake(bgRect.size.width, fTitleHeight);
    m_pTitleLabel = CCLabelTTF::create(title, "Arial", 30, titleSize, kCCTextAlignmentCenter);
    
    m_pTitleLabel->setAnchorPoint(ccp(0,1));
    m_pTitleLabel->setPosition(ccp(bgRect.origin.x,bgRect.origin.y+bgRect.size.height));
    m_pBgSprite->addChild(m_pTitleLabel);
    
    CCRect titleRect = m_pTitleLabel->getTextureRect();
    CCSize MessageSize = CCSizeMake(bgRect.size.width, bgRect.size.height - 2*fTitleHeight);
    m_pMessageLabel = CCLabelTTF::create(message, "Arial", 30, MessageSize, kCCTextAlignmentCenter);
    m_pMessageLabel->setAnchorPoint(ccp(0,1));
    m_pMessageLabel->setPosition(ccp(titleRect.origin.x,bgRect.origin.y+bgRect.size.height - 1.3*fTitleHeight));
    m_pBgSprite->addChild(m_pMessageLabel);
    
//    CCSprite * btn1Bg = CCSprite::create("btn1.png");
//    btn1Bg->setAnchorPoint(ccp(0.5,0.5));
//    btn1Bg->setPosition(ccp(bgRect.origin.x + 70*2,bgRect.origin.y+fBtnHeight));
//    m_pBgSprite->addChild(btn1Bg);
    
//    CCSprite * btn2Bg = CCSprite::create("btn2.png");
//    btn2Bg->setAnchorPoint(ccp(0.5,0.5));
//    btn2Bg->setPosition(ccp(bgRect.origin.x + 200*2,bgRect.origin.y+fBtnHeight));
//    m_pBgSprite->addChild(btn2Bg);
    
    m_pFirstButtonItem = CCMenuItemImage::create("btn1.png", "btn1.png", this, menu_selector(PopUpView::OKButtonResponse));
    CCMenu * pFirstMenu = CCMenu::create(m_pFirstButtonItem,NULL);
    pFirstMenu->setPosition(ccp(bgRect.origin.x + 70*2,bgRect.origin.y+fBtnHeight));
    m_pBgSprite->addChild(pFirstMenu);
    
    m_pSecondButtonItem = CCMenuItemImage::create("btn2.png", "btn2.png", this, menu_selector(PopUpView::CancelButtonResponse));
//    create(secondButtonTitle, this, menu_selector(PopUpView::CancelButtonResponse));
    CCMenu * pSecondMenu = CCMenu::create(m_pSecondButtonItem,NULL);
    pSecondMenu->setPosition(ccp(bgRect.origin.x + 200*2,bgRect.origin.y+fBtnHeight));
    m_pBgSprite->addChild(pSecondMenu);
    
    
    m_pFirstLabel = CCLabelTTF::create(firstButtonTitle, "Arial", 30, MessageSize, kCCTextAlignmentCenter);
//    m_pFirstLabel->setAnchorPoint(ccp(0.5,0.5));
    m_pFirstLabel->setPosition(ccp(bgRect.origin.x + 70*2,bgRect.origin.y+fBtnHeight - 70));
    m_pBgSprite->addChild(m_pFirstLabel);
    
    m_pSecondLabel = CCLabelTTF::create(secondButtonTitle, "Arial", 30, MessageSize, kCCTextAlignmentCenter);
//    m_pSecondLabel->setAnchorPoint(ccp(0.5,0.5));
    m_pSecondLabel->setPosition(ccp(bgRect.origin.x + 200*2,bgRect.origin.y+fBtnHeight - 70));
    m_pBgSprite->addChild(m_pSecondLabel);

    m_firstSel = firstSel;
    m_secondSel = secondSel;
    m_pDelegate = delegate;

    
}

void PopUpView::OKButtonResponse()
{
    (m_pDelegate->*m_firstSel)(this);
}

void PopUpView::CancelButtonResponse()
{
    (m_pDelegate->*m_secondSel)(this);
}


PopUpView::~PopUpView()
{
    
}

void PopUpView::onEnter()
{
    CCLayer::onEnter();
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    
}

bool PopUpView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;//下层菜单不再响应触摸函数
}

void PopUpView::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    CCLog("popUpView touch begin");
}

void PopUpView::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    CCLog("popUpView touch moved");

}

void PopUpView::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
    CCLog("popUpView touch end");

}