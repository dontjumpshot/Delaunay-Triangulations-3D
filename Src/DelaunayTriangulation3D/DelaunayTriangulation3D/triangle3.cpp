
#include "stdafx.h"
#include "triangle3.h"


using namespace delaunay3d;

cTriangle::cTriangle()
{
}


cTriangle::cTriangle(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3,
	const Vector3 &frontPoint)
{
	Create(p1, p2, p3, frontPoint);
}

/*首先计算出三个顶点的中心点 center，并分别计算出从中心点到每个顶点的向量，并用 Normal 方法将其正则化为单位向量，最终得到向量 a, b, c。

接下来计算从中心点到“前方点”的向量，并将其正则化为单位向量，得到向量 v。

判断向量 (a × b).Normal().DotProduct(v) 的符号，如果为正，则三角形的正面朝向我们所提供的“前方点”，此时顺序不变；如果为负，则三角形的正面朝向相反方向，需要交换顶点 m_p2 和 m_p3 的位置，并将法线向量取反。

最后，将计算得到的三个顶点和法线向量分别赋值给成员变量 m_p1, m_p2, m_p3 和 m_normal。

因此，调用该函数后，将得到一个根据三个顶点确定的三角形，并且该三角形的正面会朝向所提供的“前方点”。*/

void cTriangle::Create(const Vector3 &p1, const Vector3 &p2,
	const Vector3 &p3, const Vector3 &frontPoint)
{
	const Vector3 center = (p1 + p2 + p3) / 3.f;
	const Vector3 a = (p1 - center).Normal();
	const Vector3 b = (p2 - center).Normal();
	const Vector3 c = (p3 - center).Normal();
	
	const Vector3 v = (frontPoint - center).Normal();
	if (a.CrossProduct(b).Normal().DotProduct(v) > 0)
	{
		m_p1 = p1;
		m_p2 = p2;
		m_p3 = p3;
		m_normal = a.CrossProduct(b).Normal();
	}
	else
	{
		m_p1 = p1;
		m_p2 = p3;
		m_p3 = p2;
		m_normal = -a.CrossProduct(b).Normal();
	}
}

/*首先判断两个三角形是否是同一个对象，如果是则返回 true。

否则，分别比较两个三角形的顶点是否完全一致，如果存在至少一个顶点不相同，则说明两个三角形不相等，返回 false。

因此，调用该函数可以判断两个三角形的顶点是否完全一致，从而判断它们是否相等。需要注意的是，这并不能保证两个三角形的位置和方向完全相同，只能保证它们有完全一致的顶点。*/

bool cTriangle::operator==(const cTriangle &rhs) const
{
	if (this != &rhs)
	{
		return	(m_p1 == rhs.m_p1 || m_p1 == rhs.m_p2 || m_p1 == rhs.m_p3) &&
			(m_p2 == rhs.m_p1 || m_p2 == rhs.m_p2 || m_p2 == rhs.m_p3) &&
			(m_p3 == rhs.m_p1 || m_p3 == rhs.m_p2 || m_p3 == rhs.m_p3);
	}
	return true;
}
