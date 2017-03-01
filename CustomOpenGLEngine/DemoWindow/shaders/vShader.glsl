#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) uniform mat4 worldMatrix; 
layout (location = 3) uniform mat4 cameraMat;


layout (location = 4) in vec3 normal;


out vec2 fragUV;


out vec3 fragmentLoc;
out vec3 fragmentNormal;


void main()
{

	fragmentLoc = vec3(worldMatrix * vec4(position,1));
	//fragmentNormal = vec3(transpose(inverse(worldMatrix)) * vec4(normal, 0));
	fragmentNormal = vec3(worldMatrix * vec4(normal,0));

	gl_Position = cameraMat * worldMatrix * vec4(position,1);
	fragUV=texCoord; 
}