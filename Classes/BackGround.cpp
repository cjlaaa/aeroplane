#include "BackGround.h"

#define _BACKGROUND_SPEED_ 2
#define _MAX_DISPLACEMENT_ 2560

BackGround* BackGround::CreateBackGround( const char* pFileName )
{
	do 
	{
		BackGround* pBackGround = new BackGround;
		if (pBackGround && pBackGround->Init(pFileName))
		{
			pBackGround->autorelease();
			return pBackGround;
		}
		delete pBackGround;
	} while (false);
	CCLog("Fun BackGround::CreateBackGround Error!");
	return NULL;
}

bool BackGround::Init( const char* pFileName )
{
	do 
	{
		CC_BREAK_IF(!CCNode::init());

		m_fDisplacement = 0;
		m_fSpeed = _BACKGROUND_SPEED_;

		CCSprite* pBackGround1 = CCSprite::create(pFileName);
		CC_BREAK_IF(pBackGround1==NULL);
		pBackGround1->setPosition(ccp(_SCREEN_WIDTH_*(rand()%9/10.0),_SCREEN_HEIGHT_*0.3f));
		this->addChild(pBackGround1,enZOrderBack,enTagBack1);

		CCSprite* pBackGround2 = CCSprite::create(pFileName);
		CC_BREAK_IF(pBackGround2==NULL);
		pBackGround2->setPosition(ccp(_SCREEN_WIDTH_*(rand()%9/10.0),_SCREEN_HEIGHT_*0.8f));
		this->addChild(pBackGround2,enZOrderBack,enTagBack2);

		return true;
	} while (false);
	CCLog("Fun BackGround::Init Error!");
	return false;
}

void BackGround::BackGroundLoop()
{
	if (IsReset())
	{
		Reset();
	}
	if (IsMove())
	{
		Move();
	}
}

bool BackGround::IsMove()
{
	return true;
}

void BackGround::Move()
{
	do 
	{
		m_fDisplacement+=m_fSpeed;

		CCSprite* pBackGround1 = dynamic_cast<CCSprite*>(this->getChildByTag(enTagBack1));
		CC_BREAK_IF(pBackGround1==NULL);
		CCSprite* pBackGround2 = dynamic_cast<CCSprite*>(this->getChildByTag(enTagBack2));
		CC_BREAK_IF(pBackGround2==NULL);

		pBackGround1->setPositionY(pBackGround1->getPositionY()-m_fSpeed);
		pBackGround2->setPositionY(pBackGround2->getPositionY()-m_fSpeed);
        CCLog("x %f y %f",pBackGround1->getPositionX(),pBackGround1->getPositionY());
		return;

		return;
	} while (false);
	CCLog("Fun BackGround::Move Error!");
}

bool BackGround::IsReset()
{
	return true;
}

void BackGround::Reset()
{
	do 
	{
		CCSprite* pBackGround1 = dynamic_cast<CCSprite*>(this->getChildByTag(enTagBack1));
		CC_BREAK_IF(pBackGround1==NULL);
		CCSprite* pBackGround2 = dynamic_cast<CCSprite*>(this->getChildByTag(enTagBack2));
		CC_BREAK_IF(pBackGround2==NULL);
		
		if (pBackGround1->getPositionY() < 0)
		{
			pBackGround1->setPosition(ccp(_SCREEN_WIDTH_*(rand()%9/10.0),_SCREEN_HEIGHT_));
		}
		
		if (pBackGround2->getPositionY() < 0)
		{
			pBackGround2->setPosition(ccp(_SCREEN_WIDTH_*(rand()%9/10.0),_SCREEN_HEIGHT_));
		}
		return;
	} while (false);
	CCLog("Fun BackGround::Reset Error!");
}

