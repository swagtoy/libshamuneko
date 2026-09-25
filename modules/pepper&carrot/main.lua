local mymodule = {
	name = 'Pepper & Carrot',
	langs = { 'en' },
	url = 'https://www.peppercarrot.com',
	version = 0,
}
local http = httpsession.new()
local _lang = 'en' -- temp
local _url = mymodule.url .. '/0_sources'

local comics = {
	peppercarrot = function (chapter_idx)
		chapter_idx = chapter_idx + 1
		local data = http:request(_url .. '/episodes.json')
		js = json.parse(data)

		local pages = {}

		chapter = js[chapter_idx]
		for page=1,chapter.total_pages do
			local _url = string.format(_url..'/%s/hi-res/%s_Pepper-and-Carrot_by-David-Revoy_E%02dP%02d.jpg',
				chapter.name, _lang, chapter_idx, page)
			table.insert(pages, { img_url = _url })
		end

		return pages
	end
}

function mymodule.search(query)
	return true
end

function mymodule.get_pages(id, chapter)
	return comics[id](chapter)
end

function mymodule.get_trending()
	return {
		{
			id = 'peppercarrot',
			name = 'Pepper & Carrot',
			description = '',
			cover = '',
		},
		{
			id = 'miniFantasyTheater',
			name = 'Mini Fantasy Theater',
			description = '',
			cover = '',
		}
	}
end

return mymodule
