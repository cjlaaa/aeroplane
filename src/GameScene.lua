
require "Player"
require "GameManager"
require "Bullet"

local mainLayer = nil;
local LastMove = nil;
local WinSize = cc.Director:getInstance():getWinSize()

local kTagPlayer = 0;
local kTagBg = 1;

-- handling touch events
function onTouchBegan(touch, event)
    BeginPos = touch:getLocation()
    -- print (BeginPos.x.." "..BeginPos.y)
    LastMove = BeginPos;
    return true
end


function onTouchMoved(touch, event)
    MovedPos = touch:getLocation()

    local player = mainLayer:getChildByTag(kTagPlayer)

    local curPosX,curPosY = player:getPosition();
    local newPos = cc.p(curPosX + (MovedPos.x - LastMove.x), curPosY + (MovedPos.y - LastMove.y))
    player:setPosition(newPos)
    LastMove = MovedPos;
end

function onTouchEnded(touch, event)
    EndedPos = touch:getLocation()
end

function update(fT)
    -- print ("tick")
end

function receive(str)
    print(str)
end

function CreateGameScene()
	mainLayer = cc.Layer:create()
	
    local bg = cc.Sprite:create("bg.png")
    bg:setPosition(WinSize.width/2,WinSize.height/2);
    bg:setTag(kTagBg)
	mainLayer:addChild( bg )

    local player = createPlayer();
    player:setPosition(WinSize.width/2,WinSize.height/4);
    player:setTag(kTagPlayer)
    mainLayer:addChild( player )

    local scheduler = cc.Director:getInstance():getScheduler()
    updateFunc = scheduler:scheduleScriptFunc(update, 1.0, false)
    -- scheduler:unscheduleScriptEntry(updateFunc)

    local listener = cc.EventListenerTouchOneByOne:create()
    listener:registerScriptHandler(onTouchBegan,cc.Handler.EVENT_TOUCH_BEGAN )
    listener:registerScriptHandler(onTouchMoved,cc.Handler.EVENT_TOUCH_MOVED )
    listener:registerScriptHandler(onTouchEnded,cc.Handler.EVENT_TOUCH_ENDED )
    local eventDispatcher = mainLayer:getEventDispatcher()
    eventDispatcher:addEventListenerWithSceneGraphPriority(listener, mainLayer)

	return mainLayer
end