local mymodule = {
	name = 'Example module',
	version = 0
}

function mymodule.thing()
	print('Hello world')
end

mymodule.thing()

return mymodule
