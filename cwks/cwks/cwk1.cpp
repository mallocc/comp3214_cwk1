#include "cwk1.h"


int cwk1_main()
{
	RGL::StartGraphics("Basic Test");
	RGL::World.DefaultColour = WHITE;
	RGL::ClearScreen();
	RGL::Lock();
	for (int i = 0; i < HEIGHT; i++) {
		RGL::SetPixel(i, i, RED);
		/* This is bad practice, as it hides a call to RGL::Colour */

	}
	for (int i = 20; i < HEIGHT / 2; i++)
		for (int j = WIDTH / 2; j < (WIDTH - 20); j++)
			RGL::SetPixelRGB(j, i, 0, 0, 0);
	RGL::UnLock();
	RGL::StopGraphics(TRUE);
	return 0;
}