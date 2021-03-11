#include "system.h"

#include <iostream>


System::System()
{
	running = true;

	currentState = nullptr;
	nextState = nullptr;
	changeState = false;
}


void System::_Run( State* state )
{
	const char* title = "Engine";
	int width = 800;
	int height = 600;

	__int64 timeStep = 40'000;

	bool enableFPS = true;
	int maxFPS = 0;
	int minFPS = 1'000'000;
	int fps = 0;

	if ( core.Startup( title, width, height ) )
	{
		ChangeState( state );

		__int64 lastTime = core.Time();
		__int64 lastFPSTime = lastTime;
		__int64 accumulatedTime = 0;

		while ( running )
		{
			if ( changeState )
			{
				if ( currentState != nullptr )
				{
					currentState->Cleanup();
					delete currentState;
				}

				currentState = nextState;

				currentState->system = this;

				currentState->core = &core;
				currentState->input = core.GetInput();
				currentState->graphics = core.GetGraphics();

				currentState->Startup();

				lastTime = core.Time();
				lastFPSTime = lastTime;
				accumulatedTime = 0;

				changeState = false;
			}
			else
			{
				__int64 currentTime = core.Time();
				accumulatedTime += currentTime - lastTime;
				lastTime = currentTime;

				if ( currentTime - lastFPSTime >= 1'000'000 )
				{
					lastFPSTime = currentTime;

					if ( fps > maxFPS ) maxFPS = fps;
					if ( fps < minFPS ) minFPS = fps;

					if ( enableFPS )
					{
						std::cout << "min: " << minFPS << std::endl
						          << "max: " << maxFPS << std::endl
						          << "cur: " << fps    << std::endl << std::endl;
					}

					fps = 0;
				}

				core.Service();

				while ( accumulatedTime >= timeStep )
				{
					accumulatedTime -= timeStep;

					currentState->Update( timeStep );
					core.ResetFlags();
				}
				
				currentState->Render( ( float )accumulatedTime / ( float )timeStep );
				++fps;
			}
		}

		currentState->Cleanup();
	}
	else
	{
		MessageBox( NULL, "Core Failure", NULL, MB_OK );
	}

	delete currentState;

	core.Cleanup();
}


void System::Run( State* state )
{
	System* system = new System;

	system->_Run( state );

	delete system;
}


void System::ChangeState( State* state )
{
	nextState = state;
	changeState = true;
}


void System::Quit()
{
	running = false;
}