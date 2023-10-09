//
// 2016-12-27, jjuiddong
// Tetrahedron (사면체) 을 표현하는 클래스
//
#pragma once

#include "triangle3.h"

namespace delaunay3d
{

	class cTetrahedron
	{
	public:
		cTetrahedron();

		//创建一个四面体
		cTetrahedron(const Vector3 &p1,
			const Vector3 &p2,
			const Vector3 &p3,
			const Vector3 &p4);

		//创建一个四面体
		void Create(const Vector3 &p1,
			const Vector3 &p2,
			const Vector3 &p3,
			const Vector3 &p4);

		bool IsContain(const Vector3 &p);  //判断一个点是否在四面体内部
		bool IsContainVertex(const Vector3 &p);  //判断一个点是否是四面体的一个顶点

		bool operator==(const cTetrahedron &rhs) const;  //判断两个四面体是否相等


	public:
		cTriangle m_tr[4];  //4个三角形
		Vector3 m_p[4];  //4个顶点
		Vector3 m_circumCenter;  //外接球圆心
		float m_radius;  //外接球半径
	};

}
