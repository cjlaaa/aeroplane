#include "GameScene.h"
#include "Player.h"
#include "BackGround.h"
#include "EnemyManager.h"
#include "Bullet.h"
#include "Enemy.h"
#include "ChooseLevel.h"

//�м��ֵ���
#define _ENEMY_TYPE_COUNT_ 1

//����ÿ�ֵ�����Ϣ�Ľṹ��
struct EnemyManagerPkg
{
	CCTexture2D* pTexture;	//����
	float fSpeed;			//�ƶ��ٶ�
	int nCD;				//ˢ���ٶ�
	int nScore;				//����
	int nHp;				//Ѫ��
};

//���������Ͳ�
CCScene* GameScene::CreateScene()
{
	do 
	{
		//��������
		CCScene* pScene = CCScene::create();
		CC_BREAK_IF(pScene==NULL);

		//������
		GameScene* pLayer = GameScene::create();
		CC_BREAK_IF(pLayer==NULL);

		//������볡��
		pScene->addChild(pLayer);
		return pScene;
	} while (false);
	CCLog("Fun GameScene::CreateScene Error!");
	return NULL;
}

bool GameScene::init()
{
	do 
	{
		CCSize WinSize = CCDirector::sharedDirector()->getWinSize();

		//������¿�ʼ��ť
		CCMenu* pRestart = CCMenu::create();
		CC_BREAK_IF(pRestart==NULL);
		pRestart->setPosition(ccp(WinSize.width*0.9,WinSize.height*0.1));
		addChild(pRestart,enZOrderFront,enTagRestart);
		CCMenuItemImage* pRestartImage = CCMenuItemImage::create(
			"CloseNormal.png",
			"CloseSelected.png",
			this,
			menu_selector(GameScene::Restart));
		pRestartImage->setScale(2);
		CC_BREAK_IF(pRestartImage==NULL);
		pRestart->addChild(pRestartImage);

		InitAnimation();
		isRun = true;
		//������д�����ĸ�����
		CC_BREAK_IF(!CCLayer::init());

		//���ô�������������Ϊ���㴥��
		this->setTouchEnabled(true);
		this->setTouchMode(kCCTouchesOneByOne);

		//����back&menu
		this->setKeypadEnabled(true);

		//��ʼ���ӵ�����
		BulletArray = CCArray::create();
		CC_BREAK_IF(BulletArray==NULL);
		BulletArray->retain();
		//��ʼ����ɾ�����ӵ�����
		BulletArrayDeleted = CCArray::create();
		CC_BREAK_IF(BulletArrayDeleted==NULL);
		BulletArrayDeleted->retain();
		//��ʼ�����˼���
		EnemyArray = CCArray::create();
		CC_BREAK_IF(EnemyArray==NULL);
		EnemyArray->retain();
		//��ʼ����ɾ���ĵ��˼���
		EnemyArrayDeleted = CCArray::create();
		CC_BREAK_IF(EnemyArrayDeleted==NULL);
		EnemyArrayDeleted->retain();
		//��ʼ�����˹���������
		EnemyManagerArray = CCArray::create();
		CC_BREAK_IF(EnemyManagerArray==NULL);
		EnemyManagerArray->retain();

		//������Ҷ���
		Player* pPlayer = Player::CreatePlayer("Player.png",this);
		CC_BREAK_IF(pPlayer==NULL);
		pPlayer->setPosition(ccp(_SCREEN_WIDTH_*0.5f,_SCREEN_HEIGHT_*0.2f));
		this->addChild(pPlayer,enZOrderFront,enTagPlayer);

		//������������
        CCSprite* pBg = CCSprite::create("bg.png");
        CC_BREAK_IF(pBg==NULL);
        pBg->setPosition(ccp(_SCREEN_WIDTH_*0.5f,_SCREEN_HEIGHT_*0.5f));
        pBg->setScaleX(_SCREEN_WIDTH_/pBg->getContentSize().width);
        pBg->setScaleY(_SCREEN_HEIGHT_/pBg->getContentSize().height);
        this->addChild(pBg,enZOrderBack,enTagBg);
		BackGround* pBgMove = BackGround::CreateBackGround("cloud.png");
		CC_BREAK_IF(pBgMove==NULL);
//		pBgMove->setPosition(ccp(_SCREEN_WIDTH_*0.5f,_SCREEN_HEIGHT_*1.f));
		this->addChild(pBgMove,enZOrderBack,enTagBgMove);

		//������Ϣ����
		const EnemyManagerPkg tempEnemyManagerArray[_ENEMY_TYPE_COUNT_] = {
			{CCTextureCache::sharedTextureCache()->addImage("enemy.png"),7,10,1000,2},
//			{CCTextureCache::sharedTextureCache()->addImage("Enemy2.png"),5,250,6000,4},
//			{CCTextureCache::sharedTextureCache()->addImage("Enemy3.png"),4,1500,30000,15}
        };

		//����������Ϣ���ϣ�����ÿ�ֵ��˵Ĺ�����
		for (int i=0;i<_ENEMY_TYPE_COUNT_;i++)
		{
			EnemyManager* pEnemyManager = EnemyManager::CreateEnemyManager(
				tempEnemyManagerArray[i].pTexture,tempEnemyManagerArray[i].fSpeed,
				tempEnemyManagerArray[i].nCD,tempEnemyManagerArray[i].nScore,
				tempEnemyManagerArray[i].nHp,this);
			//ʲô�����ʹ��CCassert��
			CCAssert(pEnemyManager!=NULL,"pEnemyManage==NULL");
			//��ֹ�����ͷŵ��˹���������Ҫ�ڲ��onExit�����ͷš�
			pEnemyManager->retain();
			//�������ĵ��˹�����������˹���������
			this->EnemyManagerArray->addObject(pEnemyManager);
		}

		//ע��Update����
		this->schedule(schedule_selector(GameScene::OnPreFrameUpdate));

		//�����ͣ��ť
		CCMenu* pPause = CCMenu::create();
		CC_BREAK_IF(pPause==NULL);
		pPause->setPosition(ccp(WinSize.width*0.1,WinSize.height*0.9));
		addChild(pPause);
		//��ť����
		CCMenuItemImage* pPauseImage = CCMenuItemImage::create(
			"stop.png",
			"stop.png",
			this,
			menu_selector(GameScene::OnPause)
			);
		pPause->addChild(pPauseImage,enZOrderFront,enTagPause);

		//��ӷ�����ʾ��
		CCLabelTTF* pScore = CCLabelTTF::create(" ","Comic Sans MS",45);
		pScore->setColor(ccc3(0,0,0));
		pScore->setAnchorPoint(ccp(0,0.5));
		pScore->setPosition(ccp(WinSize.width*0.17,WinSize.height*0.9));
		addChild(pScore,enZOrderFront,enTagScore);
		Score = 0;

		return true;
	} while (false);
	CCLog("Fun GameScene::init Error!");
	return false;
}

void GameScene::onExit()
{

	CCLayer::onExit();
	BulletArray->release();
	BulletArrayDeleted->release();
	//���˹����������ڵ����г�Ա�����ֶ���ֹ�ͷţ�������Ҫ�����ֱ��ͷ�
	CCObject* pObj = NULL;
	CCARRAY_FOREACH(EnemyManagerArray,pObj)
	{
		pObj->release();
	}
	EnemyManagerArray->release();
	EnemyArray->release();
	EnemyArrayDeleted->release();
}

bool GameScene::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	return true;
}

void GameScene::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	do 
	{
		Player* pPlayer = dynamic_cast<Player*>(this->getChildByTag(enTagPlayer));
		CC_BREAK_IF(pPlayer==NULL);
		//��ֹ��ҷɻ��ɳ���Ļ
		CCSize PlayerSize = pPlayer->getTexture()->getContentSize();
		if ((pPlayer->getPosition() + pTouch->getLocation() - pTouch->getPreviousLocation()).x > PlayerSize.width*0.5 &&
			(pPlayer->getPosition() + pTouch->getLocation() - pTouch->getPreviousLocation()).x < _SCREEN_WIDTH_ - PlayerSize.width*0.5 &&
			(pPlayer->getPosition() + pTouch->getLocation() - pTouch->getPreviousLocation()).y > PlayerSize.height*0.8 &&
			(pPlayer->getPosition() + pTouch->getLocation() - pTouch->getPreviousLocation()).y < _SCREEN_HEIGHT_ - PlayerSize.height*0.8
			)
		{
			//����ƶ�����Ϊ����ǰ��⵽��λ������һ�μ�⵽��λ��֮��������λ�á�
			pPlayer->setPosition(pPlayer->getPosition() + pTouch->getLocation() 
				- pTouch->getPreviousLocation());
			//�ƶ�����µ�����ײ�����λ�á�
			pPlayer->RectForCollision.setRect(pPlayer->getPositionX()-pPlayer->boundingBox().size.width*0.25,pPlayer->getPositionY()-pPlayer->boundingBox().size.height*0.25,pPlayer->boundingBox().size.width*0.5,pPlayer->boundingBox().size.height*0.5);
		}


		return;
	} while (false);
	CCLog("Fun GameScene::ccTouchMoved Error!");
}

void GameScene::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{

}

//��д�Ľ��պ�����ÿ��������������յ��Ĳ�ͬ��Ϣִ�в�ͬ����
void GameScene::OnMsgReceive( int enMsg,void* pData,int nSize )
{
	switch (enMsg)
	{
	case enMsgBulletForCreate:
		{
			if (sizeof(BulletForCreateMsg)!=nSize)
			{
				CCAssert(false,"sizeof(BulletForCreateMsg)!=nSize");
			}
			this->BulletCreate((BulletForCreateMsg*)pData);
		}
		break;
	case enMsgBulletForDisappear:
		{
			if (sizeof(BulletForDisappearMsg)!=nSize)
			{
				CCAssert(false,"sizeof(BulletForDisappearMsg)!=nSize");
			}
			this->BulletDisappear((BulletForDisappearMsg*)pData);
		}
		break;
	case enMsgEnemyForCreate:
		{
			if (sizeof(EnemyForCreateMsg)!=nSize)
			{
				CCAssert(false,"sizeof(EnemyForCreateMsg)!=nSize");
			}
			this->EnemyCreate((EnemyForCreateMsg*)pData);
		}
		break;
	case enMsgEnemyForDisappear:
		{
			if (sizeof(EnemyForDisappearMsg)!=nSize)
			{
				CCAssert(false,"sizeof(EnemyForDisappearMsg)!=nSize");
			}
			this->EnemyDisappear((EnemyForDisappearMsg*)pData);
		}
		break;
	case enMsgAddScore:
		{
			if (sizeof(AddScoreMsg)!=nSize)
			{
				CCAssert(false,"sizeof(EnemyForDisappearMsg)!=nSize");
			}
			this->AddScore((AddScoreMsg*)pData);
		}
		break;
	}
}

void GameScene::OnPreFrameUpdate( float fT )
{
	do 
	{
		if (isRun)
		{
			//���ÿ֡ѭ��
			Player* pPlayer = dynamic_cast<Player*>(this->getChildByTag(enTagPlayer));
			CC_BREAK_IF(pPlayer==NULL);
			pPlayer->PlayerLoop();

			//�����ӵ����ϣ���ִ��ѭ��
			CCObject* pObj = NULL;
			CCARRAY_FOREACH(BulletArray,pObj)
			{
				Bullet* pBullet = dynamic_cast<Bullet*>(pObj);
				CCAssert(pBullet!=NULL,"pBullet==NULL");
				pBullet->BulletLoop();
			}

			//ɾ����һ֡����ǣ�����ɾ�����ϵ��ӵ�������Ҫɾ�����ӵ�
			BulletArray->removeObjectsInArray(BulletArrayDeleted);
			BulletArrayDeleted->removeAllObjects();

			//����ѭ��
			BackGround* pBackGround = dynamic_cast<BackGround*>(this->getChildByTag(enTagBgMove));
			CC_BREAK_IF(pBackGround==NULL);
			pBackGround->BackGroundLoop();

			//�������˹��������ϣ�ˢ������ˢ�������ĵ���
			CCARRAY_FOREACH(EnemyManagerArray,pObj)
			{
				EnemyManager* pEnemyManager = dynamic_cast<EnemyManager*>(pObj);
				CCAssert(pEnemyManager!=NULL,"pEnemyManager==NULL");
				pEnemyManager->EnemyManagerLoop();
			}

			//�������˼��ϣ���ѭ��
			CCARRAY_FOREACH(EnemyArray,pObj)
			{
				Enemy* pEnemy = dynamic_cast<Enemy*>(pObj);
				CCAssert(pEnemy!=NULL,"pEnemy!=NULL");
				pEnemy->EnemyLoop();
			}

			//ɾ����Ҫ��ɾ���ĵ���
			EnemyArray->removeObjectsInArray(EnemyArrayDeleted);
			EnemyArrayDeleted->removeAllObjects();

			//��ײ
			this->Collision();

			//�޸ķ���
			CCLabelTTF* pScore = dynamic_cast<CCLabelTTF*>(getChildByTag(enTagScore));
			pScore->setString(GetStringFromInt(Score));
		}

		return;
	} while (false);
	CCLog("GameScene::OnPreFrameUpdate");
}

void GameScene::BulletCreate( BulletForCreateMsg* pData )
{
	do 
	{
		Bullet* pBullet = Bullet::CreateBullet("Bullet.png",this);
		CC_BREAK_IF(pBullet==NULL);

		pBullet->setPosition(pData->pBulletStartPoint);
		this->addChild(pBullet,enZOrderMiddle);

		BulletArray->addObject(pBullet);
		return ;
	} while (false);
	CCLog("Fun GameScene::BulletCreate Error!");
}

void GameScene::BulletDisappear( BulletForDisappearMsg* pData )
{
	do 
	{
		this->removeChild(pData->pSelf);
		BulletArrayDeleted->addObject(pData->pSelf);
		return ;
	} while (false);
	CCLog("Fun GameScene::BulletDisappear Error!");
}

void GameScene::EnemyCreate( EnemyForCreateMsg* pData )
{
	do 
	{
		Enemy* pEnemy = Enemy::CreateEnemy(pData->pTexture,this,pData->fSpeed,pData->nScore,pData->nHp);
		CC_BREAK_IF(pEnemy==NULL);
		pEnemy->setPosition(pData->pStartPoint);
		this->addChild(pEnemy,enZOrderMiddle);
		EnemyArray->addObject(pEnemy);
		return;
	} while (false);
	CCLog("Fun GameScene::EnemyCreate Error!");
}

void GameScene::EnemyDisappear( EnemyForDisappearMsg* pData )
{
	do 
	{
		this->removeChild(pData->pSelf);
		EnemyArrayDeleted->addObject(pData->pSelf);
		return;
	} while (false);
	CCLog("Fun GameScene::EnemyDisappear Error!");
}

void GameScene::Collision()
{
	do 
	{
		//�����������飬��⵱ǰ�����Ƿ���ĳ�ӵ��������ײ
		CCObject* pObj = NULL;
		CCARRAY_FOREACH(EnemyArray,pObj)
		{
			Enemy* pEnemy = dynamic_cast<Enemy*>(pObj);
			CCAssert(pEnemy!=NULL,"pEnemy==NULL");
			//�����ӵ�����
			CCObject* pBulletObj = NULL;
			CCARRAY_FOREACH(BulletArray,pBulletObj)
			{
				Bullet* pBullet = dynamic_cast<Bullet*>(pBulletObj);
				CCAssert(pBullet!=NULL,"pBullet==NULL");
				//�����ǰ�ӵ��ڵ�ǰ���˵�����Χ���ҵ���δ����������ײ
				if (pBullet->boundingBox().intersectsRect(pEnemy->boundingBox())&&pEnemy->IsDead()==false)
				{
					pEnemy->Hurt();
					this->removeChild(pBullet);
					BulletArrayDeleted->addObject(pBullet);
				}
			}
			//�������ڵ�ǰ���˵�����Χ���ҵ���δ����������Ϸ����
			Player* pPlayer = dynamic_cast<Player*>(this->getChildByTag(enTagPlayer));
			CCAssert(pPlayer!=NULL,"pPlayer==NULL");
			if (pPlayer->BoundingBox().intersectsRect(pEnemy->RectForCollision)&&pEnemy->IsDead()==false)
			{
				//this->removeChild(pPlayer);
				this->unschedule(schedule_selector(GameScene::OnPreFrameUpdate));
				CCLabelTTF* pGameOver = CCLabelTTF::create("Game Over","Arial",56);
				CCAssert(pGameOver!=NULL,"pGameOver==NULL");
				pGameOver->setPosition(ccp(_SCREEN_WIDTH_*0.5f,_SCREEN_HEIGHT_*0.5f));
				this->addChild(pGameOver);
				this->setTouchEnabled(false);
				break;
			}
		}
		return;
	} while (false);
	CCLog("Fun GameScene::Collision Error!");
}

void GameScene::OnPause(CCObject* pSender)
{
	if (isRun)
	{
		isRun = false;
		setTouchEnabled(false);
	}
	else
	{
		isRun = true;
		setTouchEnabled(true);
	}
}

const char* GameScene::GetStringFromInt( int Num )
{
	sprintf(tempChar,"%d",Num);
	CCString* CCStr = CCString::createWithFormat(tempChar);
	const char* cStr = CCStr->getCString();
	return cStr;
}

void GameScene::keyBackClicked()
{
	CCDirector::sharedDirector()->end();
}

void GameScene::keyMenuClicked()
{
	CCScene* pScene = GameScene::CreateScene();
	CCDirector::sharedDirector()->replaceScene(pScene);
}

void GameScene::InitAnimation()
{
	do 
	{
		//��������
		//��ͼƬ��Դ��ӵ�֡����
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("PFBoom.plist","PFBoom.png");
		//����һ������
		CCAnimation* pBoomAnimaton = CCAnimation::create();
		CC_BREAK_IF(pBoomAnimaton==NULL);
		pBoomAnimaton->setDelayPerUnit(0.1f);
		pBoomAnimaton->setLoops(1);
		pBoomAnimaton->setRestoreOriginalFrame(false);
		//�򶯻����֡
		int nIndex = 1;
		char FrameName[20] = {0};
		while (true)
		{
			sprintf(FrameName,"Boom_%d.png",nIndex++);
			CCSpriteFrame* pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(FrameName);
			CC_BREAK_IF(pFrame==NULL);
			pBoomAnimaton->addSpriteFrame(pFrame);
		}
		CCAnimationCache::sharedAnimationCache()->addAnimation(pBoomAnimaton,"Boom");

		return;
	} while (false);
	CCLog("Fun GameScene::InitAnimation Error!");
}

void GameScene::AddScore( AddScoreMsg* pData )
{
	Score+=pData->nScore;
}

void GameScene::Restart(CCObject* pSender)
{
    CCScene *pScene = GameScene::CreateScene();
//	CCScene* pScene = ChooseLevel::CreateScene();
	CCDirector::sharedDirector()->replaceScene(pScene);
}
