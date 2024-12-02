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
	Vec3& add(float x, float y, float z) {this->x += x;this->y += y;this->z += z; return *this;}
	Vec3& sub(float x, float y, float z) {this->x -= x;this->y -= y;this->z -= z; return *this;}
	Vec3& scale(float s) {this->x *= s;this->y *= s;this->z *= s; return *this;}
};

class Matrix {
private:
	double arr[4][4]{ 0 };
public:
	int row;
	int col;
	double get(int row, int col) const {return arr[row][col];}
	double set(int row, int col, double val) { return arr[row][col] = val; }
	Matrix& translate(Vec3 v) { arr[3][0] += v.x;arr[3][1] += v.y;arr[3][2] += v.z;return *this;}
	Matrix& translate(double x, double y, double z) { arr[3][0] += x;arr[3][1] += y;arr[3][2] += z; return *this;}
	Matrix& scale(double x, double y, double z) { arr[0][0] *= x;arr[1][1] *= y;arr[2][2] *= z;return *this;}
	Matrix& scale(double x) {this->scale(x, x, x); return *this;}
	Matrix& rotateX(double degree);
	Matrix& rotateY(double degree);
	Matrix& rotateZ(double degree);
	Matrix* mul(const Matrix& m) const;
	Matrix& operator*(const Matrix& m) const;
	Vec3 operator*(const Vec3& vec) const;
	Matrix operator()(int excludeRow, int excludeCol) const;
	Matrix& operator=(const Matrix& other);
	double determinant() const;// 计算行列式
	Matrix subMatrix(int excludeRow, int excludeCol) const;// 获取子矩阵
	double cofactor(int row, int col) const;// 计算代数余子式
	Matrix adjugateMatrix() const;// 计算伴随矩阵
	Matrix transpose() const;// 矩阵转置
	Matrix inverse() const;// 求逆矩阵
	Matrix(int row=4,int col=4):row(row),col(col){arr[0][0] = arr[1][1] = arr[2][2] = arr[3][3] = 1;}
};



class Line {
public:
	Vec3 start;
	Vec3 end;
	Line(Vec3 s, Vec3 e) :start(s), end(e) {}
	Line() :start(Vec3()), end(Vec3()) {}
	void translate(float x, float y, float z) {start.add(x, y, z); end.add(x, y, z);}
	Line& set(float x1, float y1, float z1, float x2, float y2, float z2);
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
	AABB(float size);
	void scale(float s);
};

class Cube
{
private:

	Matrix model;
public:
	string name;
	Vec3 position;
	Vec3 anchor;
	AABB aabb;
	float size;

	Cube(string name,Vec3 pos, float size=1):size(size),aabb(size),position(pos),anchor(size, size, size),name(name) {}
	void translate(float x,float y,float z) {this->position.add(x, y, z);}
	void RotX(float pitch);
	void RotY(float yaw);
	void RotZ(float roll);
	void render(MyDrawer* drawer,  Matrix* proj, Matrix* view);
};

