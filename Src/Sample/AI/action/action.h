//
// Action ����
// �ൿ�� ó���ϴ� Ŭ������. cAction Ŭ������ �߻�Ŭ������ �ƹ� �ൿ��
// ���� �ʴ´�. ��ü���� �ൿ�� �� Ŭ������ ��ӹ޾Ƽ� ó���Ѵ�.
// Action Ʈ���� �����ϱ� ���� �������̽� ��
// �ൿ�� ���۰� ��, ����, �޼����� ó���ϴ� �������̽��� ���ǵȴ�.
//
// �ൿ�� ó���ϱ� ���� ���� �⺻���� ��ɰ� �������̽��� �����Ѵ�.
//
// 2016-05-14
//		- template class �� ����
//
#pragma once

#include "../aidef.h"


namespace ai
{

	template <class T>
	class cAction
	{
	public:
		cAction( 
			iActorInterface<T> *agent = NULL,
			const string &name = "action",
			const string &animationName = "",
			const ACTION_TYPE::TYPE type = ACTION_TYPE::NONE)
			: m_agent(agent)
			, m_name(name)
			, m_animationName(animationName)
			, m_type(type)
			, m_state(ACTION_STATE::WAIT)
			, m_current(NULL)
		{
		}

		virtual ~cAction()
		{
			Clear();
		}

		// Action�� ���� ���̶�� true�� �����ϰ�, 
		// �ƹ� �۾��� ���� �ʴ´ٸ� false �� �����Ѵ�.
		virtual bool Traverse(const float deltaSeconds)
		{
			m_state = ACTION_STATE::RUN;

			if (m_current)
			{
				if (!ActionThrough(deltaSeconds))
					return true;

				if (!m_current->Traverse(deltaSeconds))
					NextAction();
			}
			else
			{
				if (!ActionExecute(deltaSeconds))
					return false;
			}

			return true;
		}


		// false�� �����ϸ� child�� Move()�� ȣ������ �ʴ´�. 
		// �Ļ��� Ŭ�������� �����Ѵ�.
		virtual bool ActionThrough(const float deltaSeconds) 
		{ 
			return true; 
		}

		// Leaf��� �϶��� ȣ��ȴ�.
		// �ൿ�� ó���� �� ���⿡ �ڵ��� �ϸ�ȴ�.
		// false �� �����ϸ� �ൿ�� ����Ǿ��ٴ� ���̴�.
		// �Ļ��� Ŭ�������� �����Ѵ�.
		virtual bool ActionExecute(const float deltaSeconds) 
		{ 
			return true; 
		}

		// ���� ���� �ڽ� Action�� �޼����� ������.
		// �������̵��ؼ� �����Ѵ�.
		virtual bool MessageProccess(const sMsg &msg)
		{
			if (m_current)
				m_current->MessageProccess(msg);
			return true;
		}

		virtual void Clear()
		{
			m_state = ACTION_STATE::STOP;
			ClearChildAction();
		}

		// Action�� ���ÿ� �߰��Ѵ�.
		void PushAction(cAction *action)
		{
			action->m_agent = m_agent;
			m_children.push_back(action);

			// currentAction �缳���Ѵ�. currentAction�� ���� �ֱ� ���̾�� �Ѵ�.
			m_current = action;
		}

		// Action�� ���� Top�� �߰��Ѵ�.
		void PushFrontAction(cAction *action)
		{
			action->m_agent = m_agent;
			m_children.push_back(action);
			rotateright(m_children);

			if (m_children.size() == 1)
			{
				// Action�� �ϳ��ۿ� ���ٸ�, currentAction���� �����Ѵ�.
				m_current = action;
			}
		}


		// Action�� �����ϰ�, �� Action �� current�� �Ҵ��Ѵ�.
		void PopAction()
		{
			if (m_current)
			{
				delete m_current;
			}
			m_current = GetNextAction();
		}


		// ���ڷ� �Ѿ�� action�� �޸𸮱��� �����Ѵ�.
		void RemoveAction(cAction *action)
		{
			for (u_int i = m_children.size() - 1; i >= 0; --i)
			{
				if (*action == *m_children[i])
				{
					delete m_children[i];
					rotatepopvector(m_children, i);
				}
			}

			// currentAction �缳���Ѵ�.
			m_current = m_children.empty() ? NULL : m_children.back();
		}


		void RemoveAction(const string &actionName)
		{
			for (u_int i = m_children.size() - 1; i >= 0; --i)
			{
				if (actionName == m_children[i]->m_name)
				{
					delete m_children[i];
					rotatepopvector(m_children, i);
				}
			}

			// currentAction �缳���Ѵ�.
			m_current = m_children.empty() ? NULL : m_children.back();
		}


		// �ڽ����� �߰��� Action�� �����Ѵ�.
		void ClearChildAction()
		{
			for each (auto action in m_children)
			{
				delete action;
			}
			m_children.clear();
			m_current = NULL;
		}


		cAction* FindAction(const string &actionName, bool isFindChild=true)
		{
			for (u_int i = 0; i < m_children.size(); ++i)
			{
				if (actionName == m_children[i]->m_name)
					return m_children[i];
			}

			if (isFindChild)
			{
				for (u_int i = 0; i < m_children.size(); ++i)
				{
					if (cAction *action = m_children[i]->FindAction(actionName, isFindChild))
						return action;
				}
			}

			return NULL;
		}

		
		cAction* FindAction(ACTION_TYPE::TYPE type, bool isFindChild=true)
		{
			for (u_int i = 0; i < m_children.size(); ++i)
			{
				if (m_children[i]->IsCurrentAction(type))
					return m_children[i];
			}

			if (isFindChild)
			{
				for (u_int i = 0; i < m_children.size(); ++i)
				{
					if (cAction *action = m_children[i]->FindAction(type, isFindChild))
						return action;
				}
			}

			return NULL;
		}


		bool IsCurrentAction(ACTION_TYPE::TYPE type)
		{
			return m_type == type;
		}
		
		// Action Ÿ���� ���Ѵ�. this �� current �߿� 
		// ���� �� Ÿ���� �ִٸ� true fmf flxjsgksek.
		bool IsAction(ACTION_TYPE::TYPE type)
		{
			if (IsCurrentAction(type))
				return true;
			if (m_current)
				return m_current->IsAction(type);
			return false;
		}


		// Leaf ��带 �����Ѵ�.
		cAction* GetLeafAction()
		{
			cAction *action = m_current;
			if (!action)
				return this;
			return action->GetLeafAction();
		}


		virtual int GetSubState() const 
		{ 
			return 0; 
		}
		

		void SetActionState(ACTION_STATE::TYPE state)
		{
			m_state = state;
		}


		bool operator== (const cAction &rhs)
		{
			return m_name == rhs.m_name;
		}


	protected:
		// ���� �׼����� �Ѿ��.
		void NextAction()
		{
			cAction *prevAction = m_current;
			m_current = GetNextAction();
			SAFE_DELETE(prevAction);
		}

		// ���� �׼��� �����Ѵ�.
		cAction* GetNextAction()
		{
			if (m_children.empty())
				return NULL;
			m_children.pop_back();
			if (m_children.empty())
				return NULL;
			return m_children.back();
		}


	public:
		ACTION_STATE::TYPE m_state;
		ACTION_TYPE::TYPE m_type;
		string m_name;
		string m_animationName;
		iActorInterface<T> *m_agent;
		vector<cAction<T>*> m_children;
		cAction<T> *m_current;
	};

}
