#pragma once
#include "camera.h"
#include "map.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
enum CollisionCondition
{
	NOT_COLLISION,
	FRONT_COLLISION,
	BACK_COLLISION,
	LEFT_COLLISION,
	RIGHT_COLLISION,
	UP_COLLISION,
	DOWN_COLLISION,
};
enum MainDirection {
	X,Y,Z
};
class Animal
{
private:
	std::string keyValue(int x, int y, int z)
	{
		return std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z);
	}
public:
	CoorDinate position;
	CoorDinate positionLast;

	CoorDinate eyePosition;
	float height;
	float width;
	float length;
	std::vector<BlockFloat> body;
	Animal(float h = 2.0f, float w = 1.0f, float l = 2.0f, CoorDinate posi = CoorDinate(0.0f, 0.0f, 0.0f))
		:height(h), width(w), length(l), position(posi)
	{
		eyePosition.x = (position.x - 0.5) + length / 2.0f;
		eyePosition.y = (position.y - 0.5) + width / 2.0f;
		eyePosition.z = (position.z - 0.5) + height;
		createBody();
		
	}

	void createBody()
	{
		body.clear();

		for (int k = 0; k < height; k++)
		{
			for (int j = 0; j < width; j++)
			{
				for (int i = 0; i < length; i++)
				{
					CoorDinate positionVar = position;
					positionVar.z += (float)k;
					positionVar.y += (float)j;
					positionVar.x += (float)i;
					body.push_back(BlockFloat(positionVar));
				}
			}
		}
	}
	CollisionCondition  CheckAround(Map& const map)
	{
		createBody();
		for (BlockFloat blockOfBody : body)
		{
			float blockx = blockOfBody.position.x;
			float blocky = blockOfBody.position.y;
			float blockz = blockOfBody.position.z;

			int front=int( blockOfBody.position.y + 1.0f);
			int back= int(blockOfBody.position.y - 1.0f);
			int left= int(blockOfBody.position.x-1.0f);
			int right= int(blockOfBody.position.x + 1.0f);
			int up= int(blockOfBody.position.z+1.0f);
			int down= int(blockOfBody.position.z - 1.0f);

			std::string FRONT = keyValue(blockx, front, blockz);
			std::string BACK = keyValue(blockx, back, blockz);
			std::string LEFT = keyValue(left, blocky, blockz);
			std::string RIGHT = keyValue(right, blocky, blockz);
			std::string UP = keyValue(blockx, blocky, up);
			std::string DOWN = keyValue(blockx, blocky, down);

			MainDirection mainDirection;
			mainDirection = mainMoveDirection();

			if(mainDirection==MainDirection::Y)
			{
				if (map.worldBlockMap.count(FRONT))
					return CollisionCondition::FRONT_COLLISION;
				if (map.worldBlockMap.count(BACK))
					return CollisionCondition::BACK_COLLISION;
			}
			if (mainDirection == MainDirection::X)
			{
				if (map.worldBlockMap.count(LEFT))
					return CollisionCondition::LEFT_COLLISION;
				if (map.worldBlockMap.count(RIGHT))
					return CollisionCondition::RIGHT_COLLISION;
			}
			if (mainDirection == MainDirection::Z)
			{
				if (map.worldBlockMap.count(UP))
					return CollisionCondition::UP_COLLISION;
				if (map.worldBlockMap.count(DOWN))
					return CollisionCondition::DOWN_COLLISION;
			}
		}
	}
	CollisionCondition animalMoveRenew(Map& const map)
	{
		positionLast = position;
		position.x = eyePosition.x - length / 2.0f + 0.5f;
		position.y = eyePosition.y - width / 2.0f + 0.5f;
		position.z = eyePosition.z - height + 0.5f;
		return(CheckAround(map));
	}
	glm::vec3 IfCollision(CollisionCondition collisionCondition, glm::vec3 pos)
	{
		float dx=0;
		float dy=0;
		float dz=0;
		if (collisionCondition == CollisionCondition::FRONT_COLLISION)
			dy = +5.0;
		if (collisionCondition == CollisionCondition::BACK_COLLISION)
			dy = -5.0;
		if (collisionCondition == CollisionCondition::LEFT_COLLISION)
			dx = -5.0;
		if (collisionCondition == CollisionCondition::RIGHT_COLLISION)
			dx = +5.0;
		if (collisionCondition == CollisionCondition::UP_COLLISION)
			dz = +5.0;
		if (collisionCondition == CollisionCondition::DOWN_COLLISION)
			dz = -5.0;
		return glm::vec3(pos.x + dx, pos.y + dy, pos.z + dz);
	}
	MainDirection mainMoveDirection()
	{
		float dx = position.x - positionLast.x;
		float dy = position.y - positionLast.y;
		float dz = position.z - positionLast.z;
		MainDirection biggest;
		biggest = dx * dx > dy*dy ? MainDirection::X : MainDirection::Y;
		biggest = dx * dx > dz*dz ? MainDirection::X : MainDirection::Z;
		biggest = dy * dy > dz*dz ? MainDirection::Y : MainDirection::Z;
		return biggest;
	}
};

class Player:public Animal
{
public:
	glm::vec3 eyePosition;
	void setEyePosition(float x, float y, float z)
	{
		eyePosition.x = x;
		eyePosition.y = y;
		eyePosition.z = z;
	}
private:

};
