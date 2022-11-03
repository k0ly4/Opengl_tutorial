
#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <list>
#include <fstream>
#include <sstream>
#include "Resource/ImageLoader.h"
#include "Game/GeneralGameEntity.h"
#include "nlohmann/json.hpp"

////FileManager--------------------------

typedef nlohmann::json JSON;

class FileManager {

public:
	static bool read(std::string& data, const std::string& path);
	static bool read(const std::string& path, std::vector<Voxel>& data);
	static bool write(const std::string& path, const std::vector<Voxel>& data);

	static bool read(const std::string& path, JSON& json);
	static bool write(const std::string& path, const JSON& json);
private:

	FileManager() {}
	~FileManager() {}
};


#endif