#pragma once
#include<iostream>
#include"MyDrawer.h"
using namespace std;


class Vec3 {
public:
	float x;
	float y;
	float z;

	Vec3(Vec3* v) { this->x = v->x;this->y = v->y, this->z = v->z; }
	Vec3(float x, float y, float z) :x(x), y(y), z(z) {}
	Vec3() :x(0), y(0), z(0) {}
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

class Matrix {
private:
	double arr[4][4]{ 0 };
public:
	int row;
	int col;
	double get(int row, int col) const {return arr[row][col];}
	double set(int row, int col, double val) { return arr[row][col] = val; }
	Matrix& translate(Vec3 v) {
		arr[3][0] += v.x;arr[3][1] += v.y;arr[3][2] += v.z;
		return *this;
	}
	Matrix& translate(double x, double y, double z) {
		arr[3][0] += x;arr[3][1] += y;arr[3][2] += z;
		return *this;
	}
	Matrix& scale(double x, double y, double z) {
		arr[0][0] *= x;arr[1][1] *= y;arr[2][2] *= z;
		return *this;
	}
	Matrix& scale(double x) {
		this->scale(x, x, x);
		return *this;
	}
	Matrix& rotateX(double degree) {
		Matrix* m = new Matrix(4, 4);
		m->set(1, 1, cos(degree * 3.1415926 / 180));
		m->set(1, 2, sin(degree * 3.1415926 / 180));
		m->set(2, 1, -sin(degree * 3.1415926 / 180));
		m->set(2, 2, cos(degree * 3.1415926 / 180));
		Matrix* nm = this->mul(*m);
		memcpy(arr, nm->arr, 16 * sizeof(double));
		delete nm;
		delete m;
		return *this;
	}
	Matrix& rotateY(double degree) {
		Matrix* m = new Matrix(4, 4);
		m->set(0, 0, cos(degree * 3.1415926 / 180));
		m->set(0, 2, -sin(degree * 3.1415926 / 180));
		m->set(2, 0, sin(degree * 3.1415926 / 180));
		m->set(2, 2, cos(degree * 3.1415926 / 180));
		Matrix* nm = this->mul(*m);
		memcpy(arr, nm->arr, 16 * sizeof(double));
		delete nm;
		delete m;
		return *this;
	}
	Matrix& rotateZ(double degree) {
		Matrix* m = new Matrix(4, 4);
		m->set(0, 0, cos(degree * 3.1415926 / 180));
		m->set(0, 1, sin(degree * 3.1415926 / 180));
		m->set(1, 0, -sin(degree * 3.1415926 / 180));
		m->set(1, 1, cos(degree * 3.1415926 / 180));
		Matrix* nm = this->mul(*m);
		memcpy(arr, nm->arr, 16 * sizeof(double));
		delete nm;
		delete m;
		return *this;
	}

	Matrix& operator*(const Matrix& m) const {
		assert(this->col == m.row);
		return *(this->mul(m));
	}


	Vec3 operator*(const Vec3& vec) const {
		float x = arr[0][0] * vec.x + arr[1][0] * vec.y + arr[2][0] * vec.z + arr[3][0];
		float y = arr[0][1] * vec.x + arr[1][1] * vec.y + arr[2][1] * vec.z + arr[3][1];
		float z = arr[0][2] * vec.x + arr[1][2] * vec.y + arr[2][2] * vec.z + arr[3][2];
		float w = arr[0][3] * vec.x + arr[1][3] * vec.y + arr[2][3] * vec.z + arr[3][3];
		return Vec3(x / w, y / w, z / w);
	}

	Matrix* mul(const Matrix& m) const {
		assert(this->col == m.row);
		double newMatrix[4][4]{ 0 };

		for (int i = 0; i < this->row;i++) {
			for (int j = 0;j < m.col;j++) {
				for (int k = 0;k < this->col;k++) {
					newMatrix[i][j] += this->get(i, k) * m.get(k, j);
				}
			}
		}
		Matrix* mm = new Matrix(this->col, m.row);
		memcpy( mm->arr, newMatrix,16 * sizeof(double));
		return mm;
	}

	Matrix(int row,int col):row(row),col(col){
		arr[0][0] = arr[1][1] = arr[2][2] = arr[3][3] = 1;
	}
};



class Line {
public:
	Vec3 start;
	Vec3 end;
	Line(Vec3 s, Vec3 e) :start(s), end(e) {}
	Line() :start(Vec3()), end(Vec3()) {}
	void translate(float x, float y, float z) {
		start.add(x, y, z);
		end.add(x, y, z);
	}
	Line& set(float x1, float y1, float z1, float x2, float y2, float z2) {
		start.x = x1;start.y = y1;start.z = z1;
		end.x = x2;end.y = y2;end.z = z2;
		return *this;
	}
	void render(MyDrawer* drawer, Matrix* model, Matrix* proj, Matrix* view);
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
		yaw = 0;
		pitch = 0;
	}

	double getYRot() { return yaw; }
	double getXRot() { return pitch; }

	void translate(float x,float y,float z) {this->position.add(x, y, z);}
	void setYRot(float yaw) { this->yaw = yaw; }
	void setXRot(float pitch) { this->pitch = pitch; }
	void render(MyDrawer* drawer,  Matrix* proj, Matrix* view);
};

