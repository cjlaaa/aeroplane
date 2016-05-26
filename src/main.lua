cc.FileUtils:getInstance():addSearchPath("src/")
cc.FileUtils:getInstance():addSearchPath("res/")
require "cocos.init"

require "GameScene"

local scene = cc.Scene:create()
scene:addChild(CreateGameScene())
cc.Director:getInstance():runWithScene(scene)
