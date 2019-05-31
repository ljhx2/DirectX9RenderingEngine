//-----------------------------------------------------------------------------
// 이펙트 변수 정의
//-----------------------------------------------------------------------------
float4x4 gWorldMatrix;				//진호
float4x4 Mat_LightViewProj;			//진호
float4x4 Mat_LightViewProj1;		//진호
float4x4 Mat_LightViewProj2;		//진호
texture ShadowMap_Tex;				//진호
texture ShadowMap_Tex1;				//진호
texture ShadowMap_Tex2;				//진호
float Frustum_far1;				//진호
float Frustum_far2;				//진호
float4x4 Mat_View;				//진호

sampler2D ShadowSampler = sampler_state
{
	Texture = (ShadowMap_Tex);
};								//진호
sampler2D ShadowSampler1 = sampler_state
{
	Texture = (ShadowMap_Tex1);
};	//진호
sampler2D ShadowSampler2 = sampler_state
{
	Texture = (ShadowMap_Tex2);
};	//진호



float4x4 worldViewProj : WorldViewProjection;	//월드 * 카메라 뷰행렬 * 프로젝션 행렬
float4 fFog;				// Fog parameters:
											// fFog.x: fog end value
											// fFog.y: fog start value
											// fFog.z: not used
											// fFog.w: not used
											
static float m_Sat_A = 0.5f;	// Saturation Flag A
static float m_Sat_B = 0.5f;	// Saturation Flag B

float3	m_LgtDir;	// Lighting Direction
float4	m_LgtDif;	// Lighting Color

texture BaseTexture;	//기본 텍스쳐

texture AlphaTex0;		//알파 텍스쳐
texture AlphaTex1;		//알파 텍스쳐
texture AlphaTex2;		//알파 텍스쳐
texture AlphaTex3;		//알파 텍스쳐
texture AlphaTex4;		//알파 텍스쳐
texture AlphaTex5;		//알파 텍스쳐

texture SplatTex0;		//스플래팅 텍스쳐
texture SplatTex1;		//스플래팅 텍스쳐
texture SplatTex2;		//스플래팅 텍스쳐
texture SplatTex3;		//스플래팅 텍스쳐
texture SplatTex4;		//스플래팅 텍스쳐
texture SplatTex5;		//스플래팅 텍스쳐

float	TileWidth;
float	TileHeight;

//-----------------------------------------------------------------------------
//알파 텍스쳐 샘플링 설정.
//-----------------------------------------------------------------------------

sampler alpha0 = sampler_state
{
    Texture   = <AlphaTex0>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler alpha1 = sampler_state
{
    Texture   = <AlphaTex1>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler alpha2 = sampler_state
{
    Texture   = <AlphaTex2>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler alpha3 = sampler_state
{
    Texture   = <AlphaTex3>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler alpha4 = sampler_state
{
    Texture   = <AlphaTex4>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler alpha5 = sampler_state
{
    Texture   = <AlphaTex5>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};



//-----------------------------------------------------------------------------
//기본 텍스쳐 샘플링 설정.
//-----------------------------------------------------------------------------

sampler Sampler = sampler_state
{
    Texture   = <BaseTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU  = WRAP;
    AddressV  = WRAP;
};

//-----------------------------------------------------------------------------
//스플래팅 텍스쳐 샘플링 설정.
//-----------------------------------------------------------------------------

sampler splat0 = sampler_state
{
    Texture   = <SplatTex0>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU  = WRAP;
    AddressV  = WRAP;
};

sampler splat1 = sampler_state
{
    Texture   = <SplatTex1>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU  = WRAP;
    AddressV  = WRAP;
};

sampler splat2 = sampler_state
{
    Texture   = <SplatTex2>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU  = WRAP;
    AddressV  = WRAP;
};
sampler splat3 = sampler_state
{
    Texture   = <SplatTex3>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU  = WRAP;
    AddressV  = WRAP;
};

sampler splat4 = sampler_state
{
    Texture   = <SplatTex4>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU  = WRAP;
    AddressV  = WRAP;
};

sampler splat5 = sampler_state
{
    Texture   = <SplatTex5>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU  = WRAP;
    AddressV  = WRAP;
};

//-----------------------------------------------------------------------------
// 버텍스 INPUT, OUTPUT 선언
//-----------------------------------------------------------------------------

struct VS_INPUT
{
    float3 position	: POSITION;
    float3 normal   : NORMAL;
	float2 texture0 : TEXCOORD0;	//alpha
	float2 texture1 : TEXCOORD1;	//diffuse
};

struct SHADOW_PS_INPUT
{
	float4 mClipPosition: TEXCOORD;
};

//-----------------------------------------------------------------------------
// 픽셀쉐이더 쪽으로 넘길때 쓰임.
// NORMAL 대신 칼라 값으로 넘어간다.
// OUTPUT 포지션 값은 RHW같이 float4(x,y,z,w)로 해준다.
//-----------------------------------------------------------------------------

struct VS_OUTPUT
{
    float4 position : POSITION;
    float4 color	: COLOR0;
    float2 texture0 : TEXCOORD0;	//alpha
	float2 texture1 : TEXCOORD1;	//diffuse

	float4 mClipPosition : TEXCOORD2;		//진호
	float4 mClipPosition1 : TEXCOORD3;	//진호
	float4 mClipPosition2 : TEXCOORD4;	//진호

	float4 viewDistance : TEXCOORD5;	//진호
	
	float  Fog		: FOG;
};

struct SHADOW_VS_OUTPUT
{
	float4 position : POSITION;
	float4 mClipPosition : TEXCOORD;
};
//-----------------------------------------------------------------------------
// 픽셀셰이더 구조체.
//-----------------------------------------------------------------------------

struct PS_OUTPUT
{
	float4 color : COLOR0;
};


//-----------------------------------------------------------------------------
// 버텍스 셰이더
//-----------------------------------------------------------------------------
VS_OUTPUT mainVS( VS_INPUT IN )
{
    VS_OUTPUT OUT  = (VS_OUTPUT) 0;
    
	//position은 월드*뷰*프로젝션 행렬을 곱해줘야 제 위치에 그려진다.
	OUT.position = mul(float4(IN.position,1.0f),worldViewProj);
	
	//일단 색을 기본색으로..
	OUT.color =float4(1.0f,1.0f,1.0f,1.0f);
	
	//텍스쳐 UV값이 넘어간다.
	OUT.texture0 = IN.texture0;						//alpha
	//타일 만들기.
	OUT.texture1.x = IN.texture1.x * TileWidth;		//diffuse
	OUT.texture1.y = IN.texture1.y * TileHeight;		//diffuse
	
	float3 L = -m_LgtDir;
	float ReflectIntensity =  saturate(m_Sat_A * dot(IN.normal, L) + m_Sat_B);
	OUT.color = m_LgtDif * ReflectIntensity;

	//그림자
	float4 worldPosition = mul(float4(IN.position, 1.0f), gWorldMatrix);
	OUT.mClipPosition = mul(worldPosition, Mat_LightViewProj);
	OUT.mClipPosition1 = mul(worldPosition, Mat_LightViewProj1);
	OUT.mClipPosition2 = mul(worldPosition, Mat_LightViewProj2);
	OUT.viewDistance = mul(worldPosition, Mat_View);
 
	//---FOG 시작 
	if(fFog.z >= 1.0f)
	{
		float4		camPos;
		
		float fDensity	= fFog.x;
		float fFogEnd	= fFog.y;

		// 카메라 공간에서의 정점 위치 계산
		camPos = mul(worldPosition, Mat_View);

		float fDist = camPos.z / fFogEnd * 0.4;  
	 
		OUT.Fog =  exp(-fDist * fDensity);
	} else 
	{
		OUT.Fog = 1.0f;
	}

	//---FOG 끝  
	return OUT; 
}

//-----------------------------------------------------------------------------
// 픽셀 셰이더
//-----------------------------------------------------------------------------
PS_OUTPUT mainPS( VS_OUTPUT IN )
{
    PS_OUTPUT OUT = (PS_OUTPUT) 0;
    
    //버텍스 셰이더에서 normal값으로 color값을 구했다면 음영이 들어간다.
	float4 a0 = tex2D( alpha0,  IN.texture0 );
	float4 a1 = tex2D( alpha1,  IN.texture0 );
	float4 a2 = tex2D( alpha2,  IN.texture0 );
	float4 a3 = tex2D( alpha3,  IN.texture0 );
	float4 a4 = tex2D( alpha4,  IN.texture0 );
	float4 a5 = tex2D( alpha5,  IN.texture0 );
		
	float4 t0 = tex2D( splat0,  IN.texture1 );
	float4 t1 = tex2D( splat1,  IN.texture1 );
	float4 t2 = tex2D( splat2,  IN.texture1 );
	float4 t3 = tex2D( splat3,  IN.texture1 );
	float4 t4 = tex2D( splat4,  IN.texture1 );
	float4 t5 = tex2D( splat5,  IN.texture1 );
	
	float4 bs = tex2D( Sampler, IN.texture1 );
	
	OUT.color = lerp(bs,t0,a0);
	OUT.color = lerp(OUT.color,t1,a1);
	OUT.color = lerp(OUT.color,t2,a2);
	OUT.color = lerp(OUT.color,t3,a3);
	OUT.color = lerp(OUT.color,t4,a4);
	OUT.color = lerp(OUT.color,t5,a5) * IN.color;
	//--------진호
	
	float distance = IN.viewDistance.z;
	float SAMP_DX = 1.0f / 2048.0f;
	float s[4];
	float lightDepth;
	float2 shadowUV;

	if (distance < Frustum_far1)
	{
		lightDepth = IN.mClipPosition.z / IN.mClipPosition.w;
		shadowUV = IN.mClipPosition.xy / IN.mClipPosition.w;
		shadowUV.y = -shadowUV.y;
		shadowUV = (shadowUV * 0.5f) + 0.5f;

		s[0] = tex2D(ShadowSampler, shadowUV).r;
		s[1] = tex2D(ShadowSampler, shadowUV + float2(SAMP_DX, 0)).r;
		s[2] = tex2D(ShadowSampler, shadowUV + float2(0, SAMP_DX)).r;
		s[3] = tex2D(ShadowSampler, shadowUV + float2(SAMP_DX, SAMP_DX)).r;
	}
	else if (distance < Frustum_far2)
	{
		lightDepth = IN.mClipPosition1.z / IN.mClipPosition1.w;
		shadowUV = IN.mClipPosition1.xy / IN.mClipPosition1.w;
		shadowUV.y = -shadowUV.y;
		shadowUV = (shadowUV * 0.5f) + 0.5f;

		s[0] = tex2D(ShadowSampler1, shadowUV).r;
		s[1] = tex2D(ShadowSampler1, shadowUV + float2(SAMP_DX, 0)).r;
		s[2] = tex2D(ShadowSampler1, shadowUV + float2(0, SAMP_DX)).r;
		s[3] = tex2D(ShadowSampler1, shadowUV + float2(SAMP_DX, SAMP_DX)).r;
	}
	else
	{
		lightDepth = IN.mClipPosition2.z / IN.mClipPosition2.w;
		shadowUV = IN.mClipPosition2.xy / IN.mClipPosition2.w;
		shadowUV.y = -shadowUV.y;
		shadowUV = (shadowUV * 0.5f) + 0.5f;

		s[0] = tex2D(ShadowSampler2, shadowUV).r;
		s[1] = tex2D(ShadowSampler2, shadowUV + float2(SAMP_DX, 0)).r;
		s[2] = tex2D(ShadowSampler2, shadowUV + float2(0, SAMP_DX)).r;
		s[3] = tex2D(ShadowSampler2, shadowUV + float2(SAMP_DX, SAMP_DX)).r;
	}

	float result[4];

	for (int i = 0; i < 4; i++)
	{
		if ((lightDepth < 1.0f) &&
			(shadowUV.x <= 1.0f) && (shadowUV.x >= 0.0f) &&
			(shadowUV.y <= 1.0f) && (shadowUV.y >= 0.0f))
		{
			result[i] = lightDepth - 0.001f <= s[i];
		}
		else
		{
			result[i] = 1.0f;
		}
	}

	float2 texelPos = shadowUV * 2048.0f;
	float2 t = frac(texelPos);

	float resultLerp = lerp(lerp(result[0], result[1], t.x),
	lerp(result[2], result[3], t.x), t.y);

	resultLerp /= 2.0f;

	resultLerp += 0.5;


	float4 color = OUT.color;
	color = color * resultLerp;
	OUT.color = float4(color.rgb, 1.0f);
	
	//-------- Fog 
	OUT.color = lerp(float4(185/255.0f,187/255.0f,202/255.0f,1.0f),OUT.color * 1.0f,IN.Fog);
	//--------- Fog 

	return OUT;
}

//-----------------------------------------------------------------------------
// Create Shadow - VertexShader
//-----------------------------------------------------------------------------
SHADOW_VS_OUTPUT CreateShadowVS(VS_INPUT IN)
{
	SHADOW_VS_OUTPUT Out;

	float4 worldPosition = mul(float4(IN.position, 1.0f), gWorldMatrix);

	Out.position = mul(worldPosition, Mat_LightViewProj); 
	Out.mClipPosition = Out.position; 
	return Out;
}

//-----------------------------------------------------------------------------
// Create Shadow - PixelShader
//-----------------------------------------------------------------------------

float4 CreateShadowPS(SHADOW_PS_INPUT Input) : COLOR
{
	float depth = Input.mClipPosition.z / Input.mClipPosition.w;
	return float4(depth.xxx, 1.0f);
}

//-----------------------------------------------------------------------------
// Simple Effect (1 technique with 1 pass)
//-----------------------------------------------------------------------------
technique Tech
{
    pass Pass0
    {
		VertexShader = compile vs_3_0 mainVS();
		PixelShader  = compile ps_3_0 mainPS();
    }
}

technique CreateShadow
{
	pass Pass0
	{
		VertexShader = compile vs_2_0 CreateShadowVS();
		PixelShader = compile ps_2_0 CreateShadowPS();
	}
}

