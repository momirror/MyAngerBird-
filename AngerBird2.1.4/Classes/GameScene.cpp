//
//  GameScene.cpp
//  AngerBird
//
//  Created by msp on 13-11-8.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "MainScene.h"
#include "global.h"

using namespace CocosDenshion;


#define PTM_RATIO 32
#define FLOOR_HEIGHT 2*62.0f

enum
{
	kTagTileMap = 1,
	kTagSpriteManager = 1,
	kTagAnimation1 = 1,
}; 

GameScene::GameScene()
{
	this->setTouchEnabled( true );
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    this->InitPhysical();
    
    CCSprite * pBg = CCSprite::create("bg.png");
    pBg->setAnchorPoint(CCPointZero);
    pBg->setPosition(CCPointMake(0.0, 0.0));
    this->addChild(pBg,-1);
    
    CCSprite * pBase2 = CCSprite::create("catapult_base_2.png");
    pBase2->setAnchorPoint(CCPointZero);
    pBase2->setPosition(ccp(2*181.0f,FLOOR_HEIGHT));
    this->addChild(pBase2,0);
    
    CCSprite * pSquirrelLeft = CCSprite::create("squirrel_1.png");
    pSquirrelLeft->setAnchorPoint(CCPointZero);
    pSquirrelLeft->setPosition(ccp(2*11.0f,FLOOR_HEIGHT));
    this->addChild(pSquirrelLeft,0);
    
    CCSprite * pBase1 = CCSprite::create("catapult_base_1.png");
    pBase1->setAnchorPoint(CCPointZero);
    pBase1->setPosition(ccp(2*181.0f,FLOOR_HEIGHT));
    this->addChild(pBase1,9);
    
    CCSprite * pSquirrelRight = CCSprite::create("squirrel_2.png");
    pSquirrelRight->setAnchorPoint(CCPointZero);
    pSquirrelRight->setPosition(ccp(2*240.0f,FLOOR_HEIGHT));
    this->addChild(pSquirrelRight,9);
    
    
    
    
    //地面
    CCSprite * pIceGround = CCSprite::create("fg.png");
    pIceGround->setAnchorPoint(CCPointZero);
    this->addChild(pIceGround,10);
    
    
//    b2BodyDef bodyDef;
//    bodyDef.type = b2_staticBody;
//    
//    //以精灵中心为位置
//    bodyDef.position.Set((pIceGround->getPositionX() + pIceGround->getContentSize().width/2.0)/PTM_RATIO, (pIceGround->getPositionY() + pIceGround->getContentSize().
//                                                                                       height/2.0)/PTM_RATIO);
//    bodyDef.userData = pIceGround;
//    
//    b2Body * body = m_pWorld->CreateBody(&bodyDef);
//    
//    CCLog("%f,%f",body->GetPosition().x*PTM_RATIO,body->GetPosition().y*PTM_RATIO);
//    b2FixtureDef fixtureDef;
//    
//    b2PolygonShape box;
//    fixtureDef.shape = &box;
//    fixtureDef.density = 1.0f;//密度
//    fixtureDef.friction = 0.2f;//摩擦力
//    box.SetAsBox(pIceGround->getContentSize().width/2.0/PTM_RATIO, pIceGround->getContentSize().height/2.0/PTM_RATIO);
//    body->CreateFixture(&box,0.5f);

    

	CCSprite * pArm = CCSprite::create("catapult_arm.png");
    this->addChild(pArm,1);
    pArm->setTag(ARM_TAG);
    
    CCMenuItemImage * backItem = CCMenuItemImage::create("back.png", "back.png");
    backItem->setTarget(this, menu_selector(GameScene::backToMainMenu));
    CCMenu * pMenu = CCMenu::create(backItem,NULL);
    pMenu->setPosition(ccp(2*40,2*300));
    this->addChild(pMenu,111);
    
    
    
    b2BodyDef armBodyDef;
    armBodyDef.type = b2_dynamicBody;
    armBodyDef.linearDamping = 1;//线性阻尼
    armBodyDef.angularDamping = 1;//角阻抗
    armBodyDef.position.Set(2*230.0/PTM_RATIO, (FLOOR_HEIGHT + 2*91.0f)/PTM_RATIO);
    armBodyDef.userData = pArm;
    m_pArmBody = m_pWorld->CreateBody(&armBodyDef);
    
    b2PolygonShape armBox;
    b2FixtureDef armBoxDef;
    armBoxDef.shape = &armBox;
    armBoxDef.density = 0.3f;
    armBox.SetAsBox(2*11.0f/PTM_RATIO, 2*91.0f/PTM_RATIO);
    m_pArmFixture = m_pArmBody->CreateFixture(&armBoxDef);
    
    //旋转关节
    b2RevoluteJointDef armJoinDef;
    armJoinDef.Initialize(m_pGroundBody, m_pArmBody, b2Vec2(2*233.0f/PTM_RATIO, FLOOR_HEIGHT/PTM_RATIO));
    armJoinDef.enableMotor = true;
    armJoinDef.enableLimit = true;
    armJoinDef.motorSpeed = -10*2;
    armJoinDef.lowerAngle = CC_DEGREES_TO_RADIANS(9);
    armJoinDef.upperAngle = CC_DEGREES_TO_RADIANS(75);
    armJoinDef.maxMotorTorque = 2*900;//就是当关节被旋转后，使关节恢复的力，类似弹力。这个力越小，越容易旋转，恢复力也就越小。
    m_pArmJoin = (b2RevoluteJoint*)m_pWorld->CreateJoint(&armJoinDef);
    
    m_pMouseJoin = NULL;
    m_bReleaseArm = false;
    
    CCDelayTime * delayTime = CCDelayTime::create(0.2f);
    CCCallFunc * callFunc = CCCallFunc::create(this, callfunc_selector(GameScene::resetGame));
    this->runAction(CCSequence::create(delayTime,callFunc,NULL));
    
    getEnemyInfo();
    createTargets(global::getGameLevel());
    
    m_pListener = new MyContactListener();
    m_pWorld->SetContactListener(m_pListener);
    m_pBulletBody = NULL;
    

    
    
	schedule( schedule_selector(GameScene::tick) );
    
}


GameScene::~GameScene()
{
	delete m_pWorld;
	m_pWorld = NULL;
	
    delete m_pEnemyInfos;
    m_pEnemyInfos = NULL;
	//delete m_debugDraw;
}

void GameScene::InitPhysical()
{
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    b2Vec2 gravity;
    bool bSleep = true;
    gravity.Set(0.0f, -10.0f);
    m_pWorld = new b2World(gravity);
    m_pWorld->SetAllowSleeping(bSleep);

//    m_pWorld->SetContinuousPhysics(true);
    
    b2BodyDef groundDef;
    groundDef.position.Set(0,0);
    
    m_pGroundBody = m_pWorld->CreateBody(&groundDef);
    
    //边界
    b2PolygonShape groundBox;//多边形形状
    
    b2EdgeShape screenBorderShape;
    

    screenBorderShape.Set(b2Vec2(0,FLOOR_HEIGHT/PTM_RATIO), b2Vec2(2*screenSize.width*3/PTM_RATIO,
                                                                   FLOOR_HEIGHT/PTM_RATIO));//下边界
    b2Fixture * fixture = m_pGroundBody->CreateFixture(&screenBorderShape, 1);
//    fixture->SetFriction(1.0f);//给地面一点摩擦力
    
    screenBorderShape.Set(b2Vec2(0,2*screenSize.height/PTM_RATIO), b2Vec2(0,FLOOR_HEIGHT/PTM_RATIO));//左边界
    m_pGroundBody->CreateFixture(&screenBorderShape, 0);

    
    screenBorderShape.Set(b2Vec2(0,2*screenSize.height/PTM_RATIO), b2Vec2(2*screenSize.width*3/PTM_RATIO,
                                                                                                                                              2*screenSize.height/PTM_RATIO));//上边界
    m_pGroundBody->CreateFixture(&screenBorderShape, 0);

    
    screenBorderShape.Set(b2Vec2(2*screenSize.width*3/PTM_RATIO,screenSize.height/PTM_RATIO), b2Vec2(2*screenSize.width*3/PTM_RATIO,
                                                                                                                                              FLOOR_HEIGHT/PTM_RATIO));//右边界
    m_pGroundBody->CreateFixture(&screenBorderShape, 0);

    
}

void GameScene::draw()
{
//	glDisable(GL_TEXTURE_2D);
//	glDisableClientState(GL_COLOR_ARRAY);
//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//	
//	glEnable(GL_TEXTURE_2D);
//	glEnableClientState(GL_COLOR_ARRAY);
//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);	
}


void GameScene::tick(CCTime dt)
{
		
	int velocityIterations = 8;
	int positionIterations = 1;
    
	
	m_pWorld->Step(1/40.0, velocityIterations, positionIterations);
	
    //更新位置和角度
    for (b2Body* b = m_pWorld->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != NULL)
        {
            CCSprite * actor = (CCSprite*)b->GetUserData();
            actor->setPosition(ccp(b->GetPosition().x*PTM_RATIO,b->GetPosition().y*PTM_RATIO));
            actor->setRotation(-1*CC_RADIANS_TO_DEGREES(b->GetAngle()));
    
        }	
	}
    
    
    //发射子弹
    if(m_bReleaseArm && m_pBulletJoint != NULL)
    {
        if(m_pArmJoin->GetJointAngle() <= CC_DEGREES_TO_RADIANS(10))//差不多回到原始位置再发射子弹
        {
            m_bReleaseArm = false;
            m_pWorld->DestroyJoint(m_pBulletJoint);
            m_pBulletJoint = NULL;
            
            CCDelayTime *delayAction = CCDelayTime::create(5.0f);
            CCCallFunc *callSelectorAction = CCCallFunc::create(this, callfunc_selector(GameScene::resetBullet));
            this->runAction(CCSequence::create(delayAction, callSelectorAction, NULL));
//            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("fly.mp3");
            
            m_iCurrentBullet++;
            
        }
    }
    
    //镜头跟随
    if(m_pBulletBody && m_pBulletJoint == NULL)
    {
        b2Vec2 position = m_pBulletBody->GetPosition();
        CCPoint myPosition = this->getPosition();
        CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
        
        if(position.x > screenSize.width/2.0f/PTM_RATIO)
        {
            myPosition.x = -min(screenSize.width, position.x*PTM_RATIO);
            this->setPosition(myPosition);
        }
    }
    
    //消灭敌人
    std::set<b2Body*>::iterator pos;
    for(pos = m_pListener->contracts.begin();pos != m_pListener->contracts.end();)
    {
        b2Body * body = *pos;
        CCSprite * enemy = (CCSprite*)body->GetUserData();
        
        
        //爆炸效果
        CCPoint position = ccp(body->GetWorldCenter().x*PTM_RATIO,body->GetWorldCenter().y*PTM_RATIO);
        CCParticleSun *explosion = CCParticleSun::create();
		explosion->setTexture(CCTextureCache::sharedTextureCache()->addImage("fire.png"));
		explosion->initWithTotalParticles(10);
        explosion->setAutoRemoveOnFinish(true);
		explosion->setStartSizeVar(10.0f);
		explosion->setSpeed(70.0f);
		explosion->setAnchorPoint(ccp(0.5f, 0.5f));
		explosion->setPosition(position);
		explosion->setDuration(1.0f);
		addChild(explosion, 11);


        

        //移除
        this->removeChild(enemy, true);
        m_pWorld->DestroyBody(body);
        body = NULL;
        
        m_pListener->contracts.erase(*pos);
        pos++;
        
        m_iCurrentEnemy--;
        
        
        if(global::getGameEffectState())
        {
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("miao.wav");
        }

        
    }
    
  
}

#pragma mark - touch

void GameScene::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
        
   if(m_pMouseJoin != NULL)
   {
       return;
   }
    
    CCTouch * touch = (CCTouch*)pTouches->anyObject();
    CCPoint location = touch->getLocationInView();//??
    location = CCDirector::sharedDirector()->convertToGL(location);
    b2Vec2 locationWorld = b2Vec2(location.x/PTM_RATIO,location.y/PTM_RATIO);
    if(locationWorld.x < m_pArmBody->GetWorldCenter().x + 2*50.0f/PTM_RATIO && locationWorld.x > m_pArmBody->GetWorldCenter().x - 2*50.0f/PTM_RATIO)
    {
        b2MouseJointDef mouseJoinDef;
        mouseJoinDef.bodyA = m_pGroundBody;
        mouseJoinDef.bodyB = m_pArmBody;
        mouseJoinDef.target = locationWorld;
        mouseJoinDef.maxForce = 2000;//施加给加节的力。只有当它大小旋转关节上的maxMotorTorque，才能使关切旋转
        m_pMouseJoin = (b2MouseJoint*)m_pWorld->CreateJoint(&mouseJoinDef);
    }
    else if(locationWorld.x > m_pArmBody->GetWorldCenter().x + 2*50.0f/PTM_RATIO)
    {
        //镜头移动
        CCFiniteTimeAction * action1 = CCMoveTo::create(1.5f, ccp(2*-960.0f, 0.0f));
        CCDelayTime *action3 = CCDelayTime::create(1.0f);
        CCFiniteTimeAction *action4 = CCMoveTo::create(1.5f, CCPointZero);
        runAction(CCSequence::create(action1,action3, action4, NULL));
    }
}

void GameScene::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
//        CCLog("touch moved");
    if(m_pMouseJoin == NULL)
    {
        return;
    }
    
    CCTouch * touch = (CCTouch*)pTouches->anyObject();
    CCPoint location = touch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    b2Vec2 locationWorld = b2Vec2(location.x/PTM_RATIO,location.y/PTM_RATIO);
    
    m_pMouseJoin->SetTarget(locationWorld);
}

void GameScene::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
//    CCLOG("touch end");
    if(m_pMouseJoin != NULL)
    {
        if(m_pArmJoin->GetJointAngle() >= CC_DEGREES_TO_RADIANS(5))//偏离大于5度才发射子弹
        {
            m_bReleaseArm = true;
        }
        
        m_pWorld->DestroyJoint(m_pMouseJoin);
        m_pMouseJoin = NULL;
    }
}

#pragma mark -

CCScene* GameScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // add layer as a child to scene
    CCLayer* layer = new GameScene();
    scene->addChild(layer);
    layer->release();
    
    return scene;
}

void GameScene::createBullets(int count)
{
    m_iCurrentBullet = 0;
    m_iTotalBullets = count;
    float pos = 62.0f;//第一颗子弹的x位置
    
    if(count > 0)
    {
        //delta 是子弹排列的间隔
        //165是最后一颗子弹的x位置
        //30是子弹大小
        
        float fDelta = (count > 1)?((165.0f - 62.0f - 30)/(count-1)):0;
        for(int i = 0;i < count;i++,pos += fDelta)
        {
            
            CCSprite * pBullet = CCSprite::create("acorn.png");
            this->addChild(pBullet,1);
            
            b2BodyDef bulletBodyDef;
            bulletBodyDef.type = b2_dynamicBody;
            bulletBodyDef.bullet = true;
            bulletBodyDef.position.Set(pos/PTM_RATIO, (FLOOR_HEIGHT+15.0)/PTM_RATIO);
            bulletBodyDef.userData = pBullet;
            b2Body * bulletBody = m_pWorld->CreateBody(&bulletBodyDef);
            bulletBody->SetActive(false);
            
            b2CircleShape circle;
            circle.m_radius = 15.0/PTM_RATIO;
            
            b2FixtureDef ballShapeDef;
            ballShapeDef.shape = &circle;
            ballShapeDef.density = 1.8f;
            ballShapeDef.restitution = 0.2f;
            ballShapeDef.friction = 0.99f;
            bulletBody->CreateFixture(&ballShapeDef);
            
            m_pBullets.push_back(bulletBody);
            
            
        }
    }
}


bool GameScene::attachBullet()
{
    if(m_iCurrentBullet < m_pBullets.size())
    {
        m_pBulletBody = (b2Body*)m_pBullets.at(m_iCurrentBullet);
        m_pBulletBody->SetTransform(b2Vec2(2*230.0f/PTM_RATIO, (2*155.0f+FLOOR_HEIGHT)/PTM_RATIO), 0.0f);
        m_pBulletBody->SetActive(true);
        
        b2WeldJointDef weldJointDef;
        weldJointDef.Initialize(m_pBulletBody, m_pArmBody, b2Vec2(2*230.0f/PTM_RATIO,(2*155.0f+FLOOR_HEIGHT)/PTM_RATIO));
        weldJointDef.collideConnected = false;
        
        m_pBulletJoint = (b2WeldJoint*)m_pWorld->CreateJoint(&weldJointDef);
        return true;
    }
    
    return false;
}


void GameScene::resetGame( )
{
    CCArray * pArray  = (CCArray*)m_pEnemyInfos->objectAtIndex(global::getGameLevel());
    CCDictionary * pDic =  (CCDictionary*)pArray->objectAtIndex(0);
    int iBulletNum = pDic->valueForKey("BulletNumber")->intValue();
    
    
    this->createBullets(iBulletNum);
    this->attachBullet();
    
   
    
    //镜头移动
    CCFiniteTimeAction * action1 = CCMoveTo::create(1.5f, ccp(2*-480.0f, 0.0f));
    CCDelayTime *action3 = CCDelayTime::create(1.0f);
    CCFiniteTimeAction *action4 = CCMoveTo::create(1.5f, CCPointZero);
    runAction(CCSequence::create(action1,action3, action4, NULL));

}


void GameScene::resetBullet()
{
    if(!this->attachBullet())
    {
        m_pBulletBody->SetTransform(b2Vec2(0, 0), 0);
        m_pBulletBody->SetActive(false);

    }
    
    CCAction *action = CCMoveTo::create(0.2f, CCPointZero);
    runAction(action);
    
    if(m_iCurrentEnemy == 0)
    {
        succed(true);
    }
    else if(m_iTotalBullets != 0 && m_iCurrentBullet == m_iTotalBullets)
    {
        succed(false);
    }

    
}


void GameScene::createTargets(int iLevel)
{
//    if(iLevel >= m_pEnemyInfos->count())
//    {
//        iLevel = 0;
//        global::setGameLevel(iLevel);
//        
//        m_popView = new PopUpView("popUpBGg.png", "Yeah", "你已经通关，真厉害～", "OK",menu_selector(GameScene::backToMainMenu), "Cancel",menu_selector(GameScene::backToMainMenu),this);
//        this->addChild(m_popView,20);
//        return;
//    }
    
    CCDictionary * pEnemyNumDic = (CCDictionary*)((CCArray*)m_pEnemyInfos->objectAtIndex(iLevel))->objectAtIndex(0);
    m_iCurrentEnemy = pEnemyNumDic->valueForKey("EnemyNumber")->intValue();
    
    CCArray * pEnemys = (CCArray*)((CCArray*)m_pEnemyInfos->objectAtIndex(iLevel))->objectAtIndex(1);
    
    for(int i = 0;i < pEnemys->count();i++)
    {
        CCDictionary * pEnemyDic = (CCDictionary*)pEnemys->objectAtIndex(i);
        CCString * imageName = (CCString*)pEnemyDic->objectForKey("imageName");
        float positionX = pEnemyDic->valueForKey("positionX")->floatValue();
        float positionY = pEnemyDic->valueForKey("positionY")->floatValue();
        float rotation = pEnemyDic->valueForKey("rotation")->floatValue();
        bool isCircle = pEnemyDic->valueForKey("isCircle")->boolValue();
        bool isStatic = pEnemyDic->valueForKey("isStatic")->boolValue();
        bool isEnemy = pEnemyDic->valueForKey("isEnemy")->boolValue();
        
    //    CCLog("imageName->%s",imageName->getCString());
    //    CCLog("positionX->%f,positionY->%f",positionX,positionY);
        
        createTarget((char*)imageName->getCString(), CCPointMake(positionX, positionY), rotation, isCircle, isStatic, isEnemy);
    }

}


void GameScene::createTarget(char * imageName,CCPoint position,float rotation,bool isCircle,bool isStatic,bool isEnemy)
{
    CCSprite * sprite = CCSprite::create(imageName);
    this->addChild(sprite,20);
    if(isEnemy)
    {
        sprite->setTag(ENEMY_TAT);
    }
    
    
    
    b2BodyDef bodyDef;
    bodyDef.type = isStatic?b2_staticBody:b2_dynamicBody;

    //以精灵中心为位置
    bodyDef.position.Set((position.x + sprite->getContentSize().width/2.0)/PTM_RATIO, (position.y + sprite->getContentSize().
                                                                                       height/2.0)/PTM_RATIO);
    CCLog("%f,%f",bodyDef.position.x,bodyDef.position.y);
    bodyDef.angle = CC_DEGREES_TO_RADIANS(rotation);
    bodyDef.userData = sprite;

    b2Body * body = m_pWorld->CreateBody(&bodyDef);
    
    CCLog("%f,%f",body->GetPosition().x,body->GetPosition().y);
    b2FixtureDef fixtureDef;
    b2Fixture * fixture;

    if(isCircle)
    {
        b2CircleShape circle;
        fixtureDef.shape = &circle;
        fixtureDef.density = 1.0f;//密度
        fixtureDef.friction = 0.2f;//摩擦力
        circle.m_radius = sprite->getContentSize().width/2.0/PTM_RATIO;
        fixture = body->CreateFixture(&circle,0.5);
//        m_pTargets.push_back(body);
    }
    else
    {
        b2PolygonShape box;
        fixtureDef.shape = &box;
        fixtureDef.density = 1.0f;//密度
        fixtureDef.friction = 0.2f;//摩擦力
        box.SetAsBox(sprite->getContentSize().width/2.0/PTM_RATIO, sprite->getContentSize().height/2.0/PTM_RATIO);
        body->CreateFixture(&box,0.5f);
//        m_pTargets.push_back(body);
    }
    
    if(isEnemy)
    {
//        fixture->SetUserData((void*)1);//??
        m_pEnemies.push_back(body);
    }
    
}


bool GameScene::eliminateEnery(b2Body*enemyBody)
{
    CCLog("%d",m_pWorld->GetBodyCount());
    for(b2Body * body = m_pWorld->GetBodyList();body;body = body->GetNext())
    {
        if(enemyBody == body)
        {
            CCSprite * sprite = (CCSprite*)body->GetUserData();
            CCLog("%d,%d",sprite->getTag(),sprite->retainCount());
            this->removeChild(sprite, false);
            sprite = NULL;
            
            m_pWorld->DestroyBody(body);
            body = NULL;
            
            break;
        }
    }
    
 
    
    return true;
}


void GameScene::backToMainMenu()
{
    if(m_popView!= NULL)
    {
        this->removeChild(m_popView);
        m_popView = NULL;
    }
    
    global::saveGameLevel();
    
    if(global::getBackgroundMusicState())
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    }

    
    CCScene * scence = MainScene::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionJumpZoom::create(2.0f,scence));
    
}


void GameScene::succed(bool bSuccess)
{
    if(bSuccess)
    {
        global::setGameLevel(global::getGameLevel() + 1);
        
        CCParticleSystem * pExplosion = cocos2d::CCParticleSystemQuad::create("congratulation.plist") ;//create("33.plist");
        this->addChild(pExplosion,100);
        
        if(global::getGameEffectState())
        {
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("succece.mp3");
        }
        
        if(global::getGameLevel() < m_pEnemyInfos->count() - 1)
        {
            
            m_popView = new PopUpView("popUpBGg.png", "Yeah", "恭喜你，挑战下一关吧～", "OK",menu_selector(GameScene::nextLevel), "Cancel",menu_selector(GameScene::backToMainMenu),this);
        }
        else
        {
            m_popView = new PopUpView("popUpBGg.png", "Yeah", "你已经通关，真厉害～", "OK",menu_selector(GameScene::backToMainMenu), "Cancel",menu_selector(GameScene::backToMainMenu),this);
        }
        this->addChild(m_popView,20);
        
    }
    else
    {
        m_popView = new PopUpView("popUpBGg.png", "唉", "你失败了，再来一局吧！", "好",menu_selector(GameScene::repeatGame), "取消",menu_selector(GameScene::backToMainMenu),this);
        this->addChild(m_popView,20);
    }
    

   

   

}


void GameScene::getEnemyInfo()
{
    m_pEnemyInfos = CCArray::createWithContentsOfFile("enemy.plist");
//    CCArray * pArray = (CCArray *)m_pEnemyInfos->objectAtIndex(0);
//    CCDictionary * pDic = (CCDictionary*)pArray->objectAtIndex(0);
//    CCLog("EnemyNumber->%d",((CCInteger*)pDic->valueForKey("EnemyNumber"))->getValue());
    
    
    
    m_pEnemyInfos->retain();
}


void GameScene::nextLevel()

{
    this->removeChild(m_popView, true);
    
    CCScene * scene = GameScene::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2, scene)); //淡出淡入切换

}

void GameScene::repeatGame()
{
    if(global::getBackgroundMusicState())
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    }

    
    this->removeChild(m_popView, true);
    CCScene * scene = GameScene::scene();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2, scene)); //淡出淡入切换

}


void GameScene::onEnter()
{
    CCLayer::onEnter();//父类一定要执行
    
    if(global::getBackgroundMusicState())
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("backgroundMusic.mp3", true);
    }
}


void GameScene::onExit()
{
    CCLayer::onExit();
//    if(global::getBackgroundMusicState())
//    {
//        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
//    }
}