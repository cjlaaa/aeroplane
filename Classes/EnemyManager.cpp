#include "EnemyManager.h"

EnemyManager* EnemyManager::CreateEnemyManager( CCTexture2D* pTexture,float fSpeed,int nCD,int nScore,int nHp,Receiver* pReceiver )
{
	do 
	{
		EnemyManager* pEnemyManager = new EnemyManager;
		if (pEnemyManager && pEnemyManager->Init(pTexture,fSpeed,nCD,nScore,nHp,pReceiver))
		{
			pEnemyManager->autorelease();
			return pEnemyManager;
		}
		delete pEnemyManager;
	} while (false);
	CCLog("Fun EnemyManager::CreateEnemyManager Error!");
	return NULL;
}

bool EnemyManager::Init( CCTexture2D* pTexture,float fSpeed,int nCD,int nScore,int nHp,Receiver* pReceiver )
{
	do 
	{
		this->m_nScore = nScore;
		this->m_nHp = nHp;
		m_pReceiver = pReceiver;
		m_nCD = nCD;
		m_nCDTime = m_nCD;
		m_pTexture = pTexture;
		m_fSpeed = fSpeed;
		return true;
	} while (false);
	CCLog("Fun EnemyManager::Init Error!");
	return false;
}

void EnemyManager::EnemyManagerLoop()
{
	if (IsCreate())
	{
		Create();
	}
}

bool EnemyManager::IsCreate()
{
	if (m_nCD==0)
	{
		m_nCD = m_nCDTime;
		return true;
	}
	else
	{
		m_nCD--;
		return false;
	}
}

void EnemyManager::Create()
{
	CCSize PlaneSize = m_pTexture->getContentSize();
	EnemyForCreateMsg Info;
	Info.nScore = m_nScore;
	Info.nHp = m_nHp;
	Info.pTexture = m_pTexture;
	Info.fSpeed = m_fSpeed;
	//为了不让敌机产生一半在屏幕外的情况。
	Info.pStartPoint = CCPointMake(rand()%(int)(_SCREEN_WIDTH_ - PlaneSize.width)
		+ PlaneSize.width*0.5,
		_SCREEN_HEIGHT_-1);
	this->SendMsg(enMsgEnemyForCreate,&Info,sizeof(Info));
}

