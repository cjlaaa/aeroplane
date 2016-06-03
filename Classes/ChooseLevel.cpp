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

//������������Ҫ�ǻ���������������⡣ÿ���ж��ﶼҪ���㣡
void ChooseLevel::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	//test ֱ����ת����4��
	/*CCMoveTo* pMove = CCMoveTo::create(1,ccp(-_SCREEN_WIDTH_*3,0));
	runAction(pMove);*/

	//����ƶ�����С��ĳֵ��Ϊ��������δ����ؿ����飬�򷵻ص�ǰӦ��ʾ�ĵط�
	if (fabs(m_fDistance)<10)
	{
		CCSprite* pLevel1 = dynamic_cast<CCSprite*>(getChildByTag(enTagLevel1));
		//�������й����У�ʵ���Ͼ����Ѿ���λ�����Ի���ֵ�հ׵ط�����ؿ�������
		//��Ҫ��ptouch����ϵ����������ת���ɽڵ㣨�㣩���꣬�Ͳ�����Ҫ��ǰ�ؿ��ж���
		if (pLevel1->boundingBox().containsPoint(this->convertToNodeSpace(pTouch->getLocation()))/* && CurrentChooseLevel==1*/)
		{
			CCLog("pLevel1 BoindingBox: %f %f",pLevel1->boundingBox().getMidX(),pLevel1->boundingBox().getMidY());
			GoLevel1();
		}
		CCMoveTo* pMove = CCMoveTo::create(0.5,ccp(-_SCREEN_WIDTH_*(CurrentChooseLevel-1),0));
		runAction(pMove);
		//���������ʹ��return�򵽵�5�ط���ֹͣ����������⣬ԭ������
		return;
	}

	//���ڵ����ָеĲ���
	float fFeel = 0.2;

	//��һ�ؽ�ֹ�������һ�
	if (getPositionX()>0)
	{
		//����д�Ƿ�᲻��ȫ��
		CCMoveTo* pMove = CCMoveTo::create(0.5,ccp(0,0));
		runAction(pMove);
		m_fDistance = 0;
		return;
	}
	//���һ������
	//����0˵��Ϊ���һ��������������ڰ�������ʾǰһ��
	if (m_fDistance>0 && fabs(m_fDistance)>_SCREEN_WIDTH_*fFeel)
	{
		CurrentChooseLevel--;
		CCMoveTo* pMove = CCMoveTo::create(0.5,ccp(-_SCREEN_WIDTH_*(CurrentChooseLevel-1),0));
		runAction(pMove);
		m_fDistance = 0;
	}
	//���һ����������������С�ڰ������򷵻���ʾ��ǰ��
	if (m_fDistance>0 && fabs(m_fDistance)<_SCREEN_WIDTH_*fFeel)
	{
		CCMoveTo* pMove = CCMoveTo::create(0.5,ccp(-_SCREEN_WIDTH_*(CurrentChooseLevel-1),0));
		runAction(pMove);
		m_fDistance = 0;
	}
	//�����ǰ��ʾΪ��5�أ����޷��������󻬶���
	//ע�⣬��Ϊ����жϣ��������󻬶�������д�����һ����������棬Ҳ����CurrentChooseLevel++�����ڱ��жϵ�֮����֡�
	if (CurrentChooseLevel==5)
	{
		CCMoveTo* pMove = CCMoveTo::create(0.5,ccp(-_SCREEN_WIDTH_*4,0));
		runAction(pMove);
		m_fDistance = 0;
		return;
	}
	//���󻬶����
	//С��0˵��Ϊ���󻬶������������ڰ�������ʾ��һ��
	if (m_fDistance<0 && fabs(m_fDistance)>_SCREEN_WIDTH_*fFeel)
	{
		CurrentChooseLevel++;
		CCMoveTo* pMove = CCMoveTo::create(0.5,ccp(-_SCREEN_WIDTH_*(CurrentChooseLevel-1),0));
		runAction(pMove);
		m_fDistance = 0;
	}
	//���󻬶��������������С�ڰ������򷵻���ʾ��ǰ��
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
