#pragma once
class entity;
#include "draw.h"


class myMath {
public:
	struct Vec3
	{
		float x, y, z;
	};

	struct Vec2
	{
		float x, y;
	};

	struct Vec4
	{
		float x, y, z, w;
	};

	
	

	bool WorldToScreen(entity ent,myMath::Vec2* screen,float matrix[16], int windowWidth, int windowHeight)
	{
		draw draw;
		Vec4 clipCoords;
		clipCoords.x = ent.headX * matrix[0] + ent.headY * matrix[4] + ent.headZ * matrix[8] + matrix[12];
		clipCoords.y = ent.headX * matrix[1] + ent.headY * matrix[5] + ent.headZ * matrix[9] + matrix[13];
		clipCoords.z = ent.headX * matrix[2] + ent.headY * matrix[6] + ent.headZ * matrix[10] + matrix[14];
		clipCoords.w = ent.headX * matrix[3] + ent.headY * matrix[7] + ent.headZ * matrix[11] + matrix[15];

		if (clipCoords.w < 0.1f)
			return false;

		Vec3 NDC;
		NDC.x = clipCoords.x / clipCoords.w;
		NDC.y = clipCoords.y / clipCoords.w;
		NDC.z = clipCoords.z / clipCoords.w;

		screen->x = (static_cast<float>(windowWidth) / 2 * NDC.x) + (NDC.x + static_cast<float>(windowWidth) / 2);
		screen->y = -(static_cast<float>(windowHeight) / 2 * NDC.y) + (NDC.y + static_cast<float>(windowHeight) / 2);


		std::cout << "Screen X: " << screen->x << std::endl;
		std::cout << "Screen Y: " << screen->y << std::endl;
		
		return true;
	}


};