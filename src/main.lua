local jit = require("jit")
local window = require("include.win").new()

local function main()
    local ok, status = pcall(function() window:start() end)

    if not ok then
        window:end_session(status)
    end
end

if jit.os ~= "Windows" then
    --[[ IMPORTANT NOTICE:

        * There isnt a portable version of *procfs* on windows;
        * meaning this can't run properly on windows.
        * contact: <myemail@email.com>
    ]]

    main()
end
