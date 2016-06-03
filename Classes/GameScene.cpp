#include "GameScene.h"
#include "Player.h"
#include "BackGround.h"
#include "EnemyManager.h"
#include "Bullet.h"
#include "Enemy.h"
#include "ChooseLevel.h"

//有几种敌人
#define _ENEMY_TYPE_COUNT_ 1

//保存每种敌人信息的结构体
struct EnemyManagerPkg
{
	CCTexture2D* pTexture;	//纹理
	float fSpeed;			//移动速度
	int nCD;				//刷新速度
	int nScore;				//分数
	int nHp;				//血量
};

//建立场景和层
CCScene* GameScene::CreateScene()
{
	do 
	{
		//建立场景
		CCScene* pScene = CCScene::create();
		CC_BREAK_IF(pScene==NULL);

		//建立层
		GameScene* pLayer = GameScene::create();
		CC_BREAK_IF(pLayer==NULL);

		//将层加入场景
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

		//添加重新开始按钮
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
		//调用重写函数的父函数
		CC_BREAK_IF(!CCLayer::init());

		//设置触摸开启，并设为单点触摸
		this->setTouchEnabled(true);
		this->setTouchMode(kCCTouchesOneByOne);

		//开启back&menu
		this->setKeypadEnabled(true);

		//初始化子弹集合
		BulletArray = CCArray::create();
		CC_BREAK_IF(BulletArray==NULL);
		BulletArray->retain();
		//初始化被删除的子弹集合
		BulletArrayDeleted = CCArray::create();
		CC_BREAK_IF(BulletArrayDeleted==NULL);
		BulletArrayDeleted->retain();
		//初始化敌人集合
		EnemyArray = CCArray::create();
		CC_BREAK_IF(EnemyArray==NULL);
		EnemyArray->retain();
		//初始化被删除的敌人集合
		EnemyArrayDeleted = CCArray::create();
		CC_BREAK_IF(EnemyArrayDeleted==NULL);
		EnemyArrayDeleted->retain();
		//初始化敌人管理器集合
		EnemyManagerArray = CCArray::create();
		CC_BREAK_IF(EnemyManagerArray==NULL);
		EnemyManagerArray->retain();

		//建立玩家对象
		Player* pPlayer = Player::CreatePlayer("Player.png",this);
		CC_BREAK_IF(pPlayer==NULL);
		pPlayer->setPosition(ccp(_SCREEN_WIDTH_*0.5f,_SCREEN_HEIGHT_*0.2f));
		this->addChild(pPlayer,enZOrderFront,enTagPlayer);

		//建立背景对象
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

		//敌人信息集合
		const EnemyManagerPkg tempEnemyManagerArray[_ENEMY_TYPE_COUNT_] = {
			{CCTextureCache::sharedTextureCache()->addImage("enemy.png"),7,10,1000,2},
//			{CCTextureCache::sharedTextureCache()->addImage("Enemy2.png"),5,250,6000,4},
//			{CCTextureCache::sharedTextureCache()->addImage("Enemy3.png"),4,1500,30000,15}
        };

		//遍历敌人信息集合，建立每种敌人的管理器
		for (int i=0;i<_ENEMY_TYPE_COUNT_;i++)
		{
			EnemyManager* pEnemyManager = EnemyManager::CreateEnemyManager(
				tempEnemyManagerArray[i].pTexture,tempEnemyManagerArray[i].fSpeed,
				tempEnemyManagerArray[i].nCD,tempEnemyManagerArray[i].nScore,
				tempEnemyManagerArray[i].nHp,this);
			//什么情况下使用CCassert？
			CCAssert(pEnemyManager!=NULL,"pEnemyManage==NULL");
			//防止引擎释放敌人管理器，需要在层的onExit函数释放。
			pEnemyManager->retain();
			//将建立的敌人管理器存入敌人管理器集合
			this->EnemyManagerArray->addObject(pEnemyManager);
		}

		//注册Update函数
		this->schedule(schedule_selector(GameScene::OnPreFrameUpdate));

		//添加暂停按钮
		CCMenu* pPause = CCMenu::create();
		CC_BREAK_IF(pPause==NULL);
		pPause->setPosition(ccp(WinSize.width*0.1,WinSize.height*0.9));
		addChild(pPause);
		//按钮纹理
		CCMenuItemImage* pPauseImage = CCMenuItemImage::create(
			"stop.png",
			"stop.png",
			this,
			menu_selector(GameScene::OnPause)
			);
		pPause->addChild(pPauseImage,enZOrderFront,enTagPause);

		//添加分数显示器
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
	//敌人管理器集合内的所有成员都被手动防止释放，所以需要遍历分别释放
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
		//防止玩家飞机飞出屏幕
		CCSize PlayerSize = pPlayer->getTexture()->getContentSize();
		if ((pPlayer->getPosition() + pTouch->getLocation() - pTouch->getPreviousLocation()).x > PlayerSize.width*0.5 &&
			(pPlayer->getPosition() + pTouch->getLocation() - pTouch->getPreviousLocation()).x < _SCREEN_WIDTH_ - PlayerSize.width*0.5 &&
			(pPlayer->getPosition() + pTouch->getLocation() - pTouch->getPreviousLocation()).y > PlayerSize.height*0.8 &&
			(pPlayer->getPosition() + pTouch->getLocation() - pTouch->getPreviousLocation()).y < _SCREEN_HEIGHT_ - PlayerSize.height*0.8
			)
		{
			//玩家移动距离为：当前检测到的位置与上一次检测到的位置之差加上玩家位置。
			pPlayer->setPosition(pPlayer->getPosition() + pTouch->getLocation() 
				- pTouch->getPreviousLocation());
			//移动后更新敌人碰撞方块的位置。
			pPlayer->RectForCollision.setRect(pPlayer->getPositionX()-pPlayer->boundingBox().size.width*0.25,pPlayer->getPositionY()-pPlayer->boundingBox().size.height*0.25,pPlayer->boundingBox().size.width*0.5,pPlayer->boundingBox().size.height*0.5);
		}


		return;
	} while (false);
	CCLog("Fun GameScene::ccTouchMoved Error!");
}

void GameScene::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{

}

//重写的接收函数，每当被调用则根据收到的不同消息执行不同命令
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
			//玩家每帧循环
			Player* pPlayer = dynamic_cast<Player*>(this->getChildByTag(enTagPlayer));
			CC_BREAK_IF(pPlayer==NULL);
			pPlayer->PlayerLoop();

			//遍历子弹集合，并执行循环
			CCObject* pObj = NULL;
			CCARRAY_FOREACH(BulletArray,pObj)
			{
				Bullet* pBullet = dynamic_cast<Bullet*>(pObj);
				CCAssert(pBullet!=NULL,"pBullet==NULL");
				pBullet->BulletLoop();
			}

			//删除上一帧被标记（存入删除集合的子弹对象）需要删除的子弹
			BulletArray->removeObjectsInArray(BulletArrayDeleted);
			BulletArrayDeleted->removeAllObjects();

			//背景循环
			BackGround* pBackGround = dynamic_cast<BackGround*>(this->getChildByTag(enTagBgMove));
			CC_BREAK_IF(pBackGround==NULL);
			pBackGround->BackGroundLoop();

			//遍历敌人管理器集合，刷出满足刷新条件的敌人
			CCARRAY_FOREACH(EnemyManagerArray,pObj)
			{
				EnemyManager* pEnemyManager = dynamic_cast<EnemyManager*>(pObj);
				CCAssert(pEnemyManager!=NULL,"pEnemyManager==NULL");
				pEnemyManager->EnemyManagerLoop();
			}

			//遍历敌人集合，并循环
			CCARRAY_FOREACH(EnemyArray,pObj)
			{
				Enemy* pEnemy = dynamic_cast<Enemy*>(pObj);
				CCAssert(pEnemy!=NULL,"pEnemy!=NULL");
				pEnemy->EnemyLoop();
			}

			//删除需要被删除的敌人
			EnemyArray->removeObjectsInArray(EnemyArrayDeleted);
			EnemyArrayDeleted->removeAllObjects();

			//碰撞
			this->Collision();

			//修改分数
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
		//遍历敌人数组，检测当前敌人是否与某子弹或玩家碰撞
		CCObject* pObj = NULL;
		CCARRAY_FOREACH(EnemyArray,pObj)
		{
			Enemy* pEnemy = dynamic_cast<Enemy*>(pObj);
			CCAssert(pEnemy!=NULL,"pEnemy==NULL");
			//遍历子弹数组
			CCObject* pBulletObj = NULL;
			CCARRAY_FOREACH(BulletArray,pBulletObj)
			{
				Bullet* pBullet = dynamic_cast<Bullet*>(pBulletObj);
				CCAssert(pBullet!=NULL,"pBullet==NULL");
				//如果当前子弹在当前敌人的纹理范围内且敌人未死亡则发生碰撞
				if (pBullet->boundingBox().intersectsRect(pEnemy->boundingBox())&&pEnemy->IsDead()==false)
				{
					pEnemy->Hurt();
					this->removeChild(pBullet);
					BulletArrayDeleted->addObject(pBullet);
				}
			}
			//如果玩家在当前敌人的纹理范围内且敌人未死亡，则游戏结束
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
		//创建动画
		//将图片资源添加到帧缓存
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("PFBoom.plist","PFBoom.png");
		//创建一个动画
		CCAnimation* pBoomAnimaton = CCAnimation::create();
		CC_BREAK_IF(pBoomAnimaton==NULL);
		pBoomAnimaton->setDelayPerUnit(0.1f);
		pBoomAnimaton->setLoops(1);
		pBoomAnimaton->setRestoreOriginalFrame(false);
		//向动画添加帧
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
