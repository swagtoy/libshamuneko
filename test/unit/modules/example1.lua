local mymodule = {
	name = 'Example module',
	version = 0,
}
local http = httpsession.new()

function mymodule.search(query)
	http:request("https://example.com", function(data)
		print('user searched "' .. query .. '" and got data: ' .. data)

		local html = htmlparser.new(data)
		print('Site title: ' .. html:children(3):content())

	end)
end

return mymodule
