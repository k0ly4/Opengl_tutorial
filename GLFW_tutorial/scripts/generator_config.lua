
generator = {
	subHeightFactor = 10.0,
	multHeightFactor = 20.0,
	init = function()
		map.addChannelHeight(0.05,	1.0)
		map.addChannelHeight(0.1,	0.9)
		map.addChannelHeight(0.5,	0.8)
		map.addChannelHeight(1.0,	0.4)
		map.addChannelHeight(2.0,	0.2)
		map.addChannelHeight(3.0,	0.2)
	end,
	
	getBiom = function(noise,x,y,z)
		factor = noise:getNormalize(x,z,0.1)
	end,
	
	GetBlock = function(biom,x,y,z)
		
	end
}
-- script.global()
		-- .beginNamespace("vox")
		-- .addConstant("air", -1)
		-- .addConstant("turf", 0)
		-- .addConstant("earth", 1)
		-- .addConstant("stone", 2)
		-- .addConstant("bedrock", 6)
		-- .addConstant("oak", 8)
		-- .addConstant("oak_wood", 9)
		-- .addConstant("snow", 10)
		-- .addConstant("sand", 11)
		-- .endNamespace();

