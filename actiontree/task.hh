#pragma once

class ITask : public UFG::qNode<ITask>
{
public:
	ITrack* m_Track;
	f32 m_TimeBegin;
	f32 m_TimeEnd;

	virtual ~ITask() {};
	virtual void Begin(ActionContext* action_context) = 0;
	virtual bool Update(f32 timestep) = 0;
	virtual void End() = 0;
};

template <typename T>
class Task : public ITask
{
public:
};
