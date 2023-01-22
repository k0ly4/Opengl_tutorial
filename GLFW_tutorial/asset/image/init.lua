


asset = {
	directory = "textures\\",
	size_px =  32,
	texture_block = 
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
		{path = "log_oak.png",					color = 255				},	--11
		{path = "log_oak_top.png",				color = 255				},	--12
		{path = "double_plant_grass_top.png",	color = {0,255,0}		},	--13
		{path = "beacon.png",					color = 255				},	--14
		{path = "redstone_block.png",			color = 255				},	--15
		{path = "water_still.png",				frame  = 32				},	--16
		{path = "water_flow.png",				frame  = 32				}	--17
	}
}

NON 	= 0
NORM 	= 1
SPEC    = 2
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
			side ={11,11,12,12,11,11},
			emission = NON
		},
		{	--11
			name = "double_plant_grass",
			drawGroup = SPEC,
			physGroup = GAS,
			side = 13,
			emission = NON
		},
		{	--12
			name = "beacon",
			drawGroup = NORM,
			physGroup = SOLID,
			side =14,
			emission = {2,2,10}
		},
		{	--13
			name = "redstone", 
			drawGroup = NORM,
			physGroup = SOLID,
			side = 15,
			emission = {10,0,0}
		},
		{	--14
			name = "water",
			drawGroup = SPEC,
			physGroup = LIQUID,
			idle = 16,
			act = 17
		}
	}
}