local warna = require("warna")
local constants = require("include.constants")

--> `1' - Basic color support (8-16 colors).
warna.options["level"] = 1

return function(e, before_callback, code)
    code = code or constants.EXIT_FAILURE
    before_callback = before_callback or function() end

    local path = ("%s/.etop.log"):format(os.getenv("HOME"))
    local err_msg = warna.format(
        "Generated %{bold}1 %{bold red}ERROR%{reset}, check: \"%{underline}"
        .. path ..
        "\"%{reset}\n \\----> %{bold red}[ERROR]:%{reset} %{underline}"
        .. e
    )

    --> IMPORTANT: Run the `before_callback' before writting the error
    before_callback()

    io.stderr:write(err_msg, "\n")

    local log_file = io.open(path, "a")

    if log_file then
        log_file:write(string.format(
            "[%s]: %s\n",
            os.date("%Y-%m-%d %H:%M:%S"), e
        ))
        log_file:close()
    end

    --> Exit the program..
    os.exit(code)
end
