#version 430


layout (location = 5) uniform vec3 lightLoc;
layout (location = 6) uniform vec3 cameraLoc;

in vec2 fragUV;
in vec3 fragmentLoc;
in vec3 fragmentNormal;

uniform sampler2D myTexture;

void main()
{
	vec3 L = normalize(lightLoc - fragmentLoc);
	vec3 E = normalize(cameraLoc - fragmentLoc);
	vec3 H = normalize(L + E);
	vec3 N = normalize(fragmentNormal);
	float ambient = 0;
	float diffuse = 1 * max(dot(L,N), 0);
	float specular= 0 * pow(max(dot(H,N), 0),16);
	float brightness = ambient + diffuse + specular;
	vec4 texColor = texture(myTexture, fragUV);
		
    gl_FragColor= vec4(vec3(texColor * brightness), texColor.a);
    
}