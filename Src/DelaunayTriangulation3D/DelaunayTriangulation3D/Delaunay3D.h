#pragma once

#include "triangle3.h"
#include "Tetrahedron.h"

namespace delaunay3d
{

	class cDelaunay3D
	{
	public:
		void Triangulate(std::vector<Vector3> &vertices);
		
		std::vector<Vector3> m_vertices;  //顶点
		std::vector<cTetrahedron> m_tetrahedrones;  //四面体
	};

}
