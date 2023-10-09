//
// 2016-12-27, jjuiddong
//
//
#pragma once


namespace delaunay3d
{

	class cTriangle
	{
	public:
		cTriangle();  //默认构造函数
		cTriangle(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, const Vector3 &frontPoint);  //带参构造函数，创建一个三角形

		void Create(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, const Vector3 &frontPoint);  //公有函数，创建一个三角形

		bool operator==(const cTriangle &rhs) const;  //重载等于运算符，判断两个三角形是否相等

		//成员变量
		Vector3 m_p1, m_p2, m_p3;  //顶点
		Vector3 m_normal;  //法线
	};

}
