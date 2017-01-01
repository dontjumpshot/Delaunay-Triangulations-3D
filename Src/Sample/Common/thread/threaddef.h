//
// 2016-06-04, jjuiddong
//
//
#pragma once


namespace common
{

	class cTask;
	typedef std::list<cTask*> TaskList;
	typedef TaskList::iterator TaskItor;

	// ������� �������� �ְ� �޴� �޼��� ����
	typedef struct _SExternalMsg
	{
		int rcvTaskId;			// �޼����� ���� Task ID (0=tread, -1=�ܺο��� �޴� �޼���)
		int msg;
		WPARAM wParam;
		LPARAM lParam;
		LPARAM added;

		_SExternalMsg() {}
		_SExternalMsg(int rcvtaskId, int msgtype, WPARAM wparam, LPARAM lparam, LPARAM _added) :
			rcvTaskId(rcvtaskId), msg(msgtype), wParam(wparam), lParam(lparam), added(_added)
		{
		}

	} SExternalMsg;

	typedef std::list<SExternalMsg> ExternalMsgList;
	typedef ExternalMsgList::iterator ExternalMsgItor;

	
	namespace threadmsg
	{
		enum MSG
		{
			TASK_MSG = 100,// task message
										// wParam : taskId
			TERMINATE_TASK,
										// wParam : taskId
			MSG_LAST
		};
	
	}
}
