-- TODO: Get the power-usage, based of mem and cpu.

local lproc = require("include.proc.lib.lproc_lib")
local bit

if _VERSION == "Lua 5.1" then
    --> LuaJIT bitop <https://bitop.luajit.org/>

    bit = require("bit")
elseif _VERSION == "Lua 5.2" then
    --> bit32 built-in library

    bit = require("lib.bit_bit32")
elseif _VERSION == "Lua 5.3" or _VERSION == "Lua 5.4" then
    --> Built-in bitwise operations

    bit = require("lib.bit_lua53")
else
    error("Unsupported Lua Version.")
end

local pflags = {
    PROC_FILLMEM        = 0x0001,
    PROC_FILLSTATUS     = 0x0020,
    PROC_FILLSTAT       = 0x0040
}

return function(proc_flags)
    proc_flags = proc_flags or bit.bor(
        pflags["PROC_FILLMEM"],
        pflags["PROC_FILLSTAT"],
        pflags["PROC_FILLSTATUS"]
    )

    local handler = lproc.openproc(proc_flags)
    local processes = {}
    
    while true do
        local data = lproc.readproc(handler)
        if not data then break end
        
        table.insert(processes, data)
    end

    lproc.closeproc(handler)
    return processes
end
