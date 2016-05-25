
local MainScene = class("MainScene", cc.load("mvc").ViewBase)

function MainScene:onCreate()
    -- add background image
    display.newSprite("bg.png")
        :move(display.center)
        :addTo(self)

    local player = display.newSprite("player.png")
        :move(display.center)
        :addTo(self)
   	local x,y = player:getPosition()
   	print ("xxxxxxx"..x.." "..y)

   	local sprite = cc.Sprite:create("cloud.png")
   	self:addChild(sprite)
   	sprite:setPosition(display.center)
   	x,y = sprite:getPosition()
   	print ("xxxxxxx"..x.." "..y)

    -- add HelloWorld label
    cc.Label:createWithSystemFont("Hello World", "Arial", 40)
        :move(display.cx, display.cy + 200)
        :addTo(self)

end

return MainScene
