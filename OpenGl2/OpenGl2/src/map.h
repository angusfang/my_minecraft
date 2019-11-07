#pragma once
#include <vector>
#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <map>

std::string keyValue(int x, int y, int z)
{
	return std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z);
}

class CoorDinate
{
public:
	float x, y, z;

	CoorDinate() {}
	CoorDinate(float a, float b, float c) :x(a), y(b), z(c) {}
	void showCoorDinate() const
	{
		std::cout
			<< " x:" << x
			<< " y:" << y
			<< " z:" << z
			<< std::endl;
	}
	glm::vec3 trans2vec3()
	{
		glm::vec3 a;
		a.x = x;
		a.y = y;
		a.z = z;
		return a;
	}
private:
};
class CoorDinateInt
{
public:
	int x;
	int y;
	int z;
	CoorDinateInt() {}
	CoorDinateInt(int a, int b, int c) :x(a), y(b), z(c) {}
	
	void showCoorDinate() const
	{
		std::cout
			<< " x:" << x
			<< " y:" << y
			<< " z:" << z
			<< std::endl;
	}
	glm::vec3 trans2vec3()
	{
		glm::vec3 a;
		a.x = (float)x;
		a.y = (float)y;
		a.z = (float)z;
		return a;
	}

private:
};
enum BlockType {
	SOIL=1,
	SAND,
	STONE,
	GRASS,
	AIR,
};
class Block
{
public:

	CoorDinateInt position;
	BlockType type;

	Block() :position(0, 0, 0),type(BlockType::AIR)
	{}
	Block(CoorDinateInt position0, BlockType type0= BlockType::AIR) :position(position0), type(type0)
	{}
	Block(int x, int y, int z, BlockType type0 = BlockType::AIR) :position(x, y, z), type(type0)
	{}

	void showPosition()
	{
		position.showCoorDinate();
	}
	void setPosition(int x, int y, int z)
	{
		position.x = x;
		position.y = y;
		position.z = z;
	}
	void setPosition(std::string key)
	{
		std::string Delimiter = ",";
		int first = key.find_first_of(Delimiter);
		if (first == std::string::npos)
		{
			std::cout << "not find any characters" << std::endl;
		}
		int last = key.find_last_of(Delimiter);
		if (last == std::string::npos)
		{
			std::cout << "not find any characters" << std::endl;
		}
		std::stoi(key.substr(0, first));
		int key_x= std::stoi(key.substr(0, first-0));
		int key_y= std::stoi(key.substr(first+1, last-(first + 1)));
		int key_z= std::stoi(key.substr(last+1, key.size()-(last + 1)));
		position.x = key_x;
		position.y = key_y;
		position.z = key_z;
	}
	void setType(BlockType bt)
	{
		type = bt;
	}
	std::vector<CoorDinateInt> calBlockEnviromentCoord(CoorDinateInt center, CoorDinateInt centerE[26])
	{
		CoorDinateInt coordIncludeSelf[27];
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				for (int k = -1; k <= 1; k++)
				{
					//coord[13] is itself
					coordIncludeSelf[(i + 1) + 3 * (j + 1) + 9 * (k + 1)].x = center.x + (float)i;
					coordIncludeSelf[(i + 1) + 3 * (j + 1) + 9 * (k + 1)].y = center.y + (float)j;
					coordIncludeSelf[(i + 1) + 3 * (j + 1) + 9 * (k + 1)].z = center.z + (float)k;
				}
			}
		}
		for (int i = 0; i < 27; i++)
		{
			if (i < 13)
				centerE[i] = coordIncludeSelf[i];
			if (i > 13)
				centerE[i - 1] = coordIncludeSelf[i];
		}
		std::vector<CoorDinateInt> VCoorDinateInt;
		for (int i = 0; i < 27; i++)
			VCoorDinateInt.push_back(coordIncludeSelf[i]);
		return VCoorDinateInt;
	}
};
class BlockFloat
{
public:
	glm::vec3 position;
	BlockType type;

	BlockFloat(glm::vec3 p,BlockType t=AIR):position(p),type(t)
	{}
	BlockFloat(Block b0):position(b0.position.trans2vec3()),type(b0.type)

	{}
	BlockFloat(CoorDinate c, BlockType t = AIR) :type(t)
	{
		position = c.trans2vec3();
	}
};
class Mountain
{
public:
	int height;
	float steep;
	std::map<std::string, BlockType> Composition;
	Mountain() {};
	Mountain(int h) :height(h),steep(1.0f) {};
	Mountain(int h,float s ) :height(h),steep(s) {};


	void createMoutain1(CoorDinateInt const& center,BlockType blockType=SOIL) 
	{
		for (int altitude = 0; altitude < height; altitude++)
		{
			float radius = float(height - altitude) * steep;
			int X_min = (int)center.x - (int)radius;
			int X_max = (int)center.x + (int)radius;
			int y_min = (int)center.y - (int)radius;
			int y_max = (int)center.y + (int)radius;

			for (int x = X_min; x < X_max; x++)
			{
				for (int y = y_min; y < y_max; y++)
				{
					if (((x- center.x) * (x - center.x) + (y-center.y) * (y - center.y)) <= int(radius * radius))
					{
						Block a;
						a.setPosition(x, y, altitude+center.z);
						a.type = blockType;
						std::string key;
						key = keyValue(x,y,altitude+center.z);
						Composition[key] = blockType;
					}
				}
			}
		}
	}
private:
};
class Map
{
public:
	std::map<std::string,BlockType> worldBlockMap;
	std::vector<std::map<std::string, BlockType>> Envir;

	Map()
	{
		
		MapCreatrMountain(5,2.0,CoorDinateInt(0,0,0),BlockType::SOIL);
		MapCreatrMountain(6,0.5,CoorDinateInt(7,10,0),BlockType::SAND);
		MapCreatrMountain(1,20.0,CoorDinateInt(0,0,0),BlockType::SOIL);
		MapCreatrMountain(20,0.2,CoorDinateInt(-10,8,0),BlockType::SAND);
		MapCreatrMountain(3, 10.0, CoorDinateInt(-14, -7, 0), BlockType::SOIL);
		collectEnvirPosition();
	}

	std::vector<BlockFloat> worldBlockFloat()
	{
		std::vector<BlockFloat> blockFloatContain;
		for (std::pair<std::string, BlockType> b : worldBlockMap)
		{
			Block block;
			block.setPosition(b.first);
			block.setType(b.second);
			BlockFloat blockfloat(block);

			blockFloatContain.push_back(blockfloat);
		}
		return blockFloatContain;
	}
	void collectEnvirPosition()
	{

					for (std::map<std::string, BlockType> envVar : Envir)
					{		
						worldBlockMap.insert(envVar.begin(),envVar.end());
					}
				
	}


private:
	
	bool Probability(int p)
	{
		std::random_device rd;
		std::default_random_engine generator(rd());
		std::uniform_real_distribution<float> unif(1.0, p + 1);
		float x = unif(generator);
		if ((int)x == 1)
		{
			return true;
		}
		else
			return false;
	}
	void MapCreatrMountain(int height,float steep,CoorDinateInt center,BlockType blockType)
	{
		Mountain mountain(height, steep);
		mountain.createMoutain1(center, blockType);
		Envir.push_back(mountain.Composition);
	}
	

};