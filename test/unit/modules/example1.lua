local mymodule = {
	name = 'Example module',
	version = 0,
}
local http = httpsession.new()

function mymodule.search(query)
	http:request("https://example.com", function(data)
		print('user searched "' .. query .. '" and got data: ' .. data)
	end)
end

return mymodule
