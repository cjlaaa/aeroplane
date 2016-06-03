#ifndef _ENEMY_MANAGER_H_
#define _ENEMY_MANAGER_H_

#include "PublicDefine.h"

class EnemyManager : public Sender,public CCObject
{
protected:
	CCTexture2D* m_pTexture;
	float m_fSpeed;

	int m_nHp;
	int m_nScore;
	int m_nCD;
	int m_nCDTime;
public:
	static EnemyManager* CreateEnemyManager(CCTexture2D* pTexture,float fSpeed,int nCD,int nScore,int nHp,Receiver* pReceiver);
	void EnemyManagerLoop();
protected:
	bool Init(CCTexture2D* pTexture,float fSpeed,int nCD,int nScore,int nHp,Receiver* pReceiver);

	bool IsCreate();
	void Create();
};

#endif