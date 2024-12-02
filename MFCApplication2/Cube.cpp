#include "pch.h"
#include "Cube.h"

Vec3  Matrix::operator*(const Vec3& vec) const {
    float x = arr[0][0] * vec.x + arr[1][0] * vec.y + arr[2][0] * vec.z + arr[3][0];
    float y = arr[0][1] * vec.x + arr[1][1] * vec.y + arr[2][1] * vec.z + arr[3][1];
    float z = arr[0][2] * vec.x + arr[1][2] * vec.y + arr[2][2] * vec.z + arr[3][2];
    float w = arr[0][3] * vec.x + arr[1][3] * vec.y + arr[2][3] * vec.z + arr[3][3];
    return Vec3(x / w, y / w, z / w);
}



Matrix  Matrix::operator()(int excludeRow, int excludeCol) const {
    Matrix result(3, 3);
    int newRow = 0;
    for (int i = 0; i < 4; ++i) {
        if (i == excludeRow) continue;
        int newCol = 0;
        for (int j = 0; j < 4; ++j) {
            if (j == excludeCol) continue;
            result.set(newRow, newCol++, this->get(i, j));
        }
        newRow++;
    }
    return result;
}

Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                arr[i][j] = other.arr[i][j];
            }
        }
    }
    return *this;
}

Matrix& Matrix::rotateY(double degree) {
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
Matrix& Matrix::rotateZ(double degree) {
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

inline Matrix* Matrix::mul(const Matrix& m) const {
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
    memcpy(mm->arr, newMatrix, 16 * sizeof(double));
    return mm;
}

Matrix& Matrix::operator*(const Matrix& m) const {
    assert(this->col == m.row);
    return *this->mul(m);
}

// 计算行列式

inline double Matrix::determinant() const {
    return arr[0][0] * (arr[1][1] * arr[2][2] * arr[3][3] - arr[1][2] * arr[2][1] * arr[3][3] + arr[1][2] * arr[2][3] * arr[3][1] - arr[1][3] * arr[2][1] * arr[3][2] + arr[1][3] * arr[2][2] * arr[3][1])
        - arr[0][1] * (arr[1][0] * arr[2][2] * arr[3][3] - arr[1][2] * arr[2][0] * arr[3][3] + arr[1][2] * arr[2][3] * arr[3][0] - arr[1][3] * arr[2][0] * arr[3][2] + arr[1][3] * arr[2][2] * arr[3][0])
        + arr[0][2] * (arr[1][0] * arr[2][1] * arr[3][3] - arr[1][1] * arr[2][0] * arr[3][3] + arr[1][1] * arr[2][3] * arr[3][0] - arr[1][3] * arr[2][0] * arr[3][1] + arr[1][3] * arr[2][1] * arr[3][0])
        - arr[0][3] * (arr[1][0] * arr[2][1] * arr[3][2] - arr[1][1] * arr[2][0] * arr[3][2] + arr[1][1] * arr[2][2] * arr[3][0] - arr[1][2] * arr[2][0] * arr[3][1] + arr[1][2] * arr[2][1] * arr[3][0]);
}

// 获取子矩阵

inline Matrix Matrix::subMatrix(int excludeRow, int excludeCol) const {
    Matrix result(3, 3);
    int newRow = 0;
    for (int i = 0; i < 4; ++i) {
        if (i == excludeRow) continue;
        int newCol = 0;
        for (int j = 0; j < 4; ++j) {
            if (j == excludeCol) continue;
            result.set(newRow, newCol++, this->get(i, j));
        }
        newRow++;
    }
    return result;
}

// 计算代数余子式

inline double Matrix::cofactor(int row, int col) const {
    int sign = ((row + col) % 2 == 0) ? 1 : -1;
    Matrix subMatrix = subMatrix(row, col);
    return sign * subMatrix.determinant();
}

// 计算伴随矩阵

inline Matrix Matrix::adjugateMatrix() const {
    Matrix cofactorMatrix;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            cofactorMatrix.set(i, j, cofactor(i, j));
        }
    }
    return cofactorMatrix.transpose();
}

// 矩阵转置

inline Matrix Matrix::transpose() const {
    Matrix result(4, 4);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.set(i, j, this->get(j, i));
        }
    }
    return result;
}

// 求逆矩阵

inline Matrix Matrix::inverse() const {
    double det = determinant();
    if (std::abs(det) < 1e-9) {
        throw std::runtime_error("矩阵不可逆");
    }

    Matrix adjugate = adjugateMatrix();
    return adjugate * (1.0 / det);
}





void Line::render(MyDrawer* drawer, Matrix* model, Matrix* proj, Matrix* view) {
	Matrix* m1 = new Matrix(4, 4);
	m1->translate(start);
	Matrix* m2 = new Matrix(4, 4);
	m2->translate(end);

	Vec3 aclip = *m1 * *model * *proj * *view * start;
	Vec3 bclip = *m2 * *model * *proj * *view * end;

	drawer->drawLine_Mid(aclip.x, aclip.y, bclip.x, bclip.y);
}

void Cube::setYRot(float yaw) { this->model = Matrix().rotateY(yaw) * this->model; }

void Cube::setXRot(float pitch) { this->model = Matrix().rotateX(pitch) * this->model; }

void Cube::setZRot(float roll) { this->model = Matrix().rotateZ(roll) * this->model; }

void Cube::render(MyDrawer* drawer, Matrix* proj, Matrix* view) {
    Line lines[12];
    float s = size;
    lines[0].set(s, s, s, -s, s, s);
    lines[1].set(-s, s, s, -s, -s, s);
    lines[2].set(-s, -s, s, s, -s, s);
    lines[3].set(s, -s, s, s, s, s);

    lines[4].set(s, s, -s, -s, s, -s);
    lines[5].set(-s, s, -s, -s, -s, -s);
    lines[6].set(-s, -s, -s, s, -s, -s);
    lines[7].set(s, -s, -s, s, s, -s);

    lines[8].set(s, s, s, s, s, -s);
    lines[9].set(s, -s, s, s, -s, -s);
    lines[10].set(-s, -s, s, -s, -s, -s);
    lines[11].set(-s, s, s, -s, s, -s);

    for (int i = 0; i < 12; i++) {

        Matrix view1(4, 4);
        // 旋转顺序：先旋转pitch，再旋转yaw

        view1 = *view * view1.translate(position);
        lines[i].render(drawer, &model, proj, &view1);
    }
}

Matrix& Matrix::rotateX(double degree) {
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
