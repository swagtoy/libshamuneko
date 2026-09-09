local mymodule = {
	name = 'Example module',
	version = 0,
}
local http = httpsession.new()
local http2 = httpsession.new()

function mymodule.search(query)
	http:request("https://google.com", function(data)
		print('user searched "' .. query .. '" and got data: ' .. data)
		print('Got data: ' .. data)
	end)
	http2:request("hello", function(data)
		print("This is the second object")
	end)
	http2:request("hi", function(data)
		print("This is another object")
	end)
	--http.request("hi")
	-- return {
	-- 	{ image='' }
	-- }
end

return mymodule
