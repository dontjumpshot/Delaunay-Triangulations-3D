// �ΰ����� ��ü �������̽�
// �ΰ������� �����Ϸ��� ��ü�� �� �������̽��� ��ӹ޾ƾ� �Ѵ�.
#pragma once


namespace ai
{
	static Matrix44 g_tempMat;


	template<class T>
	class iActorInterface
	{
	public:
		iActorInterface(T *p)  : m_ptr(p) {}
		virtual ~iActorInterface() {}

		virtual Matrix44& aiGetTransform() { return g_tempMat; }
		virtual void aiSetAnimation(const string &animationName) {}


	public:
		T *m_ptr;
	};

}
