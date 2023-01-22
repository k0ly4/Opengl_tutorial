#ifndef GENERAL_GAME_ENTITY_H
#define GENERAL_GAME_ENTITY_H

///extendedID----------------------------------------------
/// <summary>
/// 
/// </summary>
struct extendedID {

	twobyte id;
	byte m1;
	byte m2;

	extendedID() {}
	extendedID(twobyte id_, byte m1_, byte m2_) :
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

		int id_;
		extendedID e;

	};
	
	//Voxel(int id_) :id(id_) {}
	Voxel(twobyte id,byte m1=0,byte m2=0):e(id,m1,m2)
	{}
	Voxel():e(0,0,0) {}
};

static bool operator==(const Voxel& left, const Voxel& right) {return (right.id_== left.id_);}
static bool operator!=(const Voxel& left, const Voxel& right) { return (right.id_ != left.id_); }
namespace vox {

	enum TypeVoxel :short
	{
		air = 0,
		turf = 1,
		earth = 2,
		sand = 3,
		snow = 4,
		stone = 5,
		glass = 6,
		glowstone = 7,
		bedrock = 8,
		lamp = 9,
		oak = 10,
		grass = 11,
		beacon = 12,
		redstone = 13,
		water = 14,
		size,
	};
}
class Biom {
public:
	enum Type:size_t
	{
		plain,
		tundra,
		mountains,
		desert,
	};
private:

};

#endif
