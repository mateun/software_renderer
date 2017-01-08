#pragma once
#include <vector>

template<typename T>
class Vec3 {
	
public:
	Vec3() : _x(0), _y(0), _z(0) {}
	Vec3(T x, T y, T z) : _x(x), _y(y), _z(z) {  }
	Vec3 operator+(Vec3 other) {
		return Vec3<T>(this->_x + other._x, this->_y + other._y, this->_z + other._z);
	}
	Vec3 operator-(Vec3 other) {
		return Vec3<T>(this->_x - other._x, this->_y - other._y, this->_z - other._z);
	}

	Vec3 translate(T x, T y, T z) {
		return Vec3<T>(this->_x + x, this->_y + y, this->_z + z);
	}

	Vec3 scale(T x, T y, T z) {
		return Vec3<T>(this->_x * x, this->_y * y, this->_z * z);
	}

	void setX(T val) { _x = val; }
	void setY(T val) { _y = val; }
	void setZ(T val) { _z = val; }

	T x() { return _x; }
	T y() { return _y; }
	T z() { return _z; }

private:
	T _x, _y, _z;
};

template<typename T>
struct Triangle {
public: 
	Triangle() {
		vecs[0] = Vec3<T>(0, 0, 0);
		vecs[1] = Vec3<T>(0, 0, 0);
		vecs[2] = Vec3<T>(0, 0, 0);
	}
	Triangle(Vec3<T> v1, Vec3<T> v2, Vec3<T> v3) {
		vecs[0] = v1;
		vecs[1] = v2;
		vecs[2] = v3;
	}
	Vec3<T> vecs[3];
};

template<typename T>
class Mesh {

public:
	Mesh(std::vector<Triangle<T>> tris);

private:
	std::vector<Triangle<T>> _tris;
};