#include "Resource_manager.h"

bool FileManager::read(std::string& data, const std::string& path) {
		// Этап №1: Получение исходного кода вершинного/фрагментного шейдера из переменной filePath
		std::ifstream file;
		// Убеждаемся, что объекты ifstream могут выбросить исключение
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
