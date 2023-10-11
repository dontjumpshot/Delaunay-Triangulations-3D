#pragma once

#include "Tetrahedron.h"

namespace delaunay3d
{

	class cRenderTetrahedron
	{
	public:
		cRenderTetrahedron();
		virtual ~cRenderTetrahedron();

		//初始化函数，用于传入渲染器和四面体，并创建顶点缓冲区存储四面体的顶点数据
		bool Init(graphic::cRenderer &renderer, const cTetrahedron &tet);

		//绘制函数，用于将四面体绘制到屏幕上。可以选择是否绘制法向量和边缘。
		void Render(graphic::cRenderer &renderer, const bool renderNorm=false, const bool renderEdge=false);

		cTetrahedron m_tet;  //存储四面体的信息
		graphic::cVertexBuffer m_vtxBuffer;  //存储四面体顶点数据的顶点缓冲区
		graphic::cVertexBuffer m_normalBuffer;  //存储四面体法向量数据的顶点缓冲区
		graphic::cVertexBuffer m_edgeBuffer;  //存储四面体边缘数据的顶点缓冲区
	};

}
