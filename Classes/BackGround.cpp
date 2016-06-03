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
		pBackGround1->setPosition(ccp(_SCREEN_WIDTH_*0.f,pBackGround1->getContentSize().height*0.5f));
		this->addChild(pBackGround1,enZOrderBack,enTagBack1);

		CCSprite* pBackGround2 = CCSprite::create(pFileName);
		CC_BREAK_IF(pBackGround2==NULL);
		pBackGround2->setPosition(ccp(_SCREEN_WIDTH_*0.f,-pBackGround2->getContentSize().height*0.5f));
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
		return;

		return;
	} while (false);
	CCLog("Fun BackGround::Move Error!");
}

bool BackGround::IsReset()
{
	if (m_fDisplacement>=_MAX_DISPLACEMENT_)
	{
		m_fDisplacement = 0;
		return true;
	}
	return false;
}

void BackGround::Reset()
{
	do 
	{
		CCSprite* pBackGround1 = dynamic_cast<CCSprite*>(this->getChildByTag(enTagBack1));
		CC_BREAK_IF(pBackGround1==NULL);
		CCSprite* pBackGround2 = dynamic_cast<CCSprite*>(this->getChildByTag(enTagBack2));
		CC_BREAK_IF(pBackGround2==NULL);
		//Ã»¿´¶®
		if (pBackGround1->getPositionY() < pBackGround2->getPositionY())
		{
			pBackGround1->setPositionY(pBackGround1->getPositionY()+_MAX_DISPLACEMENT_*2);
		}
		else
		{
			pBackGround2->setPositionY(pBackGround2->getPositionY()+_MAX_DISPLACEMENT_*2);
		}
		return;
	} while (false);
	CCLog("Fun BackGround::Reset Error!");
}

