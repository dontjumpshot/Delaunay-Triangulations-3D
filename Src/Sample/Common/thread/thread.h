//------------------------------------------------------------------------
// Name:    Thread.h
// Author:  jjuiddong
// Date:    2012-12-02
// 
// ������ ���̺귯��
// CTask �� �����Ѵ�.
//
// 2016-06-06
//		- refactoring
//
//------------------------------------------------------------------------
#pragma once

#include "threaddef.h"


namespace common
{

	class cTask;
	class cThread
	{
	public:
		cThread(const std::string &name="");
		virtual ~cThread();

		enum MSG_OPT
		{
			MSG_REMOVE,	// �޼��� ����
			MSG_KEEP,			// �޼��� ����
		};
		enum STATE
		{
			WAIT,			// �����
			RUN,			// ������
			END,			// ����
		};

		STATE			GetState() const;
		const char* GetName() const;
		HANDLE		GetHandle() const;

		void			Start();				// ������ ����
		void			Terminate(const int milliSeconds=-1);		// ������ ����

		void			Send2ThreadMessage( threadmsg::MSG msg, WPARAM wParam, LPARAM lParam, LPARAM added=0);
		void			Send2ExternalMessage( int msg, WPARAM wParam, LPARAM lParam, LPARAM added=0 );
		bool			GetThreadMsg( OUT SExternalMsg *pMsg, MSG_OPT opt = MSG_REMOVE );
		bool			GetExternalMsg( OUT SExternalMsg *pMsg, MSG_OPT opt = MSG_REMOVE );
		
		bool			AddTask(cTask *pTask);
		bool			RemoveTask(cTask *pTask);
		int			GetTaskCount();
		cTask*		GetTask(const int taskId);
		void			Clear();
		void			Run();
		void			Exit(); // call exit thread

		// Overriding
		virtual void	MessageProc( threadmsg::MSG msg, WPARAM wParam, LPARAM lParam, LPARAM added );

	protected:
		void			DispatchMessage();

	public:
		STATE m_State;
		HANDLE m_hThread;
		char m_Name[ 32];

		/// �ܺ� �����忡�� �����ϱ� ������ ����ȭ �ڵ尡 �� �ִ�.
		CriticalSection m_TaskCS;
		TaskList m_Tasks;
		CriticalSection m_MsgCS;
		ExternalMsgList m_ThreadMsgs;				/// �����尡 ���� �޼���
		ExternalMsgList m_ExternalMsgs;				/// �����忡�� �ܺη� ���� �޼���
	};

	inline cThread::STATE	cThread::GetState() const { return m_State; }
	inline const char* cThread::GetName() const { return m_Name; }
	inline HANDLE cThread::GetHandle() const { return m_hThread; }

}
