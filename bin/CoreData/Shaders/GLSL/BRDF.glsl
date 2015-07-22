vec3 DiffuseLambert(vec3 DiffuseColor)
{
	return DiffuseColor * (1/ 3.14159);
}

// [Burley 2012, "Physically-Based Shading at Disney"]
// [Lagrade et al. 2014, "Moving Frostbite to Physically Based Rendering"]
vec3 DiffuseBurley(vec3 DiffuseColor, float Roughness, float NdV, float NdL, float VdH)
{
	float FD90 = ( 0.5 + 2 * VdH * VdH) * Roughness;

	float InvNdV = 1-NdV;
	float NdVPow5 = InvNdV * InvNdV;
	NdVPow5 = NdVPow5 * NdVPow5 * InvNdV;

	float InvNdL = 1-NdL;
	float NdLPow5 = InvNdL * InvNdL;
	NdLPow5 = NdLPow5 * NdLPow5 * InvNdL;

	float FdV = 1 + (FD90 - 1) * NdVPow5;
	float FdL = 1 + (FD90 - 1) * NdLPow5;

	return DiffuseColor * (1 /3.14159 * FdV * FdL) * (1 - 0.3333 * Roughness);
}

// [Gotanda 2012, "Beyond a Simple Physically Based Blinn-Phong Model in Real-Time"]
vec3 DiffuseOrenNayar(vec3 DiffuseColor, float Roughness, float NdV, float NdL, float VdH)
{
	float VdL = 2 * VdH - 1;
	float m = Roughness * Roughness;
	float m2 = m * m;
	float C1 = 1 - 0.5 * m2 / (m2 + 0.33);
	float Cosri = VdL - NdV * NdL;
	float C2 = 0.45 * m2 / (m2 + 0.09) * Cosri * ( Cosri >= 0 ? min( 1, NdL / NdV ) : NdL );
	return DiffuseColor / 3.14159 * ( NdL * C1 + C2 );
}

vec3 GetBRDFDiffuse(int type, vec3 DiffuseColor, float Roughness, float NdV, float NdL, float VdH)
{
	if(type == 0)
	{
		return DiffuseLambert(DiffuseColor);
	}
	else if(type == 1)
	{
		return DiffuseBurley(DiffuseColor, Roughness, NdV, NdL, VdH);
	}
	else
	{
		return DiffuseOrenNayar(DiffuseColor, Roughness, NdV, NdL, VdH);
	}
}

// GGX / Trowbridge-Reitz
// [Walter et al. 2007, "Microfacet models for refraction through rough surfaces"]
float DistGGX( float Roughness, float NdH )
{
	float m = Roughness * Roughness;
	float m2 = m * m;
	float d = ( NdH * m2 - NdH ) * NdH + 1;	
	return m2 / ( 3.14159*d*d );
}

float VisSchlick( float Roughness, float NdV, float NdL )
{
	float k = Roughness * Roughness * 0.5;
	float Vis_SchlickV = NdV * (1 - k) + k;
	float Vis_SchlickL = NdL * (1 - k) + k;
	return 0.25 / ( Vis_SchlickV * Vis_SchlickL );
}

// [Schlick 1994, "An Inexpensive BRDF Model for Physically-Based Rendering"]
// [Lagarde 2012, "Spherical Gaussian approximation for Blinn-Phong, Phong and Fresnel"]
vec3 FresSchlick( vec3 SpecularColor, float VdH )
{
	//float Fc = pow( 1 - VoH, 5 );							// 1 sub, 3 mul
	float Fc = exp2( (-5.55473 * VdH - 6.98316) * VdH );	// 1 mad, 1 mul, 1 exp
	return Fc + (1 - Fc) * SpecularColor;					// 1 add, 3 mad
	
	// Anything less than 2% is physically impossible and is instead considered to be shadowing
	//return saturate( 50.0 * SpecularColor.g ) * Fc + (1 - Fc) * SpecularColor;
	
}