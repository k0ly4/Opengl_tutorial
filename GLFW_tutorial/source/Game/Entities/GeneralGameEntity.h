#ifndef GENERAL_GAME_ENTITY_H
#define GENERAL_GAME_ENTITY_H

///extendedID----------------------------------------------
/// <summary>
/// 
/// </summary>
struct extendedID {
	short id;
	byte m1;
	byte m2;

	extendedID() {}
	extendedID(short id_, byte m1_, byte m2_) :
		id(id_),
		m1(m1_),
		m2(m2_) {}
};

///Voxel----------------------------------------------
/// <summary>
/// 
/// </summary>
struct Voxel {

	

	union
	{

		int id;
		extendedID e;

	};
	
	Voxel(int id_) :id(id_) {}
	Voxel(short id,byte m1,byte m2):e(id,m1,m2)
	{}
	Voxel() {}
};

static bool operator==(const Voxel& left, const Voxel& right) {
	return (right.id == left.id);
}


namespace vox {
	enum Type:int
	{
		air = -1,
		turf = 0,
		earth = 1,
		stone = 2
	};
}


#endif
