#include "Bullet.h"


Bullet* Bullet::CreateBullet( const char* pFileName,Receiver* pReceiver )
{
	do 
	{
		Bullet* pBullet = new Bullet;

		if (pBullet && pBullet->Init(pFileName,pReceiver))
		{
			pBullet->autorelease();
			return pBullet;
		}
		delete pBullet;
	} while (false);
	CCLog("Fun Bullet::CreateBullet Error!");
	return NULL;
}

void Bullet::BulletLoop()
{
	if (IsDisappear())
	{
		Disappear();
		return;
	}
	if (IsMove())
	{
		Move();
	}
}

bool Bullet::Init( const char* pFileName,Receiver* pReceiver )
{
	do 
	{
		CC_BREAK_IF(!CCSprite::initWithFile(pFileName));

		m_pReceiver = pReceiver;

		m_fSpeed = _BULLET_SPEED_;

		return true;
	} while (false);
	CCLog("Fun Bullet::Init Error!");
	return false;
}

bool Bullet::IsMove()
{
	return true;
}

void Bullet::Move()
{
	this->setPositionY(this->getPositionY() + m_fSpeed);
}

bool Bullet::IsDisappear()
{
	CCSize WinSize = CCDirector::sharedDirector()->getWinSize();

	CCRect WinRect = CCRectMake(0.f,0.f,WinSize.width,WinSize.height);

	if (WinRect.containsPoint(this->getPosition()))
	{
		return false;
	}
	else 
	{
		return true;
	}
}

void Bullet::Disappear()
{
	BulletForDisappearMsg Info;
	Info.pSelf = this;
	this->SendMsg(enMsgBulletForDisappear,&Info,sizeof(Info));
}

