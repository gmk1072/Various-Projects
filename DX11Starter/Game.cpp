#include "Game.h"
#include "Vertex.h"

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	// Initialize fields
	//vertexBuffer = 0;
	//indexBuffer = 0;
	vertexShader = 0;
	pixelShader = 0;

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
#endif
}

Camera camera = Camera();

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Release any (and all!) DirectX objects
	// we've made in the Game class
	//if (vertexBuffer) { vertexBuffer->Release(); }
	//if (indexBuffer) { indexBuffer->Release(); }

	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete vertexShader;
	delete pixelShader;

	delete mesh1;
	delete mesh2;
	delete mesh3;
	delete mesh4;

	delete cone;
	delete cube;
	delete cylinder;
	delete helix;
	delete sphere;
	delete torus;
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	CreateMatrices();
	CreateBasicGeometry();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dirLight = DirectionalLight();
	dirLight.AmbientColor = XMFLOAT4(.1, .1, .1, 1.0);
	dirLight.DiffuseColor = XMFLOAT4(0, 0, 1, 1);
	dirLight.Direction = XMFLOAT3(-1,-1, 0);
	dirLight2 = DirectionalLight();
	dirLight2.AmbientColor = XMFLOAT4(.1, .1, .1, 1.0);
	dirLight2.DiffuseColor = XMFLOAT4(0, 1, 0, 1);
	dirLight2.Direction = XMFLOAT3(1, 1, 0);
	pointLight = PointLight();
	pointLight.DiffuseColor = XMFLOAT4(1, 0, 0, 1);
	pointLight.Position = XMFLOAT3(0, 0, -1);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	if (!vertexShader->LoadShaderFile(L"Debug/VertexShader.cso"))
		vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, context);
	if (!pixelShader->LoadShaderFile(L"Debug/PixelShader.cso"))
		pixelShader->LoadShaderFile(L"PixelShader.cso");

	mat_default = Material(vertexShader, pixelShader);

	// You'll notice that the code above attempts to load each
	// compiled shader file (.cso) from two different relative paths.

	// This is because the "working directory" (where relative paths begin)
	// will be different during the following two scenarios:
	//  - Debugging in VS: The "Project Directory" (where your .cpp files are) 
	//  - Run .exe directly: The "Output Directory" (where the .exe & .cso files are)

	// Checking both paths is the easiest way to ensure both 
	// scenarios work correctly, although others exist
}



// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void Game::CreateMatrices()
{
	// Set up world matrix
	// - In an actual game, each object will need one of these and they should
	//   update when/if the object moves (every frame)
	// - You'll notice a "transpose" happening below, which is redundant for
	//   an identity matrix.  This is just to show that HLSL expects a different
	//   matrix (column major vs row major) than the DirectX Math library
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!

	// Create the View matrix
	// - In an actual game, recreate this matrix every time the camera 
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction vector along which to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space
	XMVECTOR pos = XMVectorSet(0, 0, -5, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!
	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//   the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
	camera.updateProjectionMatrix(width, height);
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	Vertex vertices[] =
	{
		{ XMFLOAT3(+0.0f , +1.0f, +0.0f), XMFLOAT3(+0.0f , +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f) },//top-0
		{ XMFLOAT3(+1.5f , -1.0f, +0.0f), XMFLOAT3(+0.0f , +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f) },//bottom right-1
		{ XMFLOAT3(-1.5f , -1.0f, +0.0f), XMFLOAT3(+0.0f , +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f) },//bottom left-2
	};
	Vertex vertices2[] =
	{
		{ XMFLOAT3(+0.0f, +1.0f, +0.0f), XMFLOAT3(+0.0f , +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f) },//top left-0
		{ XMFLOAT3(+1.5f, -1.0f, +0.0f), XMFLOAT3(+0.0f , +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f) },//bottom right-1
		{ XMFLOAT3(-1.5f, -1.0f, +0.0f), XMFLOAT3(+0.0f , +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f) },//bottom left-2
		{ XMFLOAT3(+1.5f, +1.0f, +0.0f), XMFLOAT3(+0.0f , +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f) },//top right-3
	};
	Vertex vertices3[] =
	{
		{ XMFLOAT3(+0.0f, +0.0f, +0.0f), XMFLOAT3(+0.0f , +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f) },//center-0                                                           5                                                                                          																					
		{ XMFLOAT3(+1.0f, -1.0f, +0.0f), XMFLOAT3(+0.0f , +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f) },	//bottom right-1                                                / | \                                                                                   
		{ XMFLOAT3(+0.0f, -1.5f, +0.0f), XMFLOAT3(+0.0f , +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f) },    //bottom-2                                                    4/    |    \6                                 
		{ XMFLOAT3(-1.0f, -1.0f, +0.0f), XMFLOAT3(+0.0f , +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f) }, //bottom left-3                                               |  \  |  /  |                                                  
		{ XMFLOAT3(-1.0f, +1.0f, +0.0f), XMFLOAT3(+0.0f , +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f) },  //top left-4                                                  |     0     |                                                   
		{ XMFLOAT3(+0.0f, +1.5f, +0.0f), XMFLOAT3(+0.0f , +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f) },   //top-5                                                       |  /  |  \  |                                                                             
		{ XMFLOAT3(+1.0f, +1.0f, +0.0f), XMFLOAT3(+0.0f , +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f) },  //top right-6                                                 3/    |    \1                                                       
	};                                          //																      \   |   /                                       
												//                                                                      \ 2 /                                     

	Vertex vertices4[] = 
	{
		{ XMFLOAT3(+0.0f, +1.0f, +0.0f), XMFLOAT3(+0.0f , +1.0f, +0.0f), XMFLOAT2(+0.0f, +0.0f) },//top-0
		{ XMFLOAT3(+0.0f, +0.0f, +1.0f), XMFLOAT3(+0.0f , +0.0f, +1.0f), XMFLOAT2(+0.0f, +0.0f) },//back-1
		{ XMFLOAT3(+1.0f, +0.0f, -1.0f), XMFLOAT3(+0.0f , +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f) },//right-2
		{ XMFLOAT3(-1.0f, +0.0f, -1.0f), XMFLOAT3(-1.0f , +0.0f, -1.0f), XMFLOAT2(+0.0f, +0.0f) },//left-3
	};
	unsigned int indices[] = {
		0,1,2
	};
	unsigned int indices2[] = {
		0,1,2,
		0,3,1
	};
	unsigned int indices3[] = {
		0,1,2,
		0,2,3,
		0,3,4,
		0,4,5,
		0,5,6,
		0,6,1
	};
	unsigned int indices4[] = {
		0,1,2,
		0,2,3,
		0,3,1,
		2,1,3
	};
	/*mesh1 = new Mesh(vertices, 3, indices, 3, device);
	mesh2 = new Mesh(vertices2, 4, indices2, 6, device);
	mesh3 = new Mesh(vertices3, 7, indices3, 18, device);*/

	mesh1 = new Mesh(vertices, sizeof(vertices) / 24, indices, sizeof(indices) / 4, device);
	mesh2 = new Mesh(vertices2, sizeof(vertices2) / 24, indices2, sizeof(indices2) / 4, device);
	mesh3 = new Mesh(vertices3, sizeof(vertices3) / 24, indices3, sizeof(indices3) / 4, device);
	mesh4 = new Mesh(vertices4, sizeof(vertices4) / 25, indices4, sizeof(indices4) / 4, device);

	cone = new Mesh("Assets\\Models\\cone.obj", device);
	cube = new Mesh("Assets\\Models\\cube.obj", device);
	cylinder = new Mesh("Assets\\Models\\cylinder.obj", device);
	helix = new Mesh("Assets\\Models\\helix.obj", device);
	sphere = new Mesh("Assets\\Models\\sphere.obj", device);
	torus = new Mesh("Assets\\Models\\torus.obj", device);

	//ge1 = GameEntity(mesh1);
	//ge2 = GameEntity(mesh2);
	//ge3 = GameEntity(mesh3);

	gameEntities.push_back(GameEntity(sphere, &mat_default ));
	gameEntities.push_back(GameEntity(cone, &mat_default));
	gameEntities[gameEntities.size()-1].Move(3.5, 0, 0);
	gameEntities.push_back(GameEntity(cylinder, &mat_default));
	gameEntities[gameEntities.size()-1].Move(-2.5, 0, 0);
	gameEntities.push_back(GameEntity(helix,&mat_default));
	gameEntities[gameEntities.size()-1].Move(1.5, -1, 0);
	gameEntities.push_back(GameEntity(torus, &mat_default));
	gameEntities[gameEntities.size()-1].Move(0, 1.5, 0);
	gameEntities.push_back(GameEntity(cube, &mat_default));
	gameEntities[gameEntities.size()-1].Move(-1.5, -1.5, 0);

	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable


	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in memory
	//    over to a DirectX-controlled data structure (the vertex buffer)


	// Set up the indices, which tell us which vertices to use and in which order
	// - This is somewhat redundant for just 3 vertices (it's a simple example)
	// - Indices are technically not required if the vertices are in the buffer 
	//    in the correct order and each one will be used exactly once
	// - But just to see how it's done...



	// Create the VERTEX BUFFER description -----------------------------------
	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.

	//D3D11_BUFFER_DESC vbd;
	//vbd.Usage				= D3D11_USAGE_IMMUTABLE;
	//vbd.ByteWidth			= sizeof(Vertex) * 3;       // 3 = number of vertices in the buffer
	//vbd.BindFlags			= D3D11_BIND_VERTEX_BUFFER; // Tells DirectX this is a vertex buffer
	//vbd.CPUAccessFlags		= 0;
	//vbd.MiscFlags			= 0;
	//vbd.StructureByteStride	= 0;

	// Create the proper struct to hold the initial vertex data
	// - This is how we put the initial data into the buffer

	//D3D11_SUBRESOURCE_DATA initialVertexData;
	//initialVertexData.pSysMem = vertices;

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN

	//device->CreateBuffer(&vbd, &initialVertexData, &vertexBuffer);



	// Create the INDEX BUFFER description ------------------------------------
	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.

	//D3D11_BUFFER_DESC ibd;
	//ibd.Usage               = D3D11_USAGE_IMMUTABLE;
	//ibd.ByteWidth           = sizeof(int) * 3;         // 3 = number of indices in the buffer
	//ibd.BindFlags           = D3D11_BIND_INDEX_BUFFER; // Tells DirectX this is an index buffer
	//ibd.CPUAccessFlags      = 0;
	//ibd.MiscFlags           = 0;
	//ibd.StructureByteStride = 0;

	// Create the proper struct to hold the initial index data
	// - This is how we put the initial data into the buffer

	/*D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indices;*/

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN

	//device->CreateBuffer(&ibd, &initialIndexData, &indexBuffer);
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	// Update our projection matrix since the window size changed
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		(float)width / height,	// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!

	camera.updateProjectionMatrix(width, height);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	camera.Update(deltaTime);
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();
	
	if (GetAsyncKeyState('E') & 0x8000) {
		speed+= deltaTime ;
		if (speed > 10) speed = 10;
	}
	if (GetAsyncKeyState('Q') & 0x8000) {
		speed-=deltaTime;
		if (speed <1) speed = 1;
	}
	if (GetAsyncKeyState('I') & 0x8000) {
		pointLight.Position = XMFLOAT3(pointLight.Position.x, pointLight.Position.y + (1 * deltaTime), pointLight.Position.z);
	}
	if (GetAsyncKeyState('K') & 0x8000) {
		pointLight.Position = XMFLOAT3(pointLight.Position.x, pointLight.Position.y - (1 * deltaTime), pointLight.Position.z);

	}
	if (GetAsyncKeyState('J') & 0x8000) {
		pointLight.Position = XMFLOAT3(pointLight.Position.x - (1 * deltaTime), pointLight.Position.y, pointLight.Position.z);
	}
	if (GetAsyncKeyState('L') & 0x8000) {
		pointLight.Position = XMFLOAT3(pointLight.Position.x + (1 * deltaTime), pointLight.Position.y, pointLight.Position.z);
	}
	if (GetAsyncKeyState('O') & 0x8000) {
		pointLight.Position = XMFLOAT3(pointLight.Position.x , pointLight.Position.y, pointLight.Position.z + (1 * deltaTime));
	}
	if (GetAsyncKeyState('U') & 0x8000) {
		pointLight.Position = XMFLOAT3(pointLight.Position.x, pointLight.Position.y, pointLight.Position.z - (1 * deltaTime));
	}
	for (int i = 0; i < gameEntities.size(); i++)
	{
		/*if (GetAsyncKeyState('W') & 0x8000) { gameEntities[i].Move(0, deltaTime * speed,0); }
		if (GetAsyncKeyState('S') & 0x8000) { gameEntities[i].Move(0, -deltaTime * speed, 0); }
		if (GetAsyncKeyState('D') & 0x8000) { gameEntities[i].Move(deltaTime * speed, 0, 0); }
		if (GetAsyncKeyState('A') & 0x8000) { gameEntities[i].Move(-deltaTime * speed, 0, 0); }*/
		gameEntities[i].Rotate(0,deltaTime * speed/2,0);
		//gameEntities[i].Move(0,sin(totalTime) * deltaTime * speed/2,0);
		gameEntities[i].UpdateWorldMatrix();
	}
	/**/
	/**
	float sintime = (sin(totalTime * 5) + 2.0f) / 10.f;

	//XMMATRIX world = XMMatrixScaling(sintime, sintime, sintime) * XMMatrixRotationZ(totalTime) * XMMatrixTranslation(2.0f, 0, 0);

	XMMATRIX scale = XMMatrixScaling(sintime, sintime, sintime);
	XMMATRIX rotation = XMMatrixRotationZ(totalTime);
	XMMATRIX translation = XMMatrixTranslation(2.0f, 0, 0);

	//combine and transpose matrices before storing
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(scale * rotation * translation));
	/**/
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

#pragma region for loop game entity code
/**/
	for (int i = 0; i < gameEntities.size(); i++)
	{
		gameEntities[i].PrepareMaterial(camera.GetViewMatrix(), camera.GetProjectionMatrix());

		// Set buffers in the input assembler
		//  - Do this ONCE PER OBJECT you're drawing, since each object might
		//    have different geometry.
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		ID3D11Buffer* vertbuffadd = gameEntities[i].getMesh()->GetVertexBuffer();																					
		context->IASetVertexBuffers(0, 1, &vertbuffadd, &stride, &offset);													
		context->IASetIndexBuffer(gameEntities[i].getMesh()->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);										
		
		pixelShader->SetData("light", &dirLight, sizeof(DirectionalLight));
		pixelShader->SetData("light2", &dirLight2, sizeof(DirectionalLight));
		pixelShader->SetData("pLight", &pointLight, sizeof(PointLight));
		pixelShader->SetFloat3("cameraPosition", camera.getCameraPosition());
		// Finally do the actual drawing
		//  - Do this ONCE PER OBJECT you intend to draw
		//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
		//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
		//     vertices in the currently set VERTEX BUFFER
		context->DrawIndexed(
			gameEntities[i].getMesh()->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)																
			0,     // Offset to the first index we want to use																		
			0);    // Offset to add to each index when looking up vertices																		
	}
	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
/**/   
#pragma endregion
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	camera.Rotate(y, x);
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion