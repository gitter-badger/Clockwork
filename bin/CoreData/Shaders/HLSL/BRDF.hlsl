
//Diffuse
//1 = Lambert
//2 = Burley
//3 = OrenNayar

float3 GetLambertDiffuse(float3 DiffuseColor)
{
    return DiffuseColor * (1 / 3.14159265359);
}

float3 GetBurleyDiffuse(float3 DiffuseColor, float Roughness, float NdotV, float NdotL, float VdotH)
{
    float FD90 = ( 0.5  + 2 * VdotH * VdotH) * Roughness;
    
    float InvNdotV = 1 - NdotV;
    float NdotVPow5 = InvNdotV * InvNdotV;
    NdotVPow5 = NdotVPow5 * NdotVPow5 * InvNdotV;
    
    float InvNdotL = 1 - NdotL;
    float NdotLPow5 = InvNdotL * InvNdotL;
    NdotLPow5 = NdotLPow5 * NdotLPow5 * InvNdotL;
    
    float FdV = 1 + (FD90 - 1) * NdotVPow5;
    float FdL = 1 + (FD90 - 1) * NdotLPow5;
    
    return DiffuseColor * ( 1 / 3.14159 * FdV * FdL ) * ( 1 - 0.3333 * Roughness );
}

float3 GetOrenNayarDiffuse(float3 DiffuseColor, float Roughness, float NdotV, float NdotL, float VdotH)
{
    float VoL = 2 * VdotH -1;   
    float m = Roughness * Roughness;
    float m2 = m * m;
    float C1 = 1 - 0.5 * m2 / (m2 + 0.33);
	float Cosri = VoL - NdotV * NdotL;
	float C2 = 0.45 * m2 / (m2 + 0.09) * Cosri * ( Cosri >= 0 ? min( 1, NdotL / NdotV ) : NdotL );
	return DiffuseColor / 3.14159 * ( NdotL * C1 + C2 );
}


// Distrabution
//1 = GGX
//2 = Blinn
//3 = Beckmann


float GetGGXDistrabution(float Roughness, float NdotH)
{
    float m = Roughness * Roughness;
    float m2 = m * m;
    float d = (NdotH * m2 - NdotH) * NdotH + 1;
    return m2 / (3.14159  * d * d);
}

float GetBlinnDistrabution(float Roughness, float NdotH)
{
    float m = Roughness * Roughness;
    float m2 = m * m;
    float n = 2 / m2 - 2;
    return (n+2) / (2*3.14159) * pow(max(abs(NdotH),0.000001f),n);
}

float GetBeckmannDistrabution(float Roughness, float NdotH)
{
    float m = Roughness * Roughness;
	float m2 = m * m;
	float NdotH2 = NdotH * NdotH;
	return exp( (NdotH2 - 1) / (m2 * NdotH2) ) / ( 3.14159 * m2 * NdotH2 * NdotH2 );
	
}

// Geometric 
// 1: Implicit
// 2: Neumann
// 3: Kelemen
// 4: Schlick
// 5: Smith (matched to GGX)

float GetImplicitGeometric()
{
    return 0.25;
}

float GetNeumannGeometric(float NdotH, float NdotL)
{
    return 1 / (4 * max(NdotL, NdotH));
}

float GetKelemenGeometric(float VdotH)
{
    return rcp(4 * VdotH * VdotH);
}

float GetSchlickGeometric(float Roughness, float NdotV, float NdotL)
{
    float a = (Roughness * Roughness) * 0.5;
    float SchlickV = NdotV * (1 - a) + a;
    float SchlickL = NdotL * (1 - a) + a;
    return 0.25 / (SchlickV * SchlickL);
}

float GetSmithGeometric(float Roughness, float NdotV, float NdotL)
{
    float m = Roughness * Roughness;
    float m2 = m * m;
    
    float SmithV = NdotV + sqrt( NdotV * (NdotV - NdotV * m2) + m2 );
	float SmithL = NdotL + sqrt( NdotL * (NdotL - NdotL * m2) + m2 );
	return rcp( SmithV * SmithL );
}

// Fresnel 
// 1: None
// 2: Schlick
// 3: Fresnel

float3 GetNoneFresnel(float3 SpecularColor)
{
    return SpecularColor;
}

float3 GetSchlickFresnel(float3 SpecularColor, float VoH)
{
    float Fc = exp2( (-5.55473 * VoH - 6.98316) * VoH );	// 1 mad, 1 mul, 1 exp
	return Fc + (1 - Fc) * SpecularColor;	
}

float3 GetFresnelFresnel(float3 SpecularColor, float VdotH)
{
    float3 SpecularColorSqrt = sqrt( clamp( float3(0, 0, 0), float3(0.99, 0.99, 0.99), SpecularColor ) );
	float3 n = ( 1 + SpecularColorSqrt ) / ( 1 - SpecularColorSqrt );
	float3 g = sqrt( n*n + VdotH*VdotH - 1 );
	float3 a = (g - VdotH) / (g + VdotH);
	float3 a2 = a * a;
	float3 m = ((g+VdotH)*VdotH - 1) / ((g-VdotH)*VdotH + 1);
	float3 m2 = m * m;
	return 0.5 * a2 * ( 1 + m2 );
}

float3 GetBRDFDiffuse( float3 DiffuseColor, float Roughness, float NdotV, float NdotL, float VdotH, int diff)
{
    //Diffuse
    //1 = Lambert
    //2 = Burley
    //3 = OrenNayar
    
    [branch] switch(diff)
    {
    case 0:
        return GetLambertDiffuse(DiffuseColor);
    case 1:
        return GetLambertDiffuse(DiffuseColor); 
    case 2:
        return GetBurleyDiffuse(DiffuseColor, Roughness, NdotV, NdotL, VdotH);
    default:
        return GetOrenNayarDiffuse(DiffuseColor, Roughness, NdotV, NdotL, VdotH); 
    }
}

float GetBRDFDistrabution(float Roughness, float NdotH, int dis)
{
    // Distrabution
    //1 = GGX
    //2 = Blinn
    //3 = Beckmann
    
    [branch] switch(dis)
    {
    case 0:
        return GetGGXDistrabution(Roughness, NdotH);
    case 1:
        return GetGGXDistrabution(Roughness, NdotH);
    case 2:
        return GetBlinnDistrabution(Roughness, NdotH);
    default:
        return GetBeckmannDistrabution(Roughness, NdotH);
    }
}

float GetBRDFGeometricVisibility( float Roughness, float NdotV, float NdotL, float VdotH, int geo)
{
    // Geometric 
    // 1: Implicit
    // 2: Neumann
    // 3: Kelemen
    // 4: Schlick
    // 5: Smith (matched to GGX)
    
    [branch] switch(geo)
    {
    case 0:
        return GetImplicitGeometric();
    case 1:
        return GetImplicitGeometric();
    case 2:
        return GetNeumannGeometric(NdotV, NdotL);
    case 3:
        return GetKelemenGeometric(VdotH);
    case 4:
        return GetSchlickGeometric(Roughness, NdotV, NdotL);
    default:
        return GetSmithGeometric(Roughness, NdotV, NdotL);
    }
}

float3 GetBRDFFresnel( float3 SpecularColor, float VdotH, int fres)
{
    // Fresnel 
    // 1: None
    // 2: Schlick
    // 3: Fresnel
    
    [branch] switch(fres)
    {
    case 0:
        return GetNoneFresnel(SpecularColor);
    case 1:
        return GetNoneFresnel(SpecularColor);
    case 2:
        return GetSchlickFresnel(SpecularColor, VdotH);
    default:
        return GetFresnelFresnel(SpecularColor, VdotH);
    }
}