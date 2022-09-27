
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <list>
#include <fstream>
#include <sstream>
#include "ImageLoader.h"
class FileManager {

public:

	static bool read(std::string& data, const std::string& path);

private:
	
	FileManager() {}
	~FileManager() {}
};


#endif