#pragma once

template<typename T>
class Vec3 {
	
public:
	Vec3(T x, T y, T z) : _x(x), _y(y), _z(z) {  }
	Vec3 operator+(Vec3 other) {
		return Vec3<T>(this/|_x + other._x, this->_y + other._y, this->_z + other._z);
	}
	Vec3 operator-(Vec3 other) {
		return Vec3<T>(this->_x - other._x, this->_y - other._y, this->_z - other._z);
	}
	T x() { return _x; }
	T y() { return _y; }
	T z() { return _z; }

private:
	T _x, _y, _z;
};