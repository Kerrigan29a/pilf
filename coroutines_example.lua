
local function helper(f, n, a, ...)
  if n > 0 then return f(a), helper(f, n-1, ...) end
end
local function map(f, ...)
  return helper(f, select('#', ...), ...)
end

local LOG = function(who, ...)
  io.stderr:write(os.date("!%F %T "))
  io.stderr:write(("[%10s] "):format(who))
  io.stderr:write(...)
  io.stderr:write"\n"
end

-- This function expects to be a coroutine. When it is started,
-- it initializes itself, and then awaits further instructions.
-- Whenever it has nothing else to do it yields, unless it was
-- told to quit in which case it returns.

function supervisor(...)
  -- each event is a table of sets of coroutines;
  -- we assume we can handle the events in any order.
  local events = {}
  -- This table associates coroutines with names, mostly
  -- for debugging
  local name = {}
  -- to signal that we're done
  local done = false

  -- This function removes all references to a coro
  -- from all events, which we do when the coro dies.
  -- We could use weak tables to do the same thing.
  -- On the other hand, we might want to clean up other
  -- data associated with the coroutine. Anyway, this is easy:

  local function remove(coro)
    for _, coros in pairs(events) do
      coros[coro] = nil
    end
    name[coro] = nil
  end

  -- Convenience function to log tracebacks
  local function log_traceback(coro, msg, err)
    LOG("supervisor", "Coroutine ", name[coro], " ", msg, ":")
    local tb = debug.traceback(coro, err)
    for line in tb:gmatch"[^\n]+" do
      LOG("supervisor", "  ", line)
    end
  end

  -- The core routine, which handles the results of a
  -- coroutine.resume. First argument is the coro, rest
  -- are the return values from coro.resume(). The only
  -- thing we're expecting from the coro is a "waitfor"
  -- command, but other ones could be added...
  local function handle_resume(coro, ok, todo, ...)
    if coroutine.status(coro) == "dead" then
      remove(coro) -- always get rid of it.
      -- log a traceback on error
      if not ok then
        log_traceback(coro, "failed with error", todo)
      end
    elseif todo == "waitfor" then
      -- We don't care if the event doesn't exist (should we?)
      local coros = events[...]
      if coros == nil then
        coros = {}
        events[...] = coros
      end
      -- We might want to tell the upper level that a new
      -- event needs to be recognized, though.
      -- if next(coros) == nil then addevent((...)) end
      coros[coro] = true
    else
      -- todo should be "waitfor" and ... the event.
      -- If we had more verbs, we could use a case-table.
      log_traceback(coro, "unknown request "..tostring(todo),
        "bad return value")
      remove(coro)
    end
  end 

  -- A table of actions; essentially a case statement
  local handler = {}

  -- do a clean exit (although actually there's no
  -- cleanup necessary, but there might be.)
  function handler.done()
    done = true
  end

  -- debugging: report on status
  function handler.status()
    -- invert the events table for nicer printing
    local n, e = {}, {}
    for event_name, coros in pairs(events) do
      for coro in pairs(coros) do
        local coro_name = name[coro]
        n[#n+1] = coro_name
        e[coro_name] = event_name
      end
    end
    -- sort the names
    table.sort(n)
    -- and produce the report
    for _, coro_name in ipairs(n) do
      LOG("supervisor", coro_name, " is waiting for ", tostring(e[coro_name]))
    end
  end

  -- introduce a new actor (coroutine) into the system, and run
  -- it until it blocks
  function handler.introduce(coro_name, func, ...)
    local coro = coroutine.create(func)
    name[coro] = coro_name
    -- let it initialize itself
    return handle_resume(coro, coroutine.resume(coro, ...))
  end

  -- send an event to whoever cares
  function handler.signal(event_name, ...)
    local coros = events[event_name]
    if coros and next(coros) then
      for coro in pairs(coros) do
        coros[coro] = nil  -- handled
        handle_resume(coro, coroutine.resume(coro, event_name, ...))
      end
      -- Maybe tell the top-level whether the event is
      -- still active?
      return next(coros) ~= nil
    else
      -- No-one cares, sniff. "Log" the fact
      LOG("supervisor", "Event ", tostring(event_name), " dropped into the bit bucket")
    end
  end

  -- Set the __index meta for the handler table to avoid having
  -- to test for bad commands explicitly
  local function logargs(...)
    local t = {n = select("#", ...), ...}
    if t.n > 0 then
      for i = 1, t.n do
        local ti = t[i]
        t[i] = (type(ti) == "string" and "%q" or "%s")
        :format(tostring(ti))
      end
      LOG("supervisor", "..with arguments: ", table.concat(t, ", "))
    end
  end

  function handler:__index(msg)
    LOG("supervisor", "Supervisor received unknown message: ", msg)
    return logargs
  end

  setmetatable(handler, handler)

  -- auxiliary function to handle a command, necessary to
  -- capture multiple returns from coroutine.yield()
  local function handle(msg, ...)
    -- if ... then
    --   LOG("supervisor", "HANDLE: msg = ", msg, "    args = ", table.concat({map(tostring, ...)}, ", "))
    -- else
    --   LOG("supervisor", "HANDLE: msg = ", msg)
    -- end
    return handler[msg](...)
  end

  -- The main loop is a bit anti-climactic
  LOG("supervisor", "Starting up")
  local rv = handle(...)
  repeat 
    rv = handle(coroutine.yield(rv))
  until done
  LOG("supervisor", "Shutting down")
end

------------------------------------------------------------------------------
--  Sample very simple processor
------------------------------------------------------------------------------

local function block(event)
  return coroutine.yield("waitfor", event)
end

-- Initialize the pseudo random number generator
math.randomseed(tonumber(tostring(os.time()):reverse():sub(1,6)))

local function process(n, name)
  LOG(name, "starting")
  local max = math.random(n)
  LOG(name, "max = ", max)
  for i = 1, math.random(n) do
    local e, howmany = block "TICK"
    assert(e == "TICK" and howmany == i)
  end
  LOG(name, "done")
end

------------------------------------------------------------------------------
-- Test driver in Lua
------------------------------------------------------------------------------

local super = coroutine.create(supervisor)

local function check(ok, rv)
  if not ok then
    LOG("main", "supervisor crashed")
    local tb = debug.traceback(super, rv)
    for line in tb:gmatch"[^\n]+" do
      LOG("main", "  ", line)
    end
    os.exit(1)
  end
  return rv
end

local function send(msg, ...)
  return check(coroutine.resume(super, msg, ...))
end

local nprocess, nreps = tonumber(arg[1]) or 10, tonumber(arg[2] or 12)
for i = 1, tonumber(nprocess) do
  coro_name = ("worker%04i"):format(i)
  send("introduce", coro_name, process, nreps, coro_name)
end
send "status"
local j = 1
while send("signal", "TICK", j) do
  j = j + 1
end
-- This should be empty
send "<wrong msg>"
send "status"
send "done"
LOG("main", "Endcount ", j)