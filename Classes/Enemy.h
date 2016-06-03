#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "PublicDefine.h"

class Enemy : public CCSprite,public Sender
{
protected:
	float m_fSpeed;
public:
	int m_nScore;
	int m_nHp;
	bool m_bIsAnimationOn;
	CCRect RectForCollision;
	static Enemy* CreateEnemy(CCTexture2D* pTexture,Receiver* pReceiver,float fSpeed,int nScore,int nHp);
	void EnemyLoop();
	bool IsDead();
	void Hurt();
protected:
	bool Init(CCTexture2D* pTexture,Receiver* pReceiver,float fSpeed,int nScore,int nHp);

	bool IsMove();
	void Move();

	bool IsDisappear();
	void Disappear();


	void Dead();
};

#endif