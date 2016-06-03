#include "Player.h"

Player* Player::CreatePlayer( const char* pFileName,Receiver* pReceiver )
{
	do 
	{
		Player* pPlayer = new Player;
		if (pPlayer && pPlayer->Init(pFileName,pReceiver)==true)
		{
			pPlayer->autorelease();
			return pPlayer;
		}
		delete pPlayer;
	} while (false);
	CCLog("Fun Player::CreatePlayer Error!");
	return NULL;
}

bool Player::Init( const char* pFileName,Receiver* pReceiver )
{
	do 
	{
		CC_BREAK_IF(CCSprite::initWithFile(pFileName)==false);

		m_pReceiver = pReceiver;

		m_nFireCD = _FIRE_CD_;

		return true;
	} while (false);
	CCLog("Fun Player::Init Error!");
	return false;
}

bool Player::IsFire()
{
	if (m_nFireCD==0)
	{
		m_nFireCD = _FIRE_CD_;
		return true;
	}
	else
	{
		m_nFireCD--;
		return false;
	}
}

void Player::Fire()
{
	BulletForCreateMsg PlayerFireInfo;
	PlayerFireInfo.pBulletStartPoint = this->getPosition();
	this->SendMsg(enMsgBulletForCreate,&PlayerFireInfo,sizeof(PlayerFireInfo));
}

void Player::PlayerLoop()
{
	if (IsFire())
	{
		Fire();
	}
}

cocos2d::CCRect Player::BoundingBox()
{
	return RectForCollision;
}
