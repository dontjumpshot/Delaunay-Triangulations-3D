
#include "stdafx.h"
#include "vertexbuffer.h"

using namespace graphic;


cVertexBuffer::cVertexBuffer() 
:  m_fvf(0)
,	m_sizeOfVertex(0)
,	m_vertexCount(0)
,	m_pVtxBuff(NULL)
,	m_pVtxDecl(NULL)
,	m_isManagedPool(true)
{

}

cVertexBuffer::~cVertexBuffer()
{
	Clear();
}


bool cVertexBuffer::Create(cRenderer &renderer, const int vertexCount, const int sizeofVertex, DWORD fvf)
{
	SAFE_RELEASE(m_pVtxBuff);

	if (FAILED(renderer.GetDevice()->CreateVertexBuffer( vertexCount*sizeofVertex,
		D3DUSAGE_WRITEONLY, 
		fvf,
		D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))
	{
		return false;
	}

	m_fvf = fvf;
	m_vertexCount = vertexCount;
	m_sizeOfVertex = sizeofVertex;
	m_isManagedPool = true;
	return true;
}


// Video Memory 에 버텍스 버퍼를 생성한다.
// 웬만하면 사용하지 말자. 잘쓰려면 복잡해지고, 잘 쓰더라도 효과가 거의 없다.
bool cVertexBuffer::CreateVMem(cRenderer &renderer, const int vertexCount, const int sizeofVertex, DWORD fvf)
{
	SAFE_RELEASE(m_pVtxBuff);

	if (FAILED(renderer.GetDevice()->CreateVertexBuffer( vertexCount*sizeofVertex,
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 
		fvf,
		D3DPOOL_DEFAULT, &m_pVtxBuff, NULL)))
	{
		return false;
	}

	m_fvf = fvf;
	m_vertexCount = vertexCount;
	m_sizeOfVertex = sizeofVertex;
	m_isManagedPool = false;
	return true;
}


// 버텍스 버퍼 생성.
bool cVertexBuffer::Create(cRenderer &renderer, const int vertexCount, const int sizeofVertex, const cVertexDeclaration &decl)
{
	SAFE_RELEASE(m_pVtxBuff);
	SAFE_RELEASE(m_pVtxDecl);

	if (FAILED(renderer.GetDevice()->CreateVertexDeclaration(&decl.GetDecl()[0], &m_pVtxDecl)))
	{
		return false; //Failed to create vertex declaration.
	}

	if (FAILED(renderer.GetDevice()->CreateVertexBuffer( vertexCount*sizeofVertex,
		D3DUSAGE_WRITEONLY, 
		0,
		D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))
	{
		return false;
	}

	m_vtxDecl = decl;
	m_vertexCount = vertexCount;
	m_sizeOfVertex = sizeofVertex;
	m_isManagedPool = true;
	return true;
}


void* cVertexBuffer::Lock()
{
	RETV(!m_pVtxBuff, NULL);

	void *vertices = NULL;
	if (FAILED(m_pVtxBuff->Lock( 0, 0, (void**)&vertices, 0)))
		return NULL;

	return vertices;
}


// idx : 몇 번째 버텍스 부터 Lock 할지를 가르킨다.
// size: 몇 개의 버텍스를 Lock 할지를 가르킨다.
void* cVertexBuffer::LockDiscard(const int idx, const int size) // idx=0, size=0
{
	RETV(!m_pVtxBuff, NULL);
	RETV(m_isManagedPool, NULL);

	void *vertices = NULL;
	if (FAILED(m_pVtxBuff->Lock(idx*m_sizeOfVertex, size*m_sizeOfVertex, 
		(void**)&vertices, D3DLOCK_DISCARD)))
		return NULL;

	return vertices;
}


// idx : 몇 번째 버텍스 부터 Lock 할지를 가르킨다.
// size: 몇 개의 버텍스를 Lock 할지를 가르킨다.
void* cVertexBuffer::LockNooverwrite(const int idx, const int size) // idx=0, size=0
{
	RETV(!m_pVtxBuff, NULL);
	RETV(m_isManagedPool, NULL);

	void *vertices = NULL;
	if (FAILED(m_pVtxBuff->Lock(idx*m_sizeOfVertex, size*m_sizeOfVertex, 
		(void**)&vertices, D3DLOCK_NOOVERWRITE)))
		return NULL;

	return vertices;
}


void cVertexBuffer::Unlock()
{
	if (!m_pVtxBuff)
		return;

	m_pVtxBuff->Unlock();
}


void cVertexBuffer::Bind(cRenderer &renderer) const
{
	if (m_pVtxDecl)
	{
		renderer.GetDevice()->SetFVF( 0 );
		renderer.GetDevice()->SetVertexDeclaration(m_pVtxDecl);
	}
	else
	{
		renderer.GetDevice()->SetFVF(m_fvf);
	}

	renderer.GetDevice()->SetStreamSource(0, m_pVtxBuff, 0, m_sizeOfVertex);
}



void cVertexBuffer::RenderLineStrip(cRenderer &renderer)
{
	RET(!m_pVtxBuff);

	renderer.GetDevice()->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)&Matrix44::Identity);

	DWORD lighting;
	renderer.GetDevice()->GetRenderState(D3DRS_LIGHTING, &lighting);
	renderer.GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	renderer.GetDevice()->SetRenderState(D3DRS_ZENABLE, FALSE);
	Bind(renderer);
	renderer.GetDevice()->DrawPrimitive(D3DPT_LINESTRIP, 0, m_vertexCount - 1);
	renderer.GetDevice()->SetRenderState(D3DRS_LIGHTING, lighting);
	renderer.GetDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
}


void cVertexBuffer::RenderLineList(cRenderer &renderer)
{
	RET(!m_pVtxBuff);

	renderer.GetDevice()->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)&Matrix44::Identity);

	DWORD lighting;
	renderer.GetDevice()->GetRenderState(D3DRS_LIGHTING, &lighting);
	renderer.GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	renderer.GetDevice()->SetRenderState(D3DRS_ZENABLE, FALSE);
	Bind(renderer);
	renderer.GetDevice()->DrawPrimitive(D3DPT_LINELIST, 0, m_vertexCount / 2);
	renderer.GetDevice()->SetRenderState(D3DRS_LIGHTING, lighting);
	renderer.GetDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
}

// ZBuffer Enable
void cVertexBuffer::RenderLineList2(cRenderer &renderer)
{
	RET(!m_pVtxBuff);

	renderer.GetDevice()->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)&Matrix44::Identity);

	DWORD lighting;
	renderer.GetDevice()->GetRenderState(D3DRS_LIGHTING, &lighting);
	renderer.GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	Bind(renderer);
	renderer.GetDevice()->DrawPrimitive(D3DPT_LINELIST, 0, m_vertexCount / 2);
	renderer.GetDevice()->SetRenderState(D3DRS_LIGHTING, lighting);
}


inline DWORD FtoDW(FLOAT f) { return *((DWORD*)&f); }
void cVertexBuffer::RenderPointList2(cRenderer &renderer, const int count) // count=0
{
	RET(!m_pVtxBuff);

	DWORD lighting;
	renderer.GetDevice()->GetRenderState(D3DRS_LIGHTING, &lighting);
	renderer.GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	renderer.GetDevice()->SetRenderState(D3DRS_ZENABLE, FALSE);

	renderer.GetDevice()->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
	renderer.GetDevice()->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);
	renderer.GetDevice()->SetRenderState(D3DRS_POINTSIZE_MAX, FtoDW(10));
	renderer.GetDevice()->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDW(1.0f));
	renderer.GetDevice()->SetRenderState(D3DRS_POINTSCALE_A, FtoDW(0.0f));
	renderer.GetDevice()->SetRenderState(D3DRS_POINTSCALE_B, FtoDW(0.0f));
	renderer.GetDevice()->SetRenderState(D3DRS_POINTSCALE_C, FtoDW(1.0f));

	renderer.GetDevice()->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)&Matrix44::Identity);

	Bind(renderer);
	renderer.GetDevice()->DrawPrimitive(D3DPT_POINTLIST, 0, (count == 0) ? m_vertexCount : count);

	renderer.GetDevice()->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
	renderer.GetDevice()->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
	renderer.GetDevice()->SetRenderState(D3DRS_LIGHTING, lighting);
	renderer.GetDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
}


void cVertexBuffer::RenderPointList(cRenderer &renderer, const int count) // count=0
{
	RET(!m_pVtxBuff);

	renderer.GetDevice()->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)&Matrix44::Identity);

	Bind(renderer);
	renderer.GetDevice()->DrawPrimitive(D3DPT_POINTLIST, 0, (count == 0) ? m_vertexCount : count);
}


void cVertexBuffer::RenderTriangleStrip(cRenderer &renderer)
{
	RET(!m_pVtxBuff);
	Bind(renderer);
	renderer.GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_vertexCount - 2);
}


void cVertexBuffer::RenderTriangleList(cRenderer &renderer)
{
	RET(!m_pVtxBuff);
	Bind(renderer);
	renderer.GetDevice()->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_vertexCount / 3);
}



void cVertexBuffer::Clear()
{
	m_fvf = 0;
	m_vertexCount = 0;
	m_sizeOfVertex = 0;
	m_isManagedPool = true;
	SAFE_RELEASE(m_pVtxBuff);	
	SAFE_RELEASE(m_pVtxDecl);
}


//cVertexBuffer& cVertexBuffer::operator=(cVertexBuffer &rhs)
void cVertexBuffer::Set(cRenderer &renderer, cVertexBuffer &rhs)
{
	if (this != &rhs)
	{
		m_fvf = rhs.m_fvf;
		m_sizeOfVertex = rhs.m_sizeOfVertex;
		m_vertexCount = rhs.m_vertexCount;
		
		if (Create(renderer, rhs.m_vertexCount, rhs.m_sizeOfVertex, rhs.m_fvf))
		{
			if (BYTE* dest = (BYTE*)Lock())
			{
				if (BYTE *src = (BYTE*)rhs.Lock())
				{
					memcpy(dest, src, rhs.m_vertexCount*m_sizeOfVertex);
					rhs.Unlock();
				}
				Unlock();
			}
		}
	}
	//return *this;
}
