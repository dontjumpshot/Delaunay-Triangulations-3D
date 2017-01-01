//------------------------------------------------------------------------
// Name:    Task.h
// Author:  jjuiddong
// Date:    2012-12-02
// 
// �����忡�� ����Ǵ� �½�ũ�� �����Ѵ�.
//
// 2016-06-06
//		- refactoring
//
//------------------------------------------------------------------------
#pragma once

#include "threaddef.h"


namespace common
{
	class cThread;
	class cTask
	{
	public:
		cTask(int id, const std::string &name="");
		virtual ~cTask() {}

		enum RUN_RESULT
		{
			RR_END,			// �½�ũ ����
			RR_CONTINUE,	// �½�ũ ��ӽ���
		};

		int GetId() const;
		const std::string& GetName() const;
		void	SetThread(cThread *p);
		cThread* GetThread();

		// overriding
		virtual RUN_RESULT	Run() { return RR_END; }
		virtual void MessageProc( threadmsg::MSG msg, WPARAM wParam, LPARAM lParam, LPARAM added ) {}


	public:
		int m_Id;
		cThread *m_pThread;
		std::string m_Name;
	};

	inline cTask::cTask(int id, const std::string &name) : m_Id(id), m_pThread(NULL), m_Name(name) { }
	inline int cTask::GetId() const { return m_Id; }
	inline const std::string& cTask::GetName() const { return m_Name; }
	inline void	 cTask::SetThread(cThread *p) { m_pThread = p; }
	inline cThread* cTask::GetThread() { return m_pThread; }


	// list<CTask*>���� CTask�� ã�� ��ü
	class IsTask : public std::unary_function<cTask*, bool>
	{
	public:
		IsTask(int taskId):m_id(taskId) {}
		int m_id;
		bool operator ()(cTask *t) const
			{ return (t->GetId() == m_id); }
	};

}
