-- Lua5.2 bit32

local bit = {}

function bit.bnot(a) return bit32.bnot(a) end
function bit.band(a, b) return bit32.band(a, b) end
function bit.bor(a, b) return bit32.bor(a, b) end
function bit.bxor(a, b) return bit32.bxor(a, b) end
function bit.lshift(a, b) return bit32.lshift(a, b) end
function bit.rshift(a, b) return bit32.rshift(a, b) end

return bit
