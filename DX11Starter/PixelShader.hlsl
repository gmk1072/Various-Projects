
struct DirectionalLight {
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};
struct PointLight {
	float4 DiffuseColor;
	float3 Position;
};
cbuffer ExternalData : register(b0)
{
	DirectionalLight light;
	DirectionalLight light2;
	PointLight pLight;
	float3 cameraPosition;
}

// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	//float4 color		: COLOR;        // RGBA color
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float3 worldPos		: POSITION;
};


// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{

	input.normal = normalize(input.normal);

	// Ensure the light direction is normalized
	float3 dirDL = normalize(-light.Direction);
	float3 dirDL2 = normalize(-light2.Direction);

	//light.Direction = dirDL;

	// N dot L (Lambert) lighting (direction TO the light)
	float lightAmountDL = saturate(dot(input.normal, dirDL));
	float lightAmountDL2 = saturate(dot(input.normal, dirDL2));

	// Calculate the point light
	float3 dirPl = normalize((pLight.Position - input.worldPos));
	float lightAmountPL = saturate(dot(input.normal, dirPl));

	// Calc specular amount for the POINT LIGHT
	float3 toCamera = normalize(cameraPosition - input.worldPos);
	float3 refl = reflect(-dirPl, input.normal);
	float specular = pow(saturate(dot(refl, toCamera)), 64);

	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
	return
		light.DiffuseColor * lightAmountDL + light.AmbientColor + 
		light2.DiffuseColor * lightAmountDL2 + light2.AmbientColor +
		pLight.DiffuseColor * lightAmountPL + specular.rrrr;

	//return float4(input.normal,1);
}