
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
		size_vertex_to_draw = indices.size();
		T::attrib(VAO);
		VAO.end();
		EBO.end();
		
	}

	inline void draw() {
		VAO.begin();
		glDrawElements(Render::TRIANGLES, size_vertex_to_draw, GL_UNSIGNED_INT, 0);
	}
	inline void draw(GLenum primitive) {
		VAO.begin();
		glDrawElements(primitive, size_vertex_to_draw, GL_UNSIGNED_INT, 0);
	}
	inline void draw(GLenum primitive,size_t size) {
		VAO.begin();
		glDrawElements(primitive, size, GL_UNSIGNED_INT, 0);
	}

	inline void pushInd(size_t ind)			noexcept {	indices.push_back(ind);}
	inline void pushVert(const T& vertex)	noexcept {	vertices.push_back(vertex); }
	inline void push_back(const T& vertex)	noexcept {	vertices.push_back(vertex);}
	inline void pushSquareInd()			noexcept {
		size_t begin = vertices.size();
		pushInd(begin);
		pushInd(begin + 1);
		pushInd(begin + 2);
		pushInd(begin);
		pushInd(begin + 2);
		pushInd(begin + 3);
	}
	inline void pushSquareInd(size_t begin)	noexcept {
		pushInd(begin);
		pushInd(begin + 1);
		pushInd(begin + 2);
		pushInd(begin);
		pushInd(begin + 2);
		pushInd(begin + 3);
	}
	inline void pushIndices(size_t begin)	noexcept {
		pushInd(begin);
		pushInd(begin + 1);
		pushInd(begin + 2);
		pushInd(begin);
		pushInd(begin + 2);
		pushInd(begin + 3);
	}

	inline void clear() noexcept { vertices.clear(); indices.clear(); }

	ArrayBufferObject VAO;
	VertexBufferObject VBO;
	ElementBufferObject EBO;
	size_t size_vertex_to_draw;

	std::vector<unsigned int> indices;
	std::vector<T> vertices;
private:
};
///iGeometry---------------------------------------------
/// <summary>
/// 
/// </summary>
template<typename T>
class qGeometry {
public:

	qGeometry() {
		VBO.begin();
		VAO.begin();
		T::attrib(VAO);
		VAO.end();
		VBO.end();
	}

	inline void saveInBuffer() {
		VBO.begin();
		VBO.data(vertices);
	}

	inline void drawArrays(GLenum primitive = Render::TRIANGLES) {
		VAO.begin();
		glDrawArrays(primitive, 0, vertices.size());
	}
	inline void drawArrayInstanced(size_t count_obj) {
		VAO.begin();
		glDrawArraysInstanced(Render::TRIANGLES, 0, vertices.size(), count_obj);
	}
	inline void drawArrayInstanced(GLenum primitive, size_t count_obj) {
		VAO.begin();
		glDrawArraysInstanced(primitive, 0, vertices.size(), count_obj);
	}

	inline void push_back(const T& vertex) { vertices.push_back(vertex); }
	inline void clear(){vertices.clear();}
	ArrayBufferObject VAO;
	VertexBufferObject VBO;
	std::vector<T> vertices;
	inline T& operator [](size_t index) noexcept { return vertices[index]; }
private:
};
#endif