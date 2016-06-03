#ifndef _PUBLIC_DEFINE_H_
#define _PUBLIC_DEFINE_H_


#include "cocos2d.h"
USING_NS_CC;

#define _SCREEN_WIDTH_ CCDirector::sharedDirector()->getWinSize().width
#define _SCREEN_HEIGHT_ CCDirector::sharedDirector()->getWinSize().height

enum 
{
	enZOrderBack,
	enZOrderMiddle,
	enZOrderFront
};

enum 
{
	enMsgBulletForCreate,			//�����ӵ�����Ϣ
	enMsgEnemyForCreate,			//������˵���Ϣ
	enMsgBulletForDisappear,		//�ӵ���ʧ����Ϣ
	enMsgEnemyForDisappear,			//������ʧ����Ϣ
	enMsgAddScore					//�ӷֵ���Ϣ
};

class Receiver
{
public:
	//=0��ʲô��˼��
	virtual void OnMsgReceive(int enMsg,void* pData,int nSize) = 0;
};

class Sender
{
protected:
	Receiver* m_pReceiver;
public:
	void SetReceiver(Receiver* pReceiver)
	{
		m_pReceiver=pReceiver;
	}
	void SendMsg(int enMsg,void* pData,int nSize)
	{
		m_pReceiver->OnMsgReceive(enMsg,pData,nSize);
	}
};

struct BulletForCreateMsg
{
	CCPoint pBulletStartPoint;
};

class Bullet;
struct BulletForDisappearMsg
{
	Bullet* pSelf;
};

class Enemy;
struct EnemyForDisappearMsg
{
	Enemy* pSelf;
};

struct EnemyForCreateMsg
{
	CCTexture2D* pTexture;
	float fSpeed;
	CCPoint pStartPoint;
	int nScore;
	int nHp;
};

struct AddScoreMsg
{
	int nScore;
};

#endif