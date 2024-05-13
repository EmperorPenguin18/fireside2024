local turn = 0
local tiles = {}
local tiles_type = {} -- 0 grass, 1 weed, 2 dirt
local mode = 0
local selector = nil
local sel_x = 0
local sel_y = 0
local sel_rot = 0
local text = nil
local bunnies = {}
local bunnies_x = {}
local bunnies_y = {}
local bunny_count = 0
local AP <const> = 2

local function bunny()
	for i=1,bunny_count do
		local new_x = bunnies_x[i]
		local new_y = bunnies_y[i]
		for n=1,AP do
			local close_x = 100
			local close_y = 100
			for j=1,5 do
				for k=1,5 do
					if tiles_type[j][k] == 1 then
						if math.abs(j - bunnies_x[i]) < close_x or math.abs(k - bunnies_y[i]) < close_y then
							close_x = j
							close_y = k
						end
					end
				end
			end
			if close_x > new_x then
				new_x = new_x + 1
			elseif close_x < new_x then
				new_x = new_x - 1
			elseif close_y > new_y then
				new_y = new_y + 1
			elseif close_y < new_y then
				new_y = new_y - 1
			else --eat
				tiles_type[new_x][new_y] = 2
				tidal.set_sprite(tiles[new_x][new_y], "dirt.png")
			end
		end
		bunnies_x[i] = new_x
		bunnies_y[i] = new_y
		tidal.set_pos(bunnies[i], (new_x-1)*32, (new_y-1)*32)
	end
	if (turn-2) % 5 == 0 then
		bunny_count = bunny_count + 1
		bunnies[bunny_count] = tidal.create()
		local rand_x = 0
		local rand_y = 0
		repeat
			rand_x = math.random(0, 4)
			rand_y = math.random(0, 4)
		until (
			(rand_y == 0) or
			(rand_x == 0) or
			(rand_x == 4) or
			(rand_y == 4))
		bunnies_x[bunny_count] = rand_x+1
		bunnies_y[bunny_count] = rand_y+1
		tidal.set_pos(bunnies[bunny_count], rand_x*32, rand_y*32)
		tidal.set_size(bunnies[bunny_count], 32, 32)
		tidal.set_sprite(bunnies[bunny_count], "bunny.png")
	end
end

local function mode0(key)
	if key == 262 then --right
		if sel_x < 4 then
			sel_x = sel_x + 1
		end
		tidal.set_pos(selector, sel_x*32, sel_y*32)
	elseif key == 263 then --left
		if sel_x > 0 then
			sel_x = sel_x - 1
		end
		tidal.set_pos(selector, sel_x*32, sel_y*32)
	elseif key == 264 then --down
		if sel_y < 4 then
			sel_y = sel_y + 1
		end
		tidal.set_pos(selector, sel_x*32, sel_y*32)
	elseif key == 265 then --up
		if sel_y > 0 then
			sel_y = sel_y - 1
		end
		tidal.set_pos(selector, sel_x*32, sel_y*32)
	elseif key == 90 then --z
		if tiles_type[sel_x+1][sel_y+1] == 1 then
			mode = 1
			tidal.set_sprite(selector, "arrow.png")
		end
	elseif key == 88 then --x
		--nothing
	elseif key == 81 then --q
		tidal.quit()
	end
end

local function pass_turn()
	mode = 0
	turn = turn + 1
	tidal.set_text(text, "Turn: " .. turn)
	bunny()
	tidal.set_sprite(selector, "selector.png")
end

local function mode1(key)
	if key == 262 then --right
		sel_rot = 0
		tidal.set_rotation(selector, sel_rot)
	elseif key == 263 then --left
		sel_rot = 180
		tidal.set_rotation(selector, sel_rot)
	elseif key == 264 then --down
		sel_rot = 90
		tidal.set_rotation(selector, sel_rot)
	elseif key == 265 then --up
		sel_rot = 270
		tidal.set_rotation(selector, sel_rot)
	elseif key == 90 then --z
		local sel_x_new = sel_x
		local sel_y_new = sel_y
		if sel_rot == 0 and sel_x ~= 4 then
			sel_x_new = sel_x_new + 1
		elseif sel_rot == 90 and sel_y ~= 4 then
			sel_y_new = sel_y_new + 1
		elseif sel_rot == 180 and sel_x ~= 0 then
			sel_x_new = sel_x_new - 1
		elseif sel_rot == 270 and sel_y ~= 0 then
			sel_y_new = sel_y_new - 1
		end
		if tiles_type[sel_x_new+1][sel_y_new+1] == 0 then
			tiles_type[sel_x_new+1][sel_y_new+1] = 1
			tidal.set_sprite(tiles[sel_x_new+1][sel_y_new+1], "weed.png")
			pass_turn()
		elseif tiles_type[sel_x_new+1][sel_y_new+1] == 2 then
			tiles_type[sel_x_new+1][sel_y_new+1] = 0
			tidal.set_sprite(tiles[sel_x_new+1][sel_y_new+1], "grass.png")
			pass_turn()
		end
	elseif key == 88 then --x
		mode = 0
		tidal.set_sprite(selector, "selector.png")
	elseif key == 81 then --q
		tidal.quit()
	end
end

function keydown(key)
	if mode == 0 then
		mode0(key)
	else
		mode1(key)
	end
end

tidal.set_music("music.ogg")
tidal.set_background_colour(255, 153, 102, 255)
tidal.set_cb_keydown(keydown)

for i=1,5 do
	tiles[i] = {}
	tiles_type[i] = {}
	for j=1,5 do
		tiles[i][j] = tidal.create()
		tidal.set_pos(tiles[i][j], (i-1)*32, (j-1)*32)
		tidal.set_size(tiles[i][j], 32, 32)
		tidal.set_sprite(tiles[i][j], "grass.png")
		tiles_type[i][j] = 0
	end
end
tidal.set_sprite(tiles[3][3], "weed.png")
tiles_type[3][3] = 1

selector = tidal.create()
tidal.set_pos(selector, sel_x*32, sel_y*32)
tidal.set_size(selector, 32, 32)
tidal.set_sprite(selector, "selector.png")

text = tidal.create()
tidal.set_pos(text, 170, 12)
tidal.set_font(text, "fibberish.ttf")
tidal.set_text(text, "Turn: " .. turn)

math.randomseed(os.time())

