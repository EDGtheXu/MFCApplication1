#pragma once
#include<iostream>
using namespace std;

class Vec3 {
public:
	float x;
	float y;
	float z;
	Vec3(Vec3* v) { this->x = v->x;this->y = v->y, this->z = v->z; }
	Vec3(float x, float y, float z) :x(x), y(y), z(z) {}
	Vec3& add(float x, float y, float z) {
		this->x += x;this->y += y;this->z += z;
		return *this;
	}
	Vec3& sub(float x, float y, float z) {
		this->x -= x;this->y -= y;this->z -= z;
		return *this;
	}
	Vec3& scale(float s) {
		this->x *= s;this->y *= s;this->z *= s;
		return *this;
	}
};



class AABB {
public:
	float minx;
	float miny;
	float minz;
	float maxx;
	float maxy;
	float maxz;
	float _size = 1;
	AABB(float size = 1) {
		_size = size;
		minx = miny = minz = -size;
		maxx = maxy = maxz = size;
	}
	void scale(float s) {
		float t = _size * s;
		minx = miny = minz = -t;
		maxx = maxy = maxz = t;
	}
};

class Cube
{
private:
	double yaw;
	double pitch;

public:
	string name;
	Vec3 position;
	Vec3 anchor;
	AABB aabb;
	float size;


	Cube(string name,Vec3 pos, float size=1):size(size),aabb(size),position(pos),anchor(size, size, size),name(name) {
		
	}

	void translate(float x,float y,float z) {this->position.add(x, y, z);}
	void setYRot(float yaw) { this->yaw = yaw; }
	void setXRot(float pitch) { this->pitch = pitch; }

	void render() {

	}
	
};

