
#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <list>
#include <fstream>
#include <sstream>
#include "Resource/ImageLoader.h"
#include "Game/Entities/GeneralGameEntity.h"
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
	static inline void write(std::ofstream& out, const T& data) {
		out.write((char*)&data, sizeof(data));
	}

	template<typename T>
	inline void write(const T& data) {
		out.write((char*)&data, sizeof(data));
	}

	template<typename T>
	inline void write(const T& data,size_t size) {
		out.write((char*)&data, size);
	}

	template<typename T>
	inline void write(twoint pos,const T& data) {
		set(pos);
		out.write((char*)&data, sizeof(data));
	}

	inline std::streampos getPos() { return out.tellp(); }

	inline void set(twoint pos) { out.seekp(pos); }

	inline void moveFromCur(twoint offset) { out.seekp(offset, std::ios::cur); }

	inline void moveFromEnd(twoint offset) { out.seekp(offset, std::ios::end); }

	inline void moveFromBeg(twoint offset) { out.seekp(offset, std::ios::beg); }

	inline void close() { out.close(); }

	virtual ~Writer() { out.close(); }

protected:

	std::ofstream out;
};

////ReaderStatic--------------------------
class Reader {
public:
	template<typename T>
	static inline void read(std::ifstream& in, T& data) {
		in.read((char*)&data, sizeof(data));
	}
	template<typename T>
	static inline void read(std::ifstream& in, T& data, size_t size) {
		in.read((char*)&data, size);
	}
private:

	Reader() {}
	~Reader() {}

};

////Reader--------------------------
class ReaderFile {

public:
	ReaderFile() {}
	ReaderFile(const std::string& path) { in.open(path, std::ios::binary | std::ios::in); }

	inline bool open(const std::string& path) {
		in.open(path, std::ios::binary | std::ios::in);
		return in.is_open();
	}

	bool isOpen() { return in.is_open(); }
	operator bool() { return in.is_open();}

	template<typename T>
	inline void read(T& data,size_t size) {
		Reader::read(in, data, size);
	}

	template<typename T>
	inline void read(T& data) {
		Reader::read(in,data);
	}

	template<typename T>
	inline void readFromPos(twoint pos, T& data,size_t size) {
		set(pos);
		read(data, size);
	}
	template<typename T> 
	inline void readFromPos(twoint pos, T& data) {
		set(pos);
		read(data);
	}

	inline std::streampos getPos(){ return in.tellg(); }

	inline void set(twoint pos) { in.seekg(pos);}

	inline void move(twoint offset) { in.seekg(offset, std::ios::cur);}

	inline void close() { in.close(); }

	virtual ~ReaderFile() { in.close(); }

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