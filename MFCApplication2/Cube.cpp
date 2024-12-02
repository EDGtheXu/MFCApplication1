#include "pch.h"
#include "Cube.h"

void Line::render(MyDrawer* drawer, Matrix* model, Matrix* proj, Matrix* view) {
	Matrix* m1 = new Matrix(4, 4);
	m1->translate(start);
	Matrix* m2 = new Matrix(4, 4);
	m2->translate(end);

	Vec3 aclip = *m1 * *model * *proj * *view * start;
	Vec3 bclip = *m2 * *model * *proj * *view * end;

	drawer->drawLine_Mid(aclip.x, aclip.y, bclip.x, bclip.y);
}

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