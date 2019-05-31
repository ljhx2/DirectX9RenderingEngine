float4x4 gWorldViewProjectionMatrix : WorldViewProjection;
float4 gWorldCameraPosition : ViewPosition;

texture EnvironmentMap_Tex;

samplerCUBE EnvironmentSampler = sampler_state
{
   Texture = (EnvironmentMap_Tex);
   MINFILTER = LINEAR;
   MAGFILTER = LINEAR;
   MIPFILTER = LINEAR;
};

struct VS_INPUT 
{
   float3 Position : SV_POSITION; 
   float3 inNormal: NORMAL;
   float2 inTexCoord : TEXCOORD;
};

struct VS_OUTPUT 
{
   float4 Position : SV_POSITION;
	float3 TexCoord : TEXCOORD;
};

VS_OUTPUT mainVS( VS_INPUT Input )
{
   VS_OUTPUT Output;

   Output.Position = mul( float4(Input.Position, 1.0f), gWorldViewProjectionMatrix ).xyww;
   Output.TexCoord = Input.Position;
   
   return Output;
}

float4 mainPS(VS_OUTPUT Input) : COLOR
{ 
   return texCUBE(EnvironmentSampler, Input.TexCoord);
}

//--------------------------------------------------------------//
// Technique Section for EnvironmentMapping
//--------------------------------------------------------------//
technique TechSky
{
   pass Pass_0
   {
   	 LIGHTING	= FALSE;
	 CULLMODE	= NONE;

      VertexShader = compile vs_3_0 mainVS();
      PixelShader = compile ps_3_0 mainPS();
   }

}

