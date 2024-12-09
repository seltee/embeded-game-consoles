local sprite = app.activeSprite

-- Check constraints
if sprite == nil then
  app.alert("No Sprite...")
  return
end
if sprite.colorMode ~= ColorMode.INDEXED then
  app.alert("Sprite needs to be indexed")
  return
end

local function getPaletteData(palette)
	local ncolors = #palette
	local res = string.format("const unsigned char palleteSize = %i;\n", ncolors)

	res = res .. string.format("const short int palette[] = { ", rgb565)	
	for i=0, ncolors-1 do
		local color = palette:getColor(i)
		local rgb565 = ((color.red >> 3) << 11) | ((color.green >> 2) << 5) | (color.blue >> 3)
		rgb565 = ((rgb565 >> 8) | (rgb565 << 8)) & 0xffff
		res = res .. string.format("0x%04X", rgb565)	
		if i < ncolors-1 then
			res = res .. ", "
		end
	end
	res = res .. " };\n"

	return res
end

local function getIndexData8bit(img, x, y, w, h)
	local res = "const unsigned char spriteData[] = {\n"
	for y = 0,h-1 do
		for x = 0, w-1 do
			px = img:getPixel(x, y)
			res = res .. string.format("%i", px)
			if y < h - 1 or x < w - 1 then
				res = res .. ", "
			end
		end
		res = res .. "\n"
	end
		res = res .. "};\n"

	return res
end

local function getIndexData4bit(img, x, y, w, h)
	local res = "const unsigned char spriteData[] = {\n"
	for y = 0,h-1 do
		for x = 0, (w >> 1)-1 do
			px1 = img:getPixel(x * 2, y)
			px2 = img:getPixel(x * 2 + 1, y)
			res = res .. string.format("%i", (px1 << 4) | (px2 & 0x0f))
			if y < h - 1 or x < w - 1 then
				res = res .. ", "
			end
		end
		res = res .. "\n"
	end
		res = res .. "};\n"

	return res
end

local function getIndexData2bit(img, x, y, w, h)
	local res = "const unsigned char spriteData[] = {\n"
	for y = 0,h-1 do
		for x = 0, (w >> 2)-1 do
			px1 = img:getPixel(x * 2, y)
			px2 = img:getPixel(x * 2 + 1, y)
			px3 = img:getPixel(x * 2 + 2, y)
			px4 = img:getPixel(x * 2 + 3, y)
			res = res .. string.format("%i", (px1 << 6) | (px2 << 4) | (px3 << 2) | (px4 & 0x03))
			if y < h - 1 or x < w - 1 then
				res = res .. ", "
			end
		end
		res = res .. "\n"
	end
		res = res .. "};\n"

	return res
end

local function getIndexData1bit(img, x, y, w, h)
	local res = "const unsigned char spriteData[] = {\n"
	for y = 0,h-1 do
		for x = 0, (w >> 3)-1 do
			px1 = img:getPixel(x * 2, y)
			px2 = img:getPixel(x * 2 + 1, y)
			px3 = img:getPixel(x * 2 + 2, y)
			px4 = img:getPixel(x * 2 + 3, y)
			px5 = img:getPixel(x * 2 + 4, y)
			px6 = img:getPixel(x * 2 + 5, y)
			px7 = img:getPixel(x * 2 + 6, y)
			px8 = img:getPixel(x * 2 + 7, y)
			res = res .. string.format("%i", (px1 << 7) | (px2 << 6) | (px3 << 5) | (px4 << 4) | (px5 << 3) | (px6 << 2) | (px7 << 1) | (px8 & 0x01))
			if y < h - 1 or x < w - 1 then
				res = res .. ", "
			end
		end
		res = res .. "\n"
	end
		res = res .. "};\n"

	return res
end

local function exportFrame(frm)
	if frm == nil then
		frm = 1
	end

	local img = Image(sprite.spec)
	img:drawSprite(sprite, frm)
	local ncolors = #sprite.palettes[1]
	
	local res = string.format("const unsigned char palleteSize = %i;\n", ncolors)
	io.write(getPaletteData(sprite.palettes[1]))
	if ncolors > 16 then
		io.write("const unsigned char spriteBitsPerPixel = 8;\n")
		io.write(getIndexData8bit(img, x, y, sprite.width, sprite.height))
	elseif ncolors > 4 then
		io.write("const unsigned char spriteBitsPerPixel = 4;\n")
		io.write(getIndexData4bit(img, x, y, sprite.width, sprite.height))
	elseif ncolors > 2 then
		io.write("const unsigned char spriteBitsPerPixel = 2;\n")
		io.write(getIndexData2bit(img, x, y, sprite.width, sprite.height))
	end
end


local dlg = Dialog()
dlg:file{ id="exportFile",
          label="File",
          title="Gameboy-Assembler Export",
          open=false,
          save=true,
        --filename= p .. fn .. "pip",
          filetypes={"txt", "pip" }}
dlg:check{ id="onlyCurrentFrame",
           text="Export only current frame",
           selected=true }

dlg:button{ id="ok", text="OK" }
dlg:button{ id="cancel", text="Cancel" }
dlg:show()
local data = dlg.data
if data.ok then
	local f = io.open(data.exportFile, "w")
	io.output(f)

	if data.onlyCurrentFrame then
		exportFrame(app.activeFrame)
	else
	 	for i = 1,#sprite.frames do
	 		io.write(string.format(";Frame %d\n", i))
	 		exportFrame(i)
		end
	end

	io.close(f)
end