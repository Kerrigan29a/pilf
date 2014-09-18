
local ffi = require 'ffi'

function printf(...)
 io.write(string.format(...))
end

local addrs = 0x10c8af8d0

print("================================================================================\n")
print("ANTES\n")


-- local _c_addrs = ffi.cast("uint32_t *", addrs)
-- printf("_c_addrs = %s\n", tostring(_c_addrs))
-- printf("_c_addrs[0] = 0x%X\n", tonumber(_c_addrs[0]))


--printf("x[1] = %s\n", tostring(x[1]))
--printf("x[2] = %s\n", tostring(x[2]))
--printf("x[3] = %s\n", tostring(x[3]))


local say_hi = ffi.cast('void (*)(void)', addrs)
printf("say_hi = %s\n", tostring(say_hi))
say_hi()
print("DESPUES")
print("================================================================================\n")
