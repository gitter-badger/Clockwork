// Physically based shading model
// parameterized with the below options

// Microfacet specular = D*G*F / (4*NoL*NoV) = D*Vis*F
// Vis = G / (4*NoL*NoV)

// Diffuse model
// 0: Lambert
// 1: Burley
// 2: Oren-Nayar
#define PHYSICAL_DIFFUSE    1

// Microfacet distribution function
// 0: Blinn
// 1: Beckmann
// 2: GGX
#define PHYSICAL_SPEC_D     2 

// Geometric attenuation or shadowing
// 0: Implicit
// 1: Neumann
// 2: Kelemen
// 3: Schlick
// 4: Smith (matched to GGX)
#define PHYSICAL_SPEC_G     3

// Fresnel
// 0: None
// 1: Schlick
// 2: Fresnel
#define PHYSICAL_SPEC_F     1


float3 Diffuse_Lambert( float3 DiffuseColor )
{
    return DiffuseColor * (1 / 3.141596);
}

// [Burley 2012, "Physically-Based Shading at Disney"]
// [Lagrade et al. 2014, "Moving Frostbite to Physically Based Rendering"]
float3 Diffuse_Burley( float3 DiffuseColor, float Roughness, float NoV, float NoL, float VoH )
{
    float FD90 = ( 0.5 + 2 * VoH * VoH ) * Roughness;
    
    float InvNov = 1- NoV;
    float NoVPow5 = InvNov * InvNov;
    NoVPow5 = NoVPow5 * NoVPow5 * InvNov;

    float InvNoL = 1 - NoL;
    float NoLPow5 = InvNoL * InvNoL;
    NoLPow5 = NoLPow5 * NoLPow5 * InvNoL;

    float FdV = 1 + (FD90 - 1) * NoVPow5;
    float FdL = 1 + (FD90 - 1) * NoLPow5;
    return DiffuseColor * (( 1 / 3.141596) * FdV * FdL ) * ( 1 - 0.3333 * Roughness );  // TODO premultiply DiffuseColor
}

// [Gotanda 2012, "Beyond a Simple Physically Based Blinn-Phong Model in Real-Time"]
float3 Diffuse_OrenNayar( float3 DiffuseColor, float Roughness, float NoV, float NoL, float VoH )
{
    float VoL = 2 * VoH - 1;
    float m = Roughness * Roughness;
    float m2 = m * m;
    float C1 = 1 - 0.5 * m2 / (m2 + 0.33);
    float Cosri = VoL - NoV * NoL;
    float C2 = 0.45 * m2 / (m2 + 0.09) * Cosri * ( Cosri >= 0 ? min( 1, NoL / NoV ) : NoL );
    return DiffuseColor / 3.141596 * ( NoL * C1 + C2 );
}

// [Blinn 1977, "Models of light reflection for computer synthesized pictures"]
float D_Blinn( float Roughness, float NoH )
{
    float m = Roughness * Roughness;
    float m2 = m * m;
    float n = 2 / m2 - 2;
    return (n+2) / (2*3.141596) * pow( NoH, n );      // 1 mad, 1 exp, 1 mul, 1 log
}

// [Beckmann 1963, "The scattering of electromagnetic waves from rough surfaces"]
float D_Beckmann( float Roughness, float NoH )
{
    float m = Roughness * Roughness;
    float m2 = m * m;
    float NoH2 = NoH * NoH;
    return exp( (NoH2 - 1) / (m2 * NoH2) ) / ( 3.141596 * m2 * NoH2 * NoH2 );
}

// GGX / Trowbridge-Reitz
// [Walter et al. 2007, "Microfacet models for refraction through rough surfaces"]
float D_GGX( float Roughness, float NoH )
{
    float m = Roughness * Roughness;
    float m2 = m * m;
    float d = ( NoH * m2 - NoH ) * NoH + 1; // 2 mad
    return m2 / ( 3.141596*d*d );                 // 4 mul, 1 rcp
}

// Anisotropic GGX
// [Burley 2012, "Physically-Based Shading at Disney"]
float D_GGXaniso( float RoughnessX, float RoughnessY, float NoH, float3 H, float3 X, float3 Y )
{
    float mx = RoughnessX * RoughnessX;
    float my = RoughnessY * RoughnessY;
    float XoH = dot( X, H );
    float YoH = dot( Y, H );
    float d = XoH*XoH / (mx*mx) + YoH*YoH / (my*my) + NoH*NoH;
    return 1 / ( 3.141596 * mx*my * d*d );
}

float Vis_Implicit()
{
    return 0.25;
}

// [Neumann et al. 1999, "Compact metallic reflectance models"]
float Vis_Neumann( float NoV, float NoL )
{
    return 1 / ( 4 * max( NoL, NoV ) );
}

// [Kelemen 2001, "A microfacet based coupled specular-matte brdf model with importance sampling"]
float Vis_Kelemen( float VoH )
{
    return rcp( 4 * VoH * VoH );
}

// Tuned to match behavior of Vis_Smith
// [Schlick 1994, "An Inexpensive BRDF Model for Physically-Based Rendering"]
float Vis_Schlick( float Roughness, float NoV, float NoL )
{
    float k = (Roughness * Roughness) * 0.5;
    float Vis_SchlickV = NoV * (1 - k) + k;
    float Vis_SchlickL = NoL * (1 - k) + k;
    return 0.25 / ( Vis_SchlickV * Vis_SchlickL );
}

// Smith term for GGX
// [Smith 1967, "Geometrical shadowing of a random rough surface"]
float Vis_Smith( float Roughness, float NoV, float NoL )
{
    float a = Roughness * Roughness;
    float a2 = a*a;

    float Vis_SmithV = NoV + sqrt( NoV * (NoV - NoV * a2) + a2 );
    float Vis_SmithL = NoL + sqrt( NoL * (NoL - NoL * a2) + a2 );
    return rcp( Vis_SmithV * Vis_SmithL );
}

// Appoximation of joint Smith term for GGX
// [Heitz 2014, "Understanding the Masking-Shadowing Function in Microfacet-Based BRDFs"]
float Vis_SmithJointApprox( float Roughness, float NoV, float NoL )
{
    float a = Roughness * Roughness;
    float Vis_SmithV = NoL * ( NoV * ( 1 - a ) + a );
    float Vis_SmithL = NoV * ( NoL * ( 1 - a ) + a );
    return 0.5 * rcp( Vis_SmithV + Vis_SmithL );
}

float3 F_None( float3 SpecularColor )
{
    return SpecularColor;
}

// [Schlick 1994, "An Inexpensive BRDF Model for Physically-Based Rendering"]
// [Lagarde 2012, "Spherical Gaussian approximation for Blinn-Phong, Phong and Fresnel"]
float3 F_Schlick( float3 SpecularColor, float VoH )
{
    float Fc = pow( 1 - VoH, 5 );                           // 1 sub, 3 mul
    //float Fc = exp2( (-5.55473 * VoH - 6.98316) * VoH );  // 1 mad, 1 mul, 1 exp
    //return Fc + (1 - Fc) * SpecularColor;                 // 1 add, 3 mad
    
    // Anything less than 2% is physically impossible and is instead considered to be shadowing
    return saturate( 50.0 * SpecularColor.g ) * Fc + (1 - Fc) * SpecularColor;
    
}

float3 F_Fresnel( float3 SpecularColor, float VoH )
{
    float3 SpecularColorSqrt = sqrt( clamp( float3(0, 0, 0), float3(0.99, 0.99, 0.99), SpecularColor ) );
    float3 n = ( 1 + SpecularColorSqrt ) / ( 1 - SpecularColorSqrt );
    float3 g = sqrt( n*n + VoH*VoH - 1 );
    return 0.5 * (g - VoH) / (g + VoH) * (g - VoH) / (g + VoH) * ( 1 + ((g+VoH)*VoH - 1) / ((g-VoH)*VoH + 1) * ((g+VoH)*VoH - 1) / ((g-VoH)*VoH + 1) );
}


float3 Diffuse( float3 DiffuseColor, float Roughness, float NoV, float NoL, float VoH )
{
#if   PHYSICAL_DIFFUSE == 0
    return Diffuse_Lambert( DiffuseColor );
#elif PHYSICAL_DIFFUSE == 1
    return Diffuse_Burley( DiffuseColor, Roughness, NoV, NoL, VoH );
#elif PHYSICAL_DIFFUSE == 2
    return Diffuse_OrenNayar( DiffuseColor, Roughness, NoV, NoL, VoH );
#endif
}

float Distribution( float Roughness, float NoH )
{
#if   PHYSICAL_SPEC_D == 0
    return D_Blinn( Roughness, NoH );
#elif PHYSICAL_SPEC_D == 1
    return D_Beckmann( Roughness, NoH );
#elif PHYSICAL_SPEC_D == 2
    return D_GGX( Roughness, NoH );
#endif
}

// Vis = G / (4*NoL*NoV)
float GeometricVisibility( float Roughness, float NoV, float NoL, float VoH )
{
#if   PHYSICAL_SPEC_G == 0
    return Vis_Implicit();
#elif PHYSICAL_SPEC_G == 1
    return Vis_Neumann( NoV, NoL );
#elif PHYSICAL_SPEC_G == 2
    return Vis_Kelemen( VoH );
#elif PHYSICAL_SPEC_G == 3
    return Vis_Schlick( Roughness, NoV, NoL );
#elif PHYSICAL_SPEC_G == 4
    return Vis_Smith( Roughness, NoV, NoL );
#endif
}

float3 Fresnel( float3 SpecularColor, float VoH )
{
#if   PHYSICAL_SPEC_F == 0
    return F_None( SpecularColor );
#elif PHYSICAL_SPEC_F == 1
    return F_Schlick( SpecularColor, VoH );
#elif PHYSICAL_SPEC_F == 2
    return F_Fresnel( SpecularColor, VoH );
#endif
}

