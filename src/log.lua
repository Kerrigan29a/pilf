
function log(...)
  io.stderr:write(os.date("!%F %T "))
  io.stderr:write("[%10s] ")
  io.stderr:write(...)
  io.stderr:write"\n"
end

return { log = log }