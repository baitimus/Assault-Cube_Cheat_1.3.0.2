#pragma once
class entity;



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

	
	

   bool worldToScreen(float viewMatrix[15], entity& ent, myMath::Vec2 screen) {
        // Assuming ent has headX, headY, headZ for the position
        myMath::Vec4 clipCoords;
        clipCoords.x = ent.headX * viewMatrix[0] + ent.headY * viewMatrix[4] + ent.headZ * viewMatrix[8] + viewMatrix[12];
        clipCoords.y = ent.headX * viewMatrix[1] + ent.headY * viewMatrix[5] + ent.headZ * viewMatrix[9] + viewMatrix[13];
        clipCoords.z = ent.headX * viewMatrix[2] + ent.headY * viewMatrix[6] + ent.headZ * viewMatrix[10] + viewMatrix[14];
        clipCoords.w = ent.headX * viewMatrix[3] + ent.headY * viewMatrix[7] + ent.headZ * viewMatrix[11] + viewMatrix[15];

        if (clipCoords.w < 0.1f)
            return false;

        myMath::Vec3 NDC;
        NDC.x = clipCoords.x / clipCoords.w;
        NDC.y = clipCoords.y / clipCoords.w;
        NDC.z = clipCoords.z / clipCoords.w;

        // Assuming screen width and height are defined somewhere
        int screenWidth = screen.x;
		int screenHeight = screen.y;

      
        screen.x = (screenWidth / 2 * NDC.x) + (NDC.x + screenWidth / 2);
        screen.y = -(screenHeight / 2 * NDC.y) + (NDC.y + screenHeight / 2);
		std::cout << "Screen X: " << screen.x << std::endl;
		std::cout << "Screen Y: " << screen.y << std::endl;
		ent.screenX = screen.x;
		ent.screenY = screen.y;
        // You can store or use the screen coordinates as needed
        return true;
    }



};