//
//  GameScene.h
//  AngerBird
//
//  Created by msp on 13-11-8.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//
#ifndef __HELLO_WORLD_H__
#define __HELLO_WORLD_H__

// When you import this file, you import all the cocos2d classes
#include "cocos2d.h"
#include "Box2D.h"
#include "Definition.h"
#include "MyContactListener.h"
#include "PopUpView.h"
using namespace cocos2d;

#define ARM_TAG 111



class GameScene : public cocos2d::CCLayer {
public:
    ~GameScene();
    GameScene();
    


    static cocos2d::CCScene* scene();
    

    virtual void draw();
    virtual void InitPhysical();
    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
 	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    void createBullets(int count);
    bool attachBullet();
    void resetGame( ) ;
    void resetBullet();
    void tick(cocos2d::CCTime dt);
    void createTargets(int iLevel);
    void createTarget(char * imageName,CCPoint position,float rotation,bool isCircle,bool isStatic,bool isEnemy);
    bool eliminateEnery(b2Body*enemyBody);
    void backToMainMenu();
    void succed(bool bSuccess);
    void getEnemyInfo();
    void nextLevel();
    void repeatGame();
    void onEnter();
    void onExit();

    
private:
    b2World             * m_pWorld;
    b2Body              * m_pGroundBody;
    b2Fixture           * m_pArmFixture;
    b2Body              * m_pArmBody;
    b2RevoluteJoint     * m_pArmJoin;
    b2MouseJoint        * m_pMouseJoin;
    std::vector<b2Body*>  m_pBullets;
    int                   m_iCurrentBullet;
    b2Body              * m_pBulletBody;
    b2WeldJoint         * m_pBulletJoint;
    bool                  m_bReleaseArm;
    
    std::vector<b2Body *> m_pTargets;
    std::vector<b2Body *> m_pEnemies;
    MyContactListener   * m_pListener;
    int                   m_iCurrentEnemy;
    int                   m_iTotalBullets;
    PopUpView           * m_popView;
    CCArray             * m_pEnemyInfos;
};

#endif // __HELLO_WORLD_H__
