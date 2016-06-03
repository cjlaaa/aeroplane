#include "ChooseLevel.h"
#include "BackGround.h"
#include "GameScene.h"

CCScene* ChooseLevel::CreateScene()
{
	do 
	{
		CCScene* pScene = CCScene::create();
		CC_BREAK_IF(pScene==NULL);

		ChooseLevel* pLayer = ChooseLevel::create();
		CC_BREAK_IF(pLayer==NULL);

		pScene->addChild(pLayer,enZOrderFront);

		BackGround* pBackGround = BackGround::CreateBackGround("BackGround.png");
		CC_BREAK_IF(pBackGround==NULL);
		pBackGround->setPosition(ccp(_SCREEN_WIDTH_*0.5f,_SCREEN_HEIGHT_*1.f));
		pScene->addChild(pBackGround,enZOrderBack,enTagBackGround);
		
		return pScene;
	} while (false);
	CCLog("Fun ChooseLevel::CreateScene Error!");
	return NULL;
}

bool ChooseLevel::init()
{
	do 
	{	
		CC_BREAK_IF(CCLayer::init()==false);
		
		setTouchEnabled(true);
		setTouchMode(kCCTouchesOneByOne);

		setKeypadEnabled(true);

		schedule(schedule_selector(ChooseLevel::Update));
		
		CCSize WinSize = CCDirector::sharedDirector()->getWinSize();

		CCSprite* pLevel1 = CCSprite::create("ChooseLevel1.png"); 
		CC_BREAK_IF(pLevel1==NULL);
		pLevel1->setPosition(ccp(WinSize.width*0.5,WinSize.height*0.5));
		addChild(pLevel1,enZOrderFront,enTagLevel1);

		CCSprite* pLevel2 = CCSprite::create("ChooseLevel2.png"); 
		CC_BREAK_IF(pLevel2==NULL);
		pLevel2->setPosition(ccp(pLevel1->getPositionX() + _SCREEN_WIDTH_,WinSize.height*0.5));
		addChild(pLevel2,enZOrderFront,enTagLevel2);

		CCSprite* pLevel3 = CCSprite::create("ChooseLevel3.png"); 
		CC_BREAK_IF(pLevel3==NULL);
		pLevel3->setPosition(ccp(pLevel2->getPositionX() + _SCREEN_WIDTH_,WinSize.height*0.5));
		addChild(pLevel3,enZOrderFront,enTagLevel3);

		CCSprite* pLevel4 = CCSprite::create("ChooseLevel4.png"); 
		CC_BREAK_IF(pLevel4==NULL);
		pLevel4->setPosition(ccp(pLevel3->getPositionX() + _SCREEN_WIDTH_,WinSize.height*0.5));
		addChild(pLevel4,enZOrderFront,enTagLevel4);

		CCSprite* pLevel5 = CCSprite::create("ChooseLevel5.png"); 
		CC_BREAK_IF(pLevel5==NULL);
		pLevel5->setPosition(ccp(pLevel4->getPositionX() + _SCREEN_WIDTH_,WinSize.height*0.5));
		addChild(pLevel5,enZOrderFront,enTagLevel5);

		CurrentChooseLevel = 1;
		m_fDistance = 0;
		pMove = CCMoveTo::create(0.5,ccp(0,0));
		 
		return true;
	} while (false);
	CCLog("Fun ChooseLevel::init Error!");
	return false;
}

bool ChooseLevel::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	stopAllActions();
	return true;
}

void ChooseLevel::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	m_fDistance += pTouch->getLocation().x - pTouch->getPreviousLocation().x;
	setPositionX(getPositionX() + pTouch->getLocation().x - pTouch->getPreviousLocation().x);
}

//遇到的问题主要是滑动距离的清零问题。每个判断里都要清零！
void ChooseLevel::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	//test 直接跳转到第4关
	/*CCMoveTo* pMove = CCMoveTo::create(1,ccp(-_SCREEN_WIDTH_*3,0));
	runAction(pMove);*/

	//如果移动距离小于某值则为点击，如果未点击关卡精灵，则返回当前应显示的地方
	if (fabs(m_fDistance)<10)
	{
		CCSprite* pLevel1 = dynamic_cast<CCSprite*>(getChildByTag(enTagLevel1));
		//动作进行过程中，实际上精灵已经归位，所以会出现点空白地方进入关卡的现象
		//需要将ptouch坐标系从世界坐标转换成节点（层）坐标，就不再需要当前关卡判定了
		if (pLevel1->boundingBox().containsPoint(this->convertToNodeSpace(pTouch->getLocation()))/* && CurrentChooseLevel==1*/)
		{
			CCLog("pLevel1 BoindingBox: %f %f",pLevel1->boundingBox().getMidX(),pLevel1->boundingBox().getMidY());
			GoLevel1();
		}
		CCMoveTo* pMove = CCMoveTo::create(0.5,ccp(-_SCREEN_WIDTH_*(CurrentChooseLevel-1),0));
		runAction(pMove);
		//这里如果不使用return则到第5关反复停止动画会出问题，原因不明。
		return;
	}

	//用于调节手感的参数
	float fFeel = 0.2;

	//第一关禁止继续向右划
	if (getPositionX()>0)
	{
		//这样写是否会不安全？
		CCMoveTo* pMove = CCMoveTo::create(0.5,ccp(0,0));
		runAction(pMove);
		m_fDistance = 0;
		return;
	}
	//向右滑动检测
	//大于0说明为向右滑动，如果距离大于半屏则显示前一关
	if (m_fDistance>0 && fabs(m_fDistance)>_SCREEN_WIDTH_*fFeel)
	{
		CurrentChooseLevel--;
		CCMoveTo* pMove = CCMoveTo::create(0.5,ccp(-_SCREEN_WIDTH_*(CurrentChooseLevel-1),0));
		runAction(pMove);
		m_fDistance = 0;
	}
	//向右滑动，如果滑动距离小于半屏，则返回显示当前关
	if (m_fDistance>0 && fabs(m_fDistance)<_SCREEN_WIDTH_*fFeel)
	{
		CCMoveTo* pMove = CCMoveTo::create(0.5,ccp(-_SCREEN_WIDTH_*(CurrentChooseLevel-1),0));
		runAction(pMove);
		m_fDistance = 0;
	}
	//如果当前显示为第5关，则无法继续向左滑动。
	//注意，因为这个判断，所以向左滑动检测必须写在向右滑动检测的下面，也就是CurrentChooseLevel++必须在本判断的之后出现。
	if (CurrentChooseLevel==5)
	{
		CCMoveTo* pMove = CCMoveTo::create(0.5,ccp(-_SCREEN_WIDTH_*4,0));
		runAction(pMove);
		m_fDistance = 0;
		return;
	}
	//向左滑动检测
	//小于0说明为向左滑动，如果距离大于半屏则显示下一关
	if (m_fDistance<0 && fabs(m_fDistance)>_SCREEN_WIDTH_*fFeel)
	{
		CurrentChooseLevel++;
		CCMoveTo* pMove = CCMoveTo::create(0.5,ccp(-_SCREEN_WIDTH_*(CurrentChooseLevel-1),0));
		runAction(pMove);
		m_fDistance = 0;
	}
	//向左滑动，如果滑动距离小于半屏，则返回显示当前关
	if (m_fDistance<0 && fabs(m_fDistance)<_SCREEN_WIDTH_*fFeel)
	{
		CCMoveTo* pMove = CCMoveTo::create(0.5,ccp(-_SCREEN_WIDTH_*(CurrentChooseLevel-1),0));
		runAction(pMove);
		m_fDistance = 0;
	}
}

void ChooseLevel::Update(float fT)
{
	do 
	{
		BackGround* pBackGround = dynamic_cast<BackGround*>(this->getParent()->getChildByTag(enTagBackGround));
		CC_BREAK_IF(pBackGround==NULL);
		pBackGround->BackGroundLoop();
		return ;
	} while (false);
	CCLog("Fun ChooseLevel::Update Error!");
}

void ChooseLevel::keyBackClicked()
{

}

void ChooseLevel::keyMenuClicked()
{

}

void ChooseLevel::GoLevel1()
{
	CCScene* pScene = GameScene::CreateScene();
	CCAssert(pScene!=NULL,"pScene==NULL");
	CCDirector::sharedDirector()->replaceScene(pScene);
}
