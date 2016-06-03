#ifndef _BULLET_H_
#define _BULLET_H_

#include "PublicDefine.h"
#define _BULLET_SPEED_ 25

class Bullet : public CCSprite, public Sender
{
protected:
	float m_fSpeed;
public:
	static Bullet* CreateBullet(const char* pFileName,Receiver* pReceiver);
	void BulletLoop();
protected:
	bool Init(const char* pFileName,Receiver* pReceiver);

	bool IsMove();
	void Move();

	bool IsDisappear();
	void Disappear();
};

#endif