#include "FileManager.h"



void Reader::read(std::vector<Voxel>& data) {
	int size;
	read(size);
	data.resize(size);
	for (size_t i = 0; i < size; i++) {
		read(data[i].id);
	}
}

void Writer::write(const std::vector<Voxel>& data) {
	write(data.size());
	for (size_t i = 0; i < data.size(); i++) { 
		write(data[i].id);
	}
}



bool FileManager::read(std::string& data, const std::string& path) {
		// ���� �1: ��������� ��������� ���� ����������/������������ ������� �� ���������� filePath
		std::ifstream file(path);
		// ����������, ��� ������� ifstream ����� ��������� ����������
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
	std::ifstream in(path, std::ios::binary| std::ios::in); //��������� ���� � �������� ������ ������ ��� ������
	if (in.is_open()==0) {
		return 0;
	}
	int size; 
	Reader::read(in, size);
	data.resize(size);
	for (size_t i = 0; i < size; i++) {
		Reader::read(in, data[i].id);
	}
	in.close(); //��������� ����
	return 1;
}

bool FileManager::write(const std::string& path, const std::vector<Voxel>& data) {
	std::ofstream out(path, std::ios::binary | std::ios::out); //��������� ���� � �������� ������ ��� ������
	if (out.is_open() == 0) {
		return 0;
	}
	Writer::write(out, data.size());
	for (size_t i = 0; i < data.size(); i++) {
		Writer::write(out, data[i].id);
	}
	out.close(); //��������� ����
	return 1;
}

bool FileManager::read(const std::string& path, JSON& json) {
	// ���� �1: ��������� ��������� ���� ����������/������������ ������� �� ���������� filePath
	std::ifstream file(path);
	// ����������, ��� ������� ifstream ����� ��������� ����������
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