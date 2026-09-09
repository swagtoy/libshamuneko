local mymodule = {
	name = 'Example module',
	version = 0,
}
local http = httpsession()

function mymodule.search(query)
	http.request("https://google.com", function(data)
		print('user searched "' .. query .. '" and got data: ' .. data)
		print('Got data: ' .. data)
	end)
	--http.request("hi")
	-- return {
	-- 	{ image='' }
	-- }
end

return mymodule
