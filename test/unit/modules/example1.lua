local mymodule = {
	name = 'Example module',
	version = 0,
}
local http = httpsession.new()

function mymodule.search(query)
	local data, err = http:request("https://example.com")

	print('user searched "' .. query .. '" and got data: ' .. data)

	local html = htmlparser.new(data)
	local head = html:children():next():children(2)
	local pps = head:filter("p", function (e) print(e:content(true)) end)
	for i, v in ipairs(pps) do
		print(v:name())
	end
	-- idea, ignore
	--print('Site title: ' .. :select('meta', { name = 'viewport' }):attr()['content'])
end

return mymodule
