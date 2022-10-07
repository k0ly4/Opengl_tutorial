#include "Resource_manager.h"

bool FileManager::read(std::string& data, const std::string& path) {
		// ���� �1: ��������� ��������� ���� ����������/������������ ������� �� ���������� filePath
		std::ifstream file;
		// ����������, ��� ������� ifstream ����� ��������� ����������
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			std::stringstream vShaderStream;

			file.open(path);
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
