#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include "PublicDefine.h"

class BackGround : public CCNode
{
protected:
	float m_fSpeed;
	float m_fDisplacement;
public:
	static BackGround* CreateBackGround(const char* pFileName);
	void BackGroundLoop();
protected:
	bool Init(const char* pFileName);

	bool IsMove();
	void Move();
	bool IsReset();
	void Reset();

protected:
	enum 
	{
		enTagBack1,
		enTagBack2
	};
};

#endif