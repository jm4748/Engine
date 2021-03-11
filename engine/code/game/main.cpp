#include "states.h"


int main()
{
	System::Run( new MainState() );

	system( "pause" );

	return 0;
}