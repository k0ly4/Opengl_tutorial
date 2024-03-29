#ifndef ARRAYS_H
#define ARRAYS_H

#include "Game/Voxels/Declare.h"
//vector2d--------------------------------------------------------
template<typename T>
class vector2d {
public:
	vector2d() {}
	vector2d(const glm::ivec2& size):
		size_(size),
		buffer(size.x*size.y)
	{}

	inline void setSize(const glm::ivec2& size) {
		size_ = size;
		buffer.resize(size_.x * size_.y);
	}
	inline T& operator ()(size_t x, size_t y) {			return buffer[ind(x, y)]; }
	inline T& operator ()(const glm::uvec2& coord) {	return buffer[ind(coord)]; }
	inline T& operator[](size_t index) {				return buffer[index]; }
	inline size_t size() {								return buffer.size(); }
	inline glm::ivec2 v_size() {						return size_; }
	// �������� ������������ ������
	inline bool is(size_t x, size_t y)const {			return x < size_.x && y < size_.y; }
	inline bool is(const glm::uvec2& coord)const {		return coord.x < size_.x && coord.y < size_.y; }
	inline void setNull() { for (size_t i = 0; i < buffer.size(); i++) buffer[i] = 0; }
	// ���� ����� �������� �� ���������� 0
	inline const T& get(size_t x,size_t y, const T& def)const	noexcept {  return is(x,y) ? (buffer[ind(x, y)]) : def; }
	inline  T* get(const glm::uvec2& coord)						noexcept {	return is(coord) ? &(buffer[ind(coord)]) : 0; }
	inline const T* get(const glm::uvec2& coord) const			noexcept {	return is(coord) ? &(buffer[ind(coord)]) : 0; }
	//������
	inline size_t ind(size_t x, size_t y)		const noexcept { return (y * size_.x + x); }
	inline size_t ind(const glm::uvec2& coord)	const noexcept { return (coord.y * size_.x + coord.x); }
	inline auto data()const noexcept							{ return buffer.data(); }

private:

	glm::ivec2 size_;
	std::vector<T> buffer;
};

//Array2d--------------------------------------------------------
template<size_t size_, typename T>
class Array2d {
public:
	inline T& operator ()(size_t x, size_t y)noexcept {			return buffer[ind(x, y)]; }
	inline T& operator ()(const glm::uvec2& coord)noexcept {	return buffer[ind(coord)]; }
	inline T& operator[](size_t index)noexcept {				return buffer[index]; }
	inline size_t size() {										return size_ * size_; }
	// �������� ������������ ������
	inline bool is(size_t index) { return index < size(); }
	inline bool is(size_t x, size_t y) {						return x < size_&& y < size_; }
	inline bool is(const glm::uvec2& coord) {					return coord.x < size_ && coord.y < size_; }
	inline void setNull() {										for (size_t i = 0; i < size_ * size_; i++)	buffer[i] = 0; }
	inline void fill(const T& value) {							for (size_t i = 0; i < size_ * size_; i++)	buffer[i] = value; }
	// ���� ����� �������� �� ���������� 0
	inline  T* get(const glm::uvec2& coord) noexcept {			return is(coord) ? &(buffer[ind(coord)]) : 0; }
	inline const T* get(const glm::uvec2& coord)const noexcept{	return is(coord) ? &(buffer[ind(coord)]) : 0; }
	//���� ������������ T
	inline void log() {
		for (size_t y = 0; y < size_; y++) {
			for (size_t x = 0; x < size_; x++) ::LOG("%d", buffer[ind(x,y)]);
			::LOG("\n");
		}
	}
private:
	//������
	inline size_t ind(size_t x, size_t y)		const noexcept { return (y * size_ + x); }
	inline size_t ind(const glm::uvec2& coord)	const noexcept { return (coord.y * size_ + coord.x); }

	T buffer[size_ * size_];
};

//Array3d--------------------------------------------------------
template<size_t x_, size_t y_, size_t z_, typename T>
class Array3d {
public:

	inline T& operator ()(size_t x, size_t y, size_t z) noexcept { return buffer[ind(x, y, z)]; }
	inline const T& operator ()(size_t x, size_t y, size_t z)const noexcept { return buffer[ind(x, y, z)]; }
	inline T& operator ()(const glm::uvec3& coord) noexcept { return buffer[ind(coord)]; }
	inline const T& operator ()(const glm::uvec3& coord) const noexcept { return buffer[ind(coord)]; }
	inline T& operator[](size_t index)noexcept { return buffer[index]; }
	inline const T& operator[](size_t index)const noexcept { return buffer[index]; }
	//�������
	inline size_t size()const noexcept { return x_ * y_ * z_; }
	inline size_t sX()	const noexcept { return x_; }
	inline size_t sY()	const noexcept { return y_; }
	inline size_t sZ()	const noexcept { return z_; }
	// �������� ������������ ������
	inline bool is(size_t x, size_t y, size_t z)const noexcept { return x < x_&& y < y_&& z < z_; }
	inline bool is(const glm::uvec3& coord)const noexcept { return coord.x < x_&& coord.y < y_&& coord.z < z_; }
	inline void setNull() noexcept { for (size_t i = 0; i < size(); i++) buffer[i] = 0; }

	// ���� ����� �������� �� ���������� 0
	inline  T* get(const glm::uvec3& coord) noexcept { return is(coord) ? &(buffer[ind(coord)]) : 0; }
	inline const T* get(const glm::uvec3& coord) const noexcept { return is(coord) ? &(buffer[ind(coord)]) : 0; }

	//�������� �����
	inline const T& clip(const glm::uvec3& coord) const noexcept { return buffer[ind(::clip(coord.x, x_), ::clip(coord.y, y_), ::clip(coord.z, z_))]; }
	inline const T& clip(size_t x, size_t y, size_t z) const noexcept { return buffer[ind(::clip(x, x_), ::clip(y, y_), ::clip(z, z_))]; }
	inline glm::uvec3 coord(size_t i)const noexcept {return coord3(i);}

	inline size_t ind(size_t x, size_t y, size_t z)	const noexcept { return ((y * z_ + z) * x_ + x); }
	inline size_t ind(const glm::uvec3& coord)		const noexcept { return ((coord.y * z_ + coord.z) * x_ + coord.x); }

private:
	inline glm::uvec2 coord2(size_t i)const noexcept { return glm::uvec2(i % x_, i / z_); }
	inline glm::uvec3 coord3(size_t i)const noexcept {
		size_t xzVol = x_* z_;
		glm::uvec2 xz(coord2(i % xzVol));
		return glm::uvec3(xz.x, i / xzVol, xz.y);
	}
	

	T buffer[x_ * y_ * z_];
};
#endif