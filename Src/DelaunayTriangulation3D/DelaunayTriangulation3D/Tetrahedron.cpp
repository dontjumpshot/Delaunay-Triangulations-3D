
#include "stdafx.h"
#include "Tetrahedron.h"
#include "CircumCenter.h"

using namespace delaunay3d;

cTetrahedron::cTetrahedron()
{
}

cTetrahedron::cTetrahedron(const Vector3 &p1,
	const Vector3 &p2,
	const Vector3 &p3,
	const Vector3 &p4)
{
	Create(p1, p2, p3, p4);
}

/*首先计算给定点 p 到四面体外接球圆心 m_circumCenter 的距离，使用向量减法和取长度操作得到这个距离值。
判断这个距离值是否小于四面体的外接球半径 m_radius，如果是，则说明点 p 在四面体内部，返回 true；否则说明点 p 不在四面体内部，返回 false*/
bool cTetrahedron::IsContain(const Vector3 &p)
{
	const float len = (m_circumCenter - p).Length();
	return m_radius > len;
}

/*判断给定的点 p 是否等于四面体的任何一个顶点，即检查 p 是否与 m_p[0]、m_p[1]、m_p[2] 或 m_p[3] 中的任意一个相等。
如果 p 等于四面体的任何一个顶点，返回 true；否则返回 false。*/
bool cTetrahedron::IsContainVertex(const Vector3 &p)
{
	return (m_p[0] == p) || (m_p[1] == p) || (m_p[2] == p) || (m_p[3] == p);
}

/*首先计算四个顶点的平均位置得到 center，作为四面体的中心点。

分别创建四个三角形 m_tr[0]、m_tr[1]、m_tr[2] 和 m_tr[3]，其中每个三角形都是由三个顶点和一个“前端”点构成。这里的“前端”点是根据当前三个顶点和中心点计算得出的。

将四个顶点分别赋值给 m_p[0]、m_p[1]、m_p[2] 和 m_p[3]。

调用 TetCircumCenter 函数计算四面体的外接球圆心 m_circumCenter 和一个额外的向量 extra。

计算外接球的半径，即将 m_circumCenter 和第一个顶点 p1 的距离，并存储在 m_radius 中。

因此，通过调用该函数可以创建一个四面体，并计算其外接球的圆心和半径。*/
void cTetrahedron::Create(const Vector3 &p1,
	const Vector3 &p2,
	const Vector3 &p3,
	const Vector3 &p4)
{
	const Vector3 center = (p1 + p2 + p3 + p4) / 4.f;

	{
		const Vector3 c = (p1 + p2 + p3) / 3.f;
		const Vector3 front = (c - center).Normal() * 10.f + c;
		m_tr[0].Create(p1, p2, p3, front);
	}

	{
		const Vector3 c = (p2 + p3 + p4) / 3.f;
		const Vector3 front = (c - center).Normal() * 10.f + c;
		m_tr[1].Create(p2, p3, p4, front);
	}

	{
		const Vector3 c = (p3 + p4 + p1) / 3.f;
		const Vector3 front = (c - center).Normal() * 10.f + c;
		m_tr[2].Create(p3, p4, p1, front);
	}

	{
		const Vector3 c = (p4 + p1 + p2) / 3.f;
		const Vector3 front = (c - center).Normal() * 10.f + c;
		m_tr[3].Create(p4, p1, p2, front);
	}

	m_p[0] = p1;
	m_p[1] = p2;
	m_p[2] = p3;
	m_p[3] = p4;

	Vector3 extra;
	TetCircumCenter((float*)&p1, (float*)&p2, (float*)&p3, (float*)&p4,
		(float*)&m_circumCenter, (float*)&extra);

	m_radius = (m_circumCenter - p1).Length();
}

/*首先通过比较指针地址判断两个对象是否为同一个对象，即判断 this 和 rhs 的地址是否相等。如果不是同一个对象，则执行以下步骤；如果是同一个对象，则直接返回 true。

比较当前四面体的第一个顶点 m_p[0] 和参数 rhs 的第一个顶点 rhs.m_p[0] 是否相等。这里使用了 Vector3 类型的 operator== 进行比较。

如果 m_p[0] 和 rhs.m_p[0] 相等，则返回 true，表示两个四面体相等；否则返回 false，表示两个四面体不相等。

因此，通过调用该函数可以判断两个四面体是否相等。相等的判定条件是它们的第一个顶点相等。*/
bool cTetrahedron::operator==(const cTetrahedron &rhs) const
{
	if (this != &rhs)
	{
		return m_p[0] == rhs.m_p[0];
	}
	return true;
}

