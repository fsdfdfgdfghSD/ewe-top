local lproc = require("lproc")

for _, proc in ipairs(lproc()) do
    print (
        "PID: " .. proc.pid,
        "CMD: " .. proc.cmd,
        "USER: " .. proc.user,
        "CPU: " .. proc.cpu,
        "MEM: " .. proc.mem,
        "STATE: " .. proc.state
    )
end
