
asset = {
	size_px =  32,
	dir_global = 	"textures\\",
	dir_block = 	"blocks\\",
	dir_items = 	"items\\",
	dir_env = 		"environment\\",
	tex_block = 
	{
		{path = "grass_top.png", 				color = {0,255,0}		},	--0
		{path = "grass_side.png", 				color = 255				},	--1
		{path = "grass_top.png", 				color = {255,100,20}	},	--2
		{path = "sand.png", 					color = 255				},	--3
		{path = "snow.png", 					color = 255				},	--4
		{path = "grass_side_snowed.png", 		color = 255				},	--5
		{path = "stone.png",					color = 255				},	--6
		{path = "glass.png",					color = 255				},	--7
		{path = "glowstone.png",				color = 255				},	--8
		{path = "bedrock.png",					color = 255				},	--9
		{path = "redstone_lamp_on.png",			color = 255				},	--10
		{path = "double_plant_grass_top.png",	color = {0,255,0}		},	--11
		{path = "beacon.png",					color = 255				},	--12
		{path = "redstone_block.png",			color = 255				},	--13
		--logs
		{path = "log_oak.png"											},	--14
		{path = "log_oak_top.png"										},	--15
		{path = "log_big_oak.png"										},	--16
		{path = "log_big_oak_top.png"									},	--17
		{path = "log_spruce.png"										},	--18
		{path = "log_spruce_top.png"									},	--19
		{path = "log_jungle.png"										},	--20
		{path = "log_jungle_top.png"									},	--21
		{path = "log_birch.png"											},	--22
		{path = "log_birch_top.png"										},	--23
		{path = "log_acacia.png"										},	--24
		{path = "log_acacia_top.png"									},	--25
		--leaves
		{path = "leaves_oak.png",				color = {0,255,0}		},	--26
		{path = "leaves_big_oak.png",			color = {0,255,0}		},	--27
		{path = "leaves_spruce.png",			color = {0,255,0}		},	--28
		{path = "leaves_jungle.png",			color = {0,255,0}		},	--29
		{path = "leaves_birch.png",				color = {0,255,0}		},	--30
		{path = "leaves_acacia.png",			color = {0,255,0}		},	--31
		--liquids
		{path = "water_still.png",				frame  = 32				},	--32
		{path = "water_flow.png",				frame  = 32				},	--33
		
	},
	tex_items = 
	{
		{path = "bucket_water.png"},									--0
		{path = "bucket_empty.png"}										--1
	},
	tex_env = 
	{
		{path = "sun.png"},									--0
		{path = "rain.png"},									--1
		{path = "moon_phases.png", col = 4, row = 2}	--2
	}
	
}

NON 	= 0
NORM 	= 1
SPEC    = 2
ALPHA   = 3
--PHYS
GAS  =0
SOLID 	= 1
LIQUID 	= 2


pack = {
	name = "default",
	version = "0",
	--side = [left,right,bottom,top,back,forward]!!!
	--emssion.r,g,b<16
	blocks = 
	{
		{	--0
			name = "air",
			drawGroup = NON,
			physGroup = GAS
		},
		{	--1
			name = "turf", 
			drawGroup = NORM,
			physGroup = SOLID,
			side = {1,1,2,0,1,1},
			emission = NON
		},
		{	--2
			name = "earth",
			drawGroup = NORM,
			physGroup = SOLID,
			side = 2,
			emission = NON
		},
		{	--3
			name = "sand",
			drawGroup = NORM,
			physGroup = SOLID,
			side = 3,
			emission = NON
		},
		{	--4
			name = "snow",
			drawGroup = NORM,
			physGroup = SOLID,
			side = {5,5,2,4,5,5},
			emission = NON
		},
		{	--5
			name = "stone",
			drawGroup = NORM,
			physGroup = SOLID,
			side = 6,
			emission = NON
		},
		{	--6
			name = "glass",
			drawGroup = SPEC,
			physGroup = SOLID,
			side = 7,
			emission = NON
		},
		{	--7
			name = "glowstone",
			drawGroup = NORM,
			physGroup = SOLID,
			side = 8,
			emission = 10
		},
		{	--8
			name = "bedrock",
			drawGroup = NORM,
			physGroup = SOLID,
			side = 9,
			emission = NON
		},
		{	--9
			name = "redstone_lamp",
			drawGroup = NORM,
			physGroup = SOLID,
			side = 10,
			emission = {10,3,3}
		},
		{	--10
			name = "oak",
			drawGroup = NORM,
			physGroup = SOLID,
			side ={14,14,15,15,14,14},
			emission = NON
		},
		{	--11
			name = "big_oak",
			drawGroup = NORM,
			physGroup = SOLID,
			side ={16,16,17,17,16,16},
			emission = NON
		},
		{	--12
			name = "spruce",
			drawGroup = NORM,
			physGroup = SOLID,
			side ={18,18,19,19,18,18},
			emission = NON
		},
		{	--13
			name = "jungle",
			drawGroup = NORM,
			physGroup = SOLID,
			side ={20,20,21,21,20,20},
			emission = NON
		},
		{	--14
			name = "birch",
			drawGroup = NORM,
			physGroup = SOLID,
			side ={22,22,23,23,22,22},
			emission = NON
		},
		{	--15
			name = "acacia",
			drawGroup = NORM,
			physGroup = SOLID,
			side ={24,24,25,25,24,24},
			emission = NON
		},
		{	--16
			name = "double_plant_grass",
			drawGroup = SPEC,
			physGroup = GAS,
			side = 11,
			emission = NON
		},
		{	--17
			name = "beacon",
			drawGroup = NORM,
			physGroup = SOLID,
			side =12,
			emission = {2,2,10}
		},
		{	--18
			name = "redstone", 
			drawGroup = NORM,
			physGroup = SOLID,
			side = 13,
			emission = {10,0,0}
		},
		{	--19
			name = "water",
			drawGroup = ALPHA,
			physGroup = LIQUID,
			idle = 32,
			act = 33,
			icon = 0
		},
		{	--20
			name = "leaves_oak",
			drawGroup = SPEC,
			physGroup = SOLID,
			side = 26,
			emission = NON
		}
	}
}