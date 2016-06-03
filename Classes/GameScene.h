#ifndef _GAMESCENE_H_
#define _GAMESCENE_H_

#include "PublicDefine.h"

class GameScene : public CCLayer,public Receiver
{
protected:
	CCArray* BulletArray;
	CCArray* BulletArrayDeleted;
	CCArray* EnemyArray;
	CCArray* EnemyArrayDeleted;
	CCArray* EnemyManagerArray;

	int Score;
	char tempChar[10];
	bool isRun;
public:
	static CCScene* CreateScene();
protected:
	CREATE_FUNC(GameScene);
	bool init();
	void onExit();

	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	//back&menu
	void keyBackClicked();
	void keyMenuClicked();

	void Restart(CCObject* pSender);

	void OnMsgReceive(int enMsg,void* pData,int nSize);

	void OnPreFrameUpdate(float fT);

	void OnPause(CCObject* pSender);
protected:
	enum 
	{
		enTagPlayer,
		enTagBackGround,
		enTagPause,
		enTagScore,
		enTagRestart
	};
protected:
	void BulletCreate(BulletForCreateMsg* pData);
	void BulletDisappear(BulletForDisappearMsg* pData);
	void EnemyCreate(EnemyForCreateMsg* pData);
	void EnemyDisappear(EnemyForDisappearMsg* pData);
	void AddScore(AddScoreMsg* pData);
	void Collision();
	const char* GetStringFromInt(int Num);
	
	void InitAnimation();
};


#endif