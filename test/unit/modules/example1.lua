local mymodule = {
	name = 'Example module',
	version = 0,
}
local http = httpsession()
for i=0,10 do
	httpsession()
end

function mymodule.search(query)
	print('user searched "' .. query .. '" ')
	-- return {
	-- 	{ image='' }
	-- }
end

return mymodule
