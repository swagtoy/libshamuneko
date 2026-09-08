local mymodule = {
	name = 'Example module',
	version = 0,
}

function mymodule.search(query)
	print('user searched "' .. query .. '"')
	-- return {
	-- 	{ image='' }
	-- }
end

return mymodule
