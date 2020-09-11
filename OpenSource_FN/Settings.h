#pragma once
namespace settings
{
	//aimtype 1 = mouse aim
	//aimtype 2 = memory aim
	//aimtype 3 = none
	int aimtype = 1;

	//0 = no smooth 
	//10 = max smooth
	int smooth = 0;

	//draws a box around the player
	bool boxesp = true;

	//draws a line from the players head to the bottom of the screen
	bool snaplines = true;

	//aimbot fov
	int fov = 300;
}