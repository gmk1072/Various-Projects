#include "Model.h"


Model::Model()
{
	vertArr = 0;
	vertCount = 0;
}


Model::~Model()
{
}

bool Model::buffer(std::string objFile)
{


	vector<glm::vec3> locations;
	vector<glm::vec2> uvs;
	vector<glm::vec3> normals;
	vector<VertInd> indices;
	std::istringstream stream;
	ifstream inFile;
	string line;
	string fileData;

	inFile.open(objFile);

	if (inFile.is_open())
	{
		while (getline(inFile, line))
		{
			std::istringstream ss(line);
			std::string label;

			ss >> label;
			float f1, f2, f3;
			if (label == "v")
			{

				ss >> f1 >> f2 >> f3;
				locations.push_back(glm::vec3(f1, f2, f3));
			}
			else if (label == "vt")
			{
				ss >> f1 >> f2;
				uvs.push_back(glm::vec2(f1, f2));
			}
			else if (label == "vn")
			{
				ss >> f1 >> f2 >> f3;
				normals.push_back(glm::vec3(f1, f2, f3));
			}
			else if (label == "f")
			{
				unsigned int i1, i2, i3;
				char slash1, slash2;
				ss >> i1 >> slash1 >> i2 >> slash2 >> i3;
				indices.push_back(VertInd{ --i1, --i2, --i3 });
				ss >> i1 >> slash1 >> i2 >> slash2 >> i3;
				indices.push_back(VertInd{ --i1, --i2, --i3 });
				ss >> i1 >> slash1 >> i2 >> slash2 >> i3;
				indices.push_back(VertInd{ --i1, --i2, --i3 });
			}
			else{}
		}
		inFile.close();
	}



	vertCount = indices.size();
	vector<Vertex> vertBufData(vertCount);
	for (unsigned int i = 0; i < vertCount; i++)
	{
		vertBufData[i] = Vertex(locations[indices[i].locInd],
										  uvs[indices[i].uvInd],
										  normals[indices[i].normInd]
								);
	}


	GLuint vertBuf;
	glGenVertexArrays(1, &vertArr);
	glGenBuffers(1, &vertBuf);
	glBindVertexArray(vertArr);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertCount, &(vertBufData[0]), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3)); 
	glBindVertexArray(0);

	return true;
	/*
	std::vector<glm::vec3> locs = { { 1, 1, 0 },{ -1, 1, 0 },{ -1, -1, 0 },{ 1, -1, 0 } };
	std::vector<unsigned int> locInds = { 0,1,2,0,2,3 };
	vertCount = locInds.size();
	std::vector<glm::vec3> vertBufData(vertCount);
	for (unsigned int i = 0; i < vertCount; i++)
	{
		vertBufData[i] = locs[locInds[i]];
	}
	glGenVertexArrays(1, &vertArr);
	glGenBuffers(1, &vertBuf);
	glBindVertexArray(vertArr);
	glBindBuffer(GL_ARRAY_BUFFER, vertBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertCount, &vertBufData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
	return true;
	*/
}

void Model::render()
{
	glBindVertexArray(vertArr);
	glDrawArrays(GL_TRIANGLES, 0, this->vertCount);
	//glBindVertexArray(vertArr);
	//glDrawArrays(GL_TRIANGLES, 0, vertCount);
}
