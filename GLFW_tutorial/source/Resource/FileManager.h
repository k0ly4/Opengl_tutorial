
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

////Writer--------------------------
class Writer {

public:
	
	inline bool open(const std::string& path) {
		out.open(path, std::ios::binary | std::ios::out);
		return out.is_open();
	}

	template<typename T>
	static inline void write(std::ofstream& out, T data) {
		out.write((char*)&data, sizeof(data));
	}

	template<typename T>
	inline void write(T data) {
		out.write((char*)&data, sizeof(data));
	}
	
	inline void close() {
		out.close();
	}
	virtual ~Writer() {
		out.close();
	}

protected:

	std::ofstream out;
};

////Reader--------------------------
class Reader {

public:

	inline bool open(const std::string& path) {
		in.open(path, std::ios::binary | std::ios::in);
		return in.is_open();
	}
	template<typename T>
	static inline void read(std::ifstream& in, T& data) {
		in.read((char*)&data, sizeof(data));
	}

	template<typename T>
	inline void read(T& data) {
		read(in,data);
	}
	
	inline void set(size_t pos) {
		in.seekg(pos);
	}

	inline void move(size_t offset) {
		in.seekg(offset, std::ios::cur);
	}

	inline void close() {
		in.close();
	}
	virtual ~Reader() {
		in.close();
	}

protected:

	std::ifstream in;
};

//FileManager-------------------------------
/// <summary>
/// 
/// </summary>
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