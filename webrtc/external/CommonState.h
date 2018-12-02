#pragma once

enum CommonState
{
	NONE = 1,
	INITIALIZING,
	INITIALIZED,
	STARTING,
	STARTED,
	STOPPING,
	STOPPED
};

class CCommonState
{
	CommonState meState = NONE;
public:
	CommonState GetState()
	{
		return meState;
	}
	void SetState(CommonState tstate)
	{
		meState = tstate;
	}
	bool CanInit()
	{
		if (meState == NONE || meState == STOPPED)
		{
			return true;
		}
		return false;
	}
};