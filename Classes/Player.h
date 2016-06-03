#ifndef _PLAYER_H_
#define _PLAYER_H_


#include "PublicDefine.h"

#define _FIRE_CD_ 10

class Player : public CCSprite, public Sender
{
protected:
	int m_nFireCD;
public:
	CCRect RectForCollision;
	static Player* CreatePlayer(const char* pFileName,Receiver* pReceiver);

	void PlayerLoop();
	CCRect BoundingBox();
protected:
	bool Init(const char* pFileName,Receiver* pReceiver);

	bool IsFire();
	void Fire();
};

#endif