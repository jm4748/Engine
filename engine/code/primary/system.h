#pragma once


#include "core.h"


class State;

class System
{
private:
	Core core;

	bool running;

	State* currentState;
	State* nextState;
	bool changeState;

private:
	System();

	void _Run( State* state );

public:
	static void Run( State* state );

	void ChangeState( State* state );

	void Quit();
};

class State
{
public:
	System* system;

	Core* core;
	Input* input;
	Graphics* graphics;

public:
	virtual void Startup() = 0;
	virtual void Cleanup() = 0;

	virtual void Update( __int64 timeStep ) = 0;
	virtual void Render( float interpolation ) = 0;
};