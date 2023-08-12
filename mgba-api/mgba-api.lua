local server_fd = socket:tcp()

local ip_address = "127.0.0.1"
local port = 61337

local MSG_LENGTH = 128

local socket_initialized = 0

function establish_server_connection()
	server_fd:connect( ip_address, port )
	console:log("[+] connected to: " .. ip_address .. ":" .. port )
	socket_initialized = 1
end

function on_receive()
    if server_fd:hasdata() then
        local msg = server_fd:receive(MSG_LENGTH)
        if msg ~= nil then
			console:log("[+] server message: " .. msg )
			local parts = {}

			for part in string.gmatch(msg, "%S+") do
				table.insert(parts, part)
			end

			if #parts == 1 then
				local instruction = parts[1]
				
				console:log("[api:on_receive] received: " .. instruction)
				
				if instruction == "GET_GAME_TITLE" then
					local value = emu:getGameTitle()
					server_fd:send(value)
					console:log("[api::get_game_title] sent value " .. value .. " to server")
				end

				if instruction == "GET_GAME_CODE" then
					local value = emu:getGameCode()
					server_fd:send(value)
					console:log("[api::get_game_code] sent value " .. value .. " to server")
				end
				
			end

			if #parts == 2 then
				local instruction = parts[1]
				local parameter = parts[2]
				
				console:log("[api:on_receive] received: " .. instruction .. " with parameter: " .. parameter)
				
				if instruction == "READ8" then
					local value = emu:read8( tonumber( parameter, 16 ) )
					server_fd:send(value)
					console:log("[api::read8] sent value " .. value .. " to server")
				end
				
				if instruction == "READ16" then
					local value = emu:read16( tonumber( parameter, 16 ) )
					server_fd:send(value)
					console:log("[api::read16] sent value " .. value .. " to server")
				end
				
				if instruction == "READ32" then
					local value = emu:read32( tonumber( parameter, 16 ) )
					server_fd:send(value)
					console:log("[api::read32] sent value " .. value .. " to server")
				end

				if instruction == "PRESS_KEY" then
					local value = tonumber( parameter, 10 )
					emu:addKey( value )
					server_fd:send("OK")
					console:log("[api::press_key] pressed key: " .. value )
				end

				if instruction == "RELEASE_KEY" then
					local value = tonumber( parameter, 10 )
					emu:clearKey( value )
					server_fd:send("OK")
					console:log("[api::release_key] released key: " .. value )
				end
			end	
			
			if #parts == 3 then
				local instruction = parts[1]
				local parameter1 = parts[2]
				local parameter2 = parts[3]
				
				console:log("[api:on_receive] received: " .. instruction .. " with parameter1: " .. parameter1 .. " , parameter2: " .. parameter2 )
				
				if instruction == "WRITE8" then
					local _addr = tonumber( parameter1, 16 )
					local _value = tonumber( parameter2, 10 )
					emu:write8( _addr, _value )
					server_fd:send("OK")
					console:log("[api::write8] wrote value: " .. _value .. " to: " .. _addr)
				end

				if instruction == "WRITE16" then
					local _addr = tonumber( parameter1, 16 )
					local _value = tonumber( parameter2, 10 )
					emu:write16( _addr, _value )
					server_fd:send("OK")
					console:log("[api::write16] wrote value: " .. _value .. " to: " .. _addr)
				end

				if instruction == "WRITE32" then
					local _addr = tonumber( parameter1, 16 )
					local _value = tonumber( parameter2, 10 )
					emu:write32( _addr, _value )
					server_fd:send("OK")
					console:log("[api::write32] wrote value: " .. _value .. " to: " .. _addr)
				end
			end
		end
    else
        console:log("[-] no message received from server")
    end
end


function on_frame()
	if socket_initialized == 0 then
		establish_server_connection()
	end
end

server_fd:add("received", on_receive)

callbacks:add("frame", on_frame)