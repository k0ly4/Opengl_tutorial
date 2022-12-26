
#ifndef IGEOMETRY_H
#define IGEOMETRY_H
#include <vector>
#include "Math/Math.h"
#include "Resource/Buffer.h"
#include "Graphic/ContextRender.h"
///iGeometry---------------------------------------------
/// <summary>
/// 
/// </summary>
template<typename T>
class iGeometry {
public:

	iGeometry() :size_vertex_to_draw(0) {
		VBO.begin();
		VAO.begin();
		T::attrib(VAO);
		VAO.end();
		VBO.end();
	}

	inline void saveInBuffer() {
		VAO.begin();
		VBO.begin();
		VBO.data(vertices);
		EBO.begin();
		EBO.data(indices);
		T::attrib(VAO);
		VAO.end();
		EBO.end();
		size_vertex_to_draw = indices.size();
	}

	inline void draw() {
		VAO.begin();
		glDrawElements(GlRender::TRIANGLES, size_vertex_to_draw, GL_UNSIGNED_INT, 0);
	}
	inline void draw(GLenum primitive) {
		VAO.begin();
		glDrawElements(primitive, size_vertex_to_draw, GL_UNSIGNED_INT, 0);
	}

	inline void pushIndices(size_t begin) {
		indices.push_back(begin);
		indices.push_back(begin + 1);
		indices.push_back(begin + 2);
		indices.push_back(begin);
		indices.push_back(begin + 2);
		indices.push_back(begin + 3);
	}

	inline void push_back(const T& vertex) {
		vertices.push_back(vertex);
	}

	ArrayBufferObject VAO;
	VertexBufferObject VBO;
	ElementBufferObject EBO;
	size_t size_vertex_to_draw;

	std::vector<unsigned int> indices;
	std::vector<T> vertices;
private:
};
#endif