#include "Engine.h"
Engine::Engine()
{
}
Engine::~Engine()
{
	for (int i = 0; i < objectAmount; i++)
	{
		glDeleteTextures(2, &objects[i].texid);
	}
}
bool Engine::init()
{
	if (glfwInit() == GL_FALSE)
	{
		return false;
	}
	GLFWwindowPtr = glfwCreateWindow(800, 600, " Gavin DSA1 Engine", NULL, NULL);
	if (GLFWwindowPtr != nullptr)
	{
		glfwMakeContextCurrent(GLFWwindowPtr);
	}
	else {
		glfwTerminate();
		return false;
	}
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return false;
	}
	InputManager::init(GLFWwindowPtr);

	camera = Camera();
	objectAmount = 6;
	for (int i = 0; i < objectAmount; i++)
	{
		objects.push_back(Object());
		objects[i].filename = "textures/background.png";//background
		objects[i].transform.loc = glm::vec3(0, 0, -.1);
		objects[i].transform.rot = glm::vec3(0, 0, 0);
		objects[i].transform.size = glm::vec3(1, 1, .01);
		objects[i].rigidBody.velocity = glm::vec3(0, 0, 0);
		objects[i].rigidBody.force = glm::vec3(0, 0, 0);
		objects[i].rigidBody.mass = 1;
		objects[i].collisionType = COLLIDERLESS;
		objects[i].objectType = OBJECT;
	}
	//object 0
	//default
	//object 1
	objects[1].filename = "textures/grass.png";//grass
	objects[1].transform.loc = glm::vec3(0, -1, .01);
	objects[1].transform.size = glm::vec3(1, 1.6, .01);
	//object 2
	player = 2;
	objects[player].filename = "textures/bucketsBackground.png";//buckets
	objects[player].transform.loc = glm::vec3(-.5, -.6, .1);
	objects[player].transform.size = glm::vec3(.1, .1, .1);
	objects[player].collisionType = AABB;
	objects[player].objectType = BUCKETS;
	//object 3
	bomber = 3;
	objects[bomber].transform.loc = glm::vec3(.6, .75, 0.1);
	objects[bomber].filename = "textures/bomber.png";//bomber
	objects[bomber].transform.size = glm::vec3(.1, .15, .01);
	objects[bomber].collisionType = AABB;
	objects[bomber].objectType = BOMBER;
	//object 4
	ground = 4;
	objects[ground].transform.loc = glm::vec3(-1, -1.31, 0.1);
	objects[ground].transform.size = glm::vec3(2, .05, 2);
	objects[ground].collisionType = AABB;
	objects[ground].objectType = GROUND;
	//object 5
	int bomb = 5;
	objects[bomb].filename = "textures/bomb.png";//bomb
	objects[bomb].transform.loc = glm::vec3(.6, .525, .1);
	objects[bomb].transform.size = glm::vec3(.05, .075, .05);
	objects[bomb].collisionType = AABB;
	objects[bomb].objectType = BOMB;
	return true;

	

	glfwSetCursorPos(GLFWwindowPtr, 800 * .5f, 600 * .5f);

}
bool Engine::bufferModels()
{
	if (model.buffer("models/mybox.obj") == false)
	{
		return false;
	}
	return true;
}
bool Engine::useTextures(Object& o)
{
	/*std::vector<Vertex> uvCoord
	{
		Vertex{ glm::vec3(1, 1, 0),glm::vec2(1, 1) ,glm::vec3()},
		Vertex{ glm::vec3(-1, 1, 0),glm::vec2(0, 1) ,glm::vec3() },
		Vertex{ glm::vec3(-1, -1, 0),glm::vec2(0, 0) ,glm::vec3() },
		Vertex{ glm::vec3(1, -1, 0),	glm::vec2(1, 0) ,glm::vec3() }
	};
	vector<unsigned int> locInds = { 0,1,2,0,2,3 };
	uvVertCount = locInds.size();
	vector<unsigned int> uvInds = { 0,1,2,0,2,3 };
	vector<Vertex> vertBufData(uvVertCount);
	for (unsigned int i = 0; i < uvVertCount; i++)
	{
		vertBufData[i] = uvCoord[uvInds[i]];
	}*/
	/*glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * uvVertCount, &vertBufData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec3));*/
	//texture loading
	FIBITMAP* texture1;
	texture1 = FreeImage_Load(FIF_PNG, o.filename);
	FIBITMAP* image32Bit1 = FreeImage_ConvertTo32Bits(texture1);
	FreeImage_Unload(texture1);
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &o.texid);
	glBindTexture(GL_TEXTURE_2D, o.texid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FreeImage_GetWidth(image32Bit1), FreeImage_GetHeight(image32Bit1),
		0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(image32Bit1));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	FreeImage_Unload(image32Bit1);
	return true;
}

bool Engine::collides(const Object &o1, const Object &o2)
{
	if (o1.collisionType == COLLIDERLESS || o2.collisionType == COLLIDERLESS)
	{
		return false;
	}
	if (o1.collisionType == SPHERE && o2.collisionType == SPHERE)
	{
		if (glm::distance(o1.transform.loc, o2.transform.loc) > (o1.transform.size.x + o2.transform.size.x))
		{
			return false;
		}
	}
	if (o1.collisionType == AABB && o2.collisionType == AABB)
	{
		if (glm::distance(o1.transform.loc.x, o2.transform.loc.x) > (o1.transform.size.x + o2.transform.size.x))
		{
			return false;
		}
		if (glm::distance(o1.transform.loc.y, o2.transform.loc.y) > (o1.transform.size.y + o2.transform.size.y))
		{
			return false;
		}
		if (glm::distance(o1.transform.loc.z, o2.transform.loc.z) > (o1.transform.size.z + o2.transform.size.z))
		{
			return false;
		}
	}
	if (o1.collisionType == SPHERE)
	{
		glm::vec3 sphereLocation = o1.transform.loc - o2.transform.loc;
		glm::vec3 closestPoint;
		// x axis
		if (sphereLocation.x < -o2.transform.size.x / 2.0)
		{
			closestPoint.x = -o2.transform.size.x / 2.0;
		}
		else if (sphereLocation.x > o2.transform.size.x / 2.0)
		{
			closestPoint.x = o2.transform.size.x / 2.0;
		}
		else
		{
			closestPoint.x = sphereLocation.x;
		}
		// y axis
		if (sphereLocation.y < -o2.transform.size.y / 2.0)
		{
			closestPoint.y = -o2.transform.size.y / 2.0;
		}
		else if (sphereLocation.y > o2.transform.size.y / 2.0)
		{
			closestPoint.y = o2.transform.size.y / 2.0;
		}
		else
		{
			closestPoint.y = sphereLocation.y;
		}
		// z axis
		if (sphereLocation.z < -o2.transform.size.z / 2.0)
		{
			closestPoint.z = -o2.transform.size.z / 2.0;
		}
		else if (sphereLocation.x > o2.transform.size.z / 2.0)
		{
			closestPoint.z = o2.transform.size.z / 2.0;
		}
		else
		{
			closestPoint.z = sphereLocation.z;
		}

		glm::vec3 dist = sphereLocation - closestPoint;
		if (dist.x*dist.x + dist.y*dist.y + dist.z*dist.z < o1.transform.size.x*o1.transform.size.x)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	if (o2.collisionType == SPHERE)
	{
		glm::vec3 sphereLocation = o2.transform.loc - o1.transform.loc;
		glm::vec3 closestPoint;
		// x axis
		if (sphereLocation.x < -o1.transform.size.x / 2.0)
		{
			closestPoint.x = -o1.transform.size.x / 2.0;
		}
		else if (sphereLocation.x > o1.transform.size.x / 2.0)
		{
			closestPoint.x = o1.transform.size.x / 2.0;
		}
		else
		{
			closestPoint.x = sphereLocation.x;
		}
		// y axis
		if (sphereLocation.y < -o1.transform.size.y / 2.0)
		{
			closestPoint.y = -o1.transform.size.y / 2.0;
		}
		else if (sphereLocation.y > o1.transform.size.y / 2.0)
		{
			closestPoint.y = o1.transform.size.y / 2.0;
		}
		else
		{
			closestPoint.y = sphereLocation.y;
		}
		// z axis
		if (sphereLocation.z < -o1.transform.size.z / 2.0)
		{
			closestPoint.z = -o1.transform.size.z / 2.0;
		}
		else if (sphereLocation.x > o1.transform.size.z / 2.0)
		{
			closestPoint.z = o1.transform.size.z / 2.0;
		}
		else
		{
			closestPoint.z = sphereLocation.z;
		}

		glm::vec3 dist = sphereLocation - closestPoint;
		if (dist.x*dist.x + dist.y*dist.y + dist.z*dist.z < o2.transform.size.x*o2.transform.size.x)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool Engine::gameLoop()
{
	//srand(time(NULL));
	float dropLoc = (float)((rand() % 20)) / 10 - 1;
	float initialLoc = objects[bomber].transform.loc.x;
	float dropTime = 0.0;
	float dropInterval = .5;
	bool lightLocationToggle = false;
	//current time of the program
	t = (float)glfwGetTime();


	//initializes each of the textures for the objects
	for (int i = 0; i < objectAmount; i++)
	{
		useTextures(objects[i]);
	}


	//alpha for textures
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

	int gravity = -15;
	bool wireframe = false;
	//game loop while the window is not supposed to be closed
	while (!glfwWindowShouldClose(GLFWwindowPtr))
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		if (InputManager::getKey(GLFW_KEY_SPACE))
		{
			wireframe = !wireframe;
		}
		if (wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		//gets events
		glfwPollEvents();
		//previous time
		pt = t;
		//current time
		t = (float)glfwGetTime();
		//change in time
		dt = t - pt;
		dropTime += dt;
		//clears window and binds the vertex array
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glBindVertexArray(vertArr);

		//looping for each object to check collisions, update forces and velocities
		for (int i = 0; i < objectAmount; i++)
		{
			//translates the object
			objects[i].transform.matrix = glm::translate(objects[i].transform.loc)*glm::yawPitchRoll(objects[i].transform.rot.y, objects[i].transform.rot.x, objects[i].transform.rot.z)*glm::scale(objects[i].transform.size);
			//resets the force
			objects[i].rigidBody.force = glm::vec3();
			glUniformMatrix4fv(2, 1, GL_FALSE, &objects[i].transform.matrix[0][0]);
			camera.update(dt, GLFWwindowPtr);//camera

			//binds the texture for drawing, then draws
			glBindTexture(GL_TEXTURE_2D, objects[i].texid);
			//glDrawArrays(GL_TRIANGLES, 0, vertCount);

			//normal vec3
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
			//lightloc vec3

			if (InputManager::getKey(GLFW_KEY_L))
			{
				lightLocationToggle = !lightLocationToggle;
			}
			if (!lightLocationToggle)
			{
				glUniform3fv(5, 1, &camera.transform.loc[0]);
			}
			else
			{
				glUniform3fv(5, 1, &glm::vec3(0, 5, 10)[0]);
			}
			
			//cameraloc vec3
			glUniform3fv(6, 1, &camera.transform.loc[0]);

			model.render();
			//input for the character
			if (InputManager::getKey(GLFW_KEY_D))
			{
				objects[player].rigidBody.force.x = 50;//20;
			}
			if (InputManager::getKey(GLFW_KEY_A))
			{
				objects[player].rigidBody.force.x = -50;// 20;
			}


			if (objects[i].objectType == BOMBER)
			{
				if (objects[i].transform.loc.x < dropLoc)
				{
					if (initialLoc > dropLoc)
					{
						dropLoc = ((float)(rand() % 19 - ((dropLoc + 1) * 10)) + ((dropLoc + 1) * 10)) / 10 - 1;
						initialLoc = objects[i].transform.loc.x;
						objects[bomber].rigidBody.velocity.x = 0;
					}
					objects[i].rigidBody.force.x = 1;
				}
				else if (objects[i].transform.loc.x > dropLoc)
				{
					if (initialLoc < dropLoc)
					{
						dropLoc = (float)(rand() % (int)((dropLoc + 1) * 10)) / 10 - 1;
						initialLoc = objects[i].transform.loc.x;
						objects[bomber].rigidBody.velocity.x = 0;
					}
					objects[i].rigidBody.force.x = -1;
				}
			}
			if (t > 30)
			{
				dropInterval = .2;
			}
			if (dropTime > dropInterval)
			{
				dropTime = 0;
				objectAmount++;
				Object newObject = Object();
				newObject.filename = "textures/bomb.png";//bomb
				newObject.transform.rot = glm::vec3(0, 0, 0);
				newObject.transform.size = glm::vec3(.05, .075, .05);
				newObject.transform.loc = glm::vec3(objects[bomber].transform.loc.x, objects[bomber].transform.loc.y - objects[bomber].transform.size.y - newObject.transform.size.y, 0);
				newObject.rigidBody.velocity = glm::vec3(0, 0, 0);
				newObject.rigidBody.force = glm::vec3(0, 0, 0);
				newObject.rigidBody.mass = 1;
				newObject.collisionType = AABB;
				newObject.objectType = BOMB;
				objects.push_back(newObject);
				useTextures(objects[objectAmount - 1]);
			}
			if (objects[i].objectType == BOMB)
			{
				objects[i].rigidBody.force.y = -1;//moves the bomb down
				for (int j = 0; j < objectAmount; j++)
				{
					if (objects[j].objectType == BUCKETS)
					{
						if (collides(objects[i], objects[j]))
						{
							
							objects.erase(objects.begin() + i);
							objectAmount--;
						}
					}
					else if (objects[j].objectType == GROUND)
					{
						if (collides(objects[i], objects[j]))
						{
							cout << "You lose" << endl;
							objects.erase(objects.begin() + i);
							objectAmount--;
						}
					}

				}
			}

			//if not the character(just in case) checks any collisions with the character
			if (i != player)
			{
				if (collides(objects[player], objects[i]))
				{
					//cout << i << ": you collided" << endl;
				}
			}
			objects[player].rigidBody.force -= objects[player].rigidBody.velocity * 16.0f;//drag
			//gravity

			objects[player].rigidBody.force.y = gravity;

			if (collides(objects[player], objects[ground]))
			{
				objects[player].rigidBody.force.y += -gravity;
				objects[player].rigidBody.velocity.y = 0;
			}
			//change in velocity and change in location to update location and velocity
			glm::vec3 dv;
			glm::vec3 dl;
			dv = objects[i].rigidBody.force / objects[i].rigidBody.mass * dt;
			objects[i].rigidBody.velocity += dv;
			dl = objects[i].rigidBody.velocity * dt;
			objects[i].transform.loc += dl;

		}
		glfwSwapBuffers(GLFWwindowPtr);

		if (InputManager::getKey(GLFW_KEY_ESCAPE))//how to close
		{
			glfwSetWindowShouldClose(GLFWwindowPtr, GL_TRUE);
		}
		if (glfwGetKey(GLFWwindowPtr, GLFW_KEY_ESCAPE) == GLFW_PRESS)//or how to close
		{
			glfwTerminate();
			return 0;
		}
	}
	glfwTerminate();
	return true;
}
bool Engine::useShaders()
{
	bool loaded = shade.loadShaders("shaders/vShader.glsl", "shaders/fShader.glsl");
	if (loaded)
	{
		glUseProgram(shade.getProgram());
	}
	return loaded;
}