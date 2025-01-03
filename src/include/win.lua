--[[

    TODO: Selecting (F9 raises the SIGTERM signal for that pid)
    TODO: Refreshing every 3 seconds, make it also work based of the `-d, --delay' option.
    TODO: Remove line (we'll need this once a signal has been ended or "termed")

]]

local curses = require("curses")

local cerror = require("include.error")
local constants = require("include.constants")
local lproc = require("include.proc.lproc")

local win = {}
win.__index = win

function win.new()
    local self = setmetatable({
        stdscr = curses.initscr(),

        content = {},

        visible_start = 1,
        visible_count = curses.lines()
    }, win)

    self:_init_win()
    return self
end

function win:_init_win()
    self.stdscr:keypad  (true)    --> enable key padding support
    self.stdscr:nodelay (true)    --> disable block during read
    curses.raw          (true)    --> see: <https://en.wikipedia.org/wiki/Terminal_mode>
    curses.echo         (false)   --> dont echo any characters
    curses.curs_set     (0)       --> `0' hides the cursor

    self.visible_end = (
        self.visible_start + self.visible_count - 1
    )
end

function win:write(s)
    table.insert(self.content, s)
    self:redraw()
end

function win:draw_line(y, s)
    self.stdscr:move(y, 0)
    self.stdscr:clrtoeol()
    self.stdscr:addstr(s or "")
end

function win:redraw()
    for i = 1, self.visible_count do
        local line_index = self.visible_start + i - 1
        local line_text = self.content[line_index]

        self:draw_line(i - 1, line_text)
    end

    self.stdscr:refresh()
end

function win:scroll_up()
    if self.visible_start > 1 then
        self.visible_start = self.visible_start - 1
        self.visible_end = self.visible_end - 1

        self:redraw()
    end
end

function win:scroll_down()
    if self.visible_end < #self.content then
        self.visible_start = self.visible_start + 1
        self.visible_end = self.visible_end + 1

        self:redraw()
    end
end

function win:end_session(e)
    cerror(e, function()
        self.stdscr:endwin()
    end)
end

function win:display_processes()
    local new_content = {}

    for _, proc in ipairs(lproc()) do
        local fmt = ("%d\t%-20s\t%-1s\t%d\t%d\t%s"):format(
            proc.pid,
            proc.user,
            proc.state,
            proc.cpu,
            proc.mem,
            proc.cmd
        )

        table.insert(new_content, fmt)
    end

    self.content = new_content
    self:redraw()
end

function win:start()
    self:display_processes()

    local last_refresh = os.time()
    local refresh_interval = 3

    while true do
        local ch = self.stdscr:getch()

        if ch then
            if ch == constants.EXIT_KEY then
                self.stdscr:endwin()
                break
            elseif ch == curses.KEY_F10 then
                self.stdscr:endwin()
                break

            -- Scrolling...
            elseif ch == curses.KEY_UP then
                self:scroll_up()
            elseif ch == curses.KEY_DOWN then
                self:scroll_down()
            end
        end

        if last_refresh - os.clock() >= refresh_interval then
            self:display_processes()
            last_refresh = os.time()
        end
    end
end

return win
