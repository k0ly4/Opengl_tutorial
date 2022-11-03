#include "FileManager.h"

template<typename T>
inline void read(std::ifstream& in,T& data) {
	in.read((char*)&data, sizeof(data));
}

template<typename T>
inline void write(std::ofstream& out, T data) {
	out.write((char*)&data, sizeof(data));
}

bool FileManager::read(std::string& data, const std::string& path) {
		// Этап №1: Получение исходного кода вершинного/фрагментного шейдера из переменной filePath
		std::ifstream file(path);
		// Убеждаемся, что объекты ifstream могут выбросить исключение
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			std::stringstream vShaderStream;
			vShaderStream << file.rdbuf();
			data = vShaderStream.str();
			file.close();
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "(!)ERROR::FILE_NOT_SUCCESFULLY_READ::" << path << std::endl;
			return 0;
		}
		return 1;
	}

bool FileManager::read(const std::string& path, std::vector<Voxel>& data) {
	std::ifstream in(path, std::ios::binary| std::ios::in); //Открываем файл в двоичном режиме только для чтения
	if (in.is_open()==0) {
		return 0;
	}
	int size; 
	::read(in, size);
	data.resize(size);
	for (size_t i = 0; i < size; i++) {
		::read(in, data[i].id);
	}
	in.close(); //Закрываем файл
	return 1;
}

bool FileManager::write(const std::string& path, const std::vector<Voxel>& data) {
	std::ofstream out(path, std::ios::binary | std::ios::out); //Открываем файл в двоичном режиме для записи
	if (out.is_open() == 0) {
		return 0;
	}
	::write(out, data.size());
	for (size_t i = 0; i < data.size(); i++) {
		::write(out, data[i].id);
	}
	out.close(); //Закрываем файл
	return 1;
}

bool FileManager::read(const std::string& path, JSON& json) {
	// Этап №1: Получение исходного кода вершинного/фрагментного шейдера из переменной filePath
	std::ifstream file(path);
	// Убеждаемся, что объекты ifstream могут выбросить исключение
	if (file.is_open() == 0) {
		LOG("FILE_NOT_SUCCESFULLY_READ::%s\n", path.c_str());
		return 0;
	}
	json = JSON::parse(file);
	file.close();
	return 1;
}

bool FileManager::write(const std::string& path, const JSON& json) {
	return 1;
}