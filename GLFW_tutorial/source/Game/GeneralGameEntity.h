#ifndef GENERAL_GAME_ENTITY_H
#define GENERAL_GAME_ENTITY_H

///Voxel----------------------------------------------
/// <summary>
/// 
/// </summary>
struct Voxel {
	int id;
	Voxel(int id_) :id(id_) {}
	Voxel() {}
};
static bool operator==(const Voxel& left, const Voxel& right) {
	return (right.id == left.id);
}


#endif