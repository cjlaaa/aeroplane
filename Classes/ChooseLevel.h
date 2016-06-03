#ifndef CHOOSE_LEVEL
#define CHOOSE_LEVEL

#include "PublicDefine.h"

class ChooseLevel : public CCLayer
{
public:
	static CCScene* CreateScene();
protected:
	CREATE_FUNC(ChooseLevel);
	bool init();
	int CurrentChooseLevel;
	bool isMoveDirectionRight;
	float m_fDistance;
	CCMoveTo* pMove;

	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	void Update(float fT);

	void keyBackClicked();
	void keyMenuClicked();
	enum 
	{
		enTagPlayer,
		enTagBackGround,
		enTagLevel1,
		enTagLevel2,
		enTagLevel3,
		enTagLevel4,
		enTagLevel5,
	};
	void GoLevel1();
};

#endif