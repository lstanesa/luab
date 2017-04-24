r = Card.new("Red Goblin", "", 5, 1)
c = Card.new("Rusted Machina", "", 2, 3)

function checkdied(creature)
    if not conditions.isalive(creature) then
        print(creature:getname() .. " has " .. r:getfort() .. " fortitude remaining")
    else
        print(creature:getname() .. " has died")
    end
end

function attack(atk, tgt)
    atk:attack(tgt)
    print(atk:getname() .. " attacks for " .. atk:getpower() .. " damage")
    print(tgt:getname() .. " defends for " .. tgt:getpower() .. " damage")

    checkdied(atk)
    checkdied(tgt)
end

attack(r, c)
