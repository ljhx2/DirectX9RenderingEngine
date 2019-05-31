int Model_Type;

float4x4 ControlMatrixArray[50];
float4x4 Mat_World;
float4x4 Mat_LightViewProj;


struct VS_INPUT
{
	float4 position: POSITION;
	float4 weight : BLENDWEIGHT;
	float4 index : BLENDINDICES;
};

struct VS_OUTPUT
{
	float4 mPosition: POSITION;
	float4 mClipPosition: TEXCOORD1;
};

VS_OUTPUT CreateShadowShader_CreateShadow_Vertex_Shader_vs_main(VS_INPUT input)
{
	VS_OUTPUT Output;
	if (Model_Type == 1)
	{
		float4x4 resultMat = (float4x4)0;
		float resultWeight = 0;

		resultMat += ControlMatrixArray[(int)input.index[0]] * input.weight[0];
		resultWeight += input.weight[0];

		resultMat += ControlMatrixArray[(int)input.index[1]] * input.weight[1];
		resultWeight += input.weight[1];

		resultMat += ControlMatrixArray[(int)input.index[2]] * input.weight[2];
		resultWeight += input.weight[2];

		resultMat += ControlMatrixArray[(int)input.index[3]] * input.weight[3];
		resultWeight += input.weight[3];

		float4 pos;
		pos.x = (resultMat._11 * input.position.x) + (resultMat._21 * input.position.y) + (resultMat._31 * input.position.z) + resultMat._41;
		pos.y = (resultMat._12 * input.position.x) + (resultMat._22 * input.position.y) + (resultMat._32 * input.position.z) + resultMat._42;
		pos.z = (resultMat._13 * input.position.x) + (resultMat._23 * input.position.y) + (resultMat._33 * input.position.z) + resultMat._43;
		pos.w = 1.0;

		float4 worldPosition = mul(pos, Mat_World);

		Output.mPosition = mul(worldPosition, Mat_LightViewProj);
		Output.mClipPosition = Output.mPosition;

		return Output;
	}
	else// if (Model_Type == -1)
	{
		float4 worldPosition = mul(input.position, Mat_World);

		Output.mPosition = mul(worldPosition, Mat_LightViewProj);
		Output.mClipPosition = Output.mPosition;

		return Output;
	}
}

struct PS_INPUT
{
	float4 mClipPosition: TEXCOORD1;
};

float4 CreateShadowShader_CreateShadow_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR
{
	float depth = Input.mClipPosition.z / Input.mClipPosition.w;
	return float4(depth.xxx, 1.0f);
}

//--------------------------------------------------------------//
// Technique Section for CreateShadowShader
//--------------------------------------------------------------//
technique CreateShadowShader
{
	pass CreateShadow
	{
		VertexShader = compile vs_2_0 CreateShadowShader_CreateShadow_Vertex_Shader_vs_main();
		PixelShader = compile ps_2_0 CreateShadowShader_CreateShadow_Pixel_Shader_ps_main();
	}
}

