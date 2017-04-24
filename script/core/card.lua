local Card = {}
Card.__index = Card

function Card.new(name, desc, power, hp)
    local self = setmetatable({}, Card)
    self.name = name
    self.desc = desc
    self.power = power
    self.hp    = hp
    self.hooks = {
        selfsummon = (function(self) print(self.name .. " summoned (" .. self.power .. "/" .. self.hp .. ")") end),
        selfdied = (function(self, res, target) print(self.name .. " died") end),
        attacking = (function(self, target) print(self.name .. " attacking " .. target.name) end)
    }
    return self
end

function Card.hook(self, ev, fn)
    self.hooks[ev] = fn
end

function Card.call(self, ev, ...)
    if self.hooks[ev] ~= nil then
        self.hooks[ev](self, ...)
    end
end

function Card.mod_hp(self, n)
    self.hp = self.hp + n
end

function Card.damage(self, res, n)
    self:mod_hp(-n)
    self:call("selfdamaged", res)
end

function Card.attack(self, target)
    self:call("attacking", target)
    target:call("attacked", self)
    self:mod_hp(-target.power)
    target:mod_hp(-self.power)

    if self.hp <= 0 then self:call("selfdied", "damage", target) end
    if target.hp <= 0 then target:call("selfdied", "damage", self) end
end

_G["Card"] = Card
