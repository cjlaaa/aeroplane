#include "Enemy.h"


Enemy* Enemy::CreateEnemy( CCTexture2D* pTexture,Receiver* pReceiver,float fSpeed,int nScore,int nHp )
 {
	do 
	{
		Enemy* pEnemy = new Enemy;
		if (pEnemy && pEnemy->Init(pTexture,pReceiver,fSpeed,nScore,nHp))
		{
			pEnemy->autorelease();
			return pEnemy;
		}
		delete pEnemy;
	} while (false);
	CCLog("Fun Enemy::CreateEnemy Error!");
	return NULL;
}

bool Enemy::Init( CCTexture2D* pTexture,Receiver* pReceiver,float fSpeed,int nScore,int nHp )
{
	do 
	{
		CC_BREAK_IF(!CCSprite::initWithTexture(pTexture));

		m_pReceiver = pReceiver;
		m_fSpeed = fSpeed;
		this->m_nScore = nScore;
		this->m_nHp = nHp;
		m_bIsAnimationOn = true;
		return true;
	} while (false);
	CCLog("Fun Enemy::Init Error!");
	return false;
}

void Enemy::EnemyLoop()
{
	if (IsDead())
	{
		Dead();
		return;
	}
	if (IsDisappear())
	{
		Disappear();
		return;
	}
	if (IsMove())
	{
		Move();
		//�ƶ�����µ�����ײ�����λ�á�
		RectForCollision.setRect(this->getPositionX(),this->getPositionY(),this->boundingBox().size.width*0.5,this->boundingBox().size.height*0.5);
	}
}

bool Enemy::IsMove()
{
	return true;
}

void Enemy::Move()
{
	this->setPositionY(this->getPositionY() - m_fSpeed);
}

bool Enemy::IsDisappear()
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

void Enemy::Disappear()
{
	EnemyForDisappearMsg Info;
	Info.pSelf = this;
	this->SendMsg(enMsgEnemyForDisappear,&Info,sizeof(Info));
}

bool Enemy::IsDead()
{
	if (m_nHp<1)
	{
		return true;
	}
	return false;
}

void Enemy::Dead()
{
	do 
	{
		//Ϊ��ֹ����ѭ��ʱ�������ţ�����һ�����أ������ſ�ʼʱ���ٲ��š�
		if (m_bIsAnimationOn)
		{
			//�ӷ�
			AddScoreMsg Info;
			Info.nScore = m_nScore;
			SendMsg(enMsgAddScore,&Info,sizeof(Info));

			//��Ϊ��������ֻ��Ҫ����һ�Σ����Բ���Ҫ�ٴ�
			m_bIsAnimationOn = false;
			CCAnimation* pBoom = CCAnimationCache::sharedAnimationCache()->animationByName("Boom");
			//CC_BREAK_IF(pBoom==NULL);

			runAction(CCSequence::create(CCAnimate::create(pBoom),CCCallFunc::create(this,callfunc_selector(Enemy::Disappear)),NULL));
		}
		return;
	} while (false);
	CCLog("Fun Enemy::Dead Error!");
}

void Enemy::Hurt()
{
	m_nHp--;
}
