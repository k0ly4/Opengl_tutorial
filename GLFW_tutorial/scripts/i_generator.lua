
generator_config = {
	
	heights_map = {
		subFactor = 10.0,
		scaleFactor = 20.0,
		noiseType = NoiseType.OpenSimplex2,
		--temperature = [0,255]
		--humidity 	 = [0,255]
		init = function()
			map.addChannelHeight(0.05,	1.0)
			map.addChannelHeight(0.1,	0.9)
			map.addChannelHeight(0.9,	0.8)
			map.addChannelHeight(3.0,	0.4)
			--map.addChannelHeight(3.0,	0.2)
			--map.addChannelHeight(3.0,	0.2)
		end,
	},
	
	biom_map = {
		
		scaleTemperature 	= 1.0,
		scaleHumidity 		= 1.0,
		noiseType = NoiseType.Perlin,
		
		bioms = {
			getPlane= function(biom)
				biom.r_humidity = Range(100,200)
				biom.r_tmpr = Range(81,169)
				biom.ground = vox.turf
				biom.underground = vox.earth
			end,
			
			getDesert= function(biom)
				biom.r_humidity = Range(0,80)
				biom.r_tmpr =Range(170,255)
				biom.ground = 		vox.sand
				biom.underground = 	vox.sand
			end,
			
			getTundra= function(biom)
				biom.r_humidity = Range(150,255)
				biom.r_tmpr =Range(0,80)
				biom.ground = 		vox.snow
				biom.underground = 	vox.earth
			end
		}
	}
}

