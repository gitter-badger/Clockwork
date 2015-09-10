#if defined(PBR) && defined(DEFERRED)

    /// Convert an RGB color into YCoCg
    ///     color: input color to convert
    vec3 YCoCg_FromRGB(in vec3 color)
    {
        return vec3(
           0.25 * color.r + 0.5 * color.g + 0.25 * color.b, 
            0.5 * color.r - 0.5 * color.b + 0.5, 
            -0.25* color.r + 0.5 * color.g - 0.25 * color.b +0.5);
    }

    /// Convert a YCoCg encoded color into RGB
    ///     ycocg: The YCoCg encoded color to convert
    vec3 YCoCg_ToRGB(in vec3 ycocg)
    {
       ycocg.y -= 0.5;
        ycocg.z -= 0.5;
        return vec3(
            ycocg.r + ycocg.g - ycocg.b, 
            ycocg.r + ycocg.b, 
            ycocg.r - ycocg.g - ycocg.b);
    }
    
    /// Extract Albedo and specular components from interleaved
    ///     coords: screen coordinates of the interleaved pixel
    vec2 YCoCg_GetInterleaved(in vec2 coords)
    {
       // vec4 tex = texture2D(sAlbedoBuffer, coords + vec2(cGBufferInvSize.x,0.0f))
        return texture2D(sAlbedoBuffer, coords + vec2(cGBufferInvSize.x,0.0f)).zw;
    }
    
    /// Encodes the normal as Lambert Azimuth equal-area
    ///     normal: world space normal to pack
    ///     viewDir: viewing direction vector of the camera
    vec2 EncodeNormals(in vec3 normal, in vec3 viewDir)
    {    
        vec2 enc = normalize(normal.xy) * sqrt(-normal.z * 0.5f + 0.5f);
        return enc * 0.5f + 0.5f;
    }
   
    /// Decodes the normal from Lambert Azimuth equal-area packing
    ///     screenNormal: 2 channel packed normal from GBuffer
    ///     viewDir: viewing direction vector of the camera
    vec3 DecodeGBufferNormal(in vec2 screenNormal, in vec3 viewDir)
    {            
        vec4 nn = vec4(screenNormal, 0.0f, 0.0f) * vec4(2.0f,2.0f,0.0f,0.0f) + vec4(-1.0f,-1.0f,1.0f,-1.0f);
        float l = dot(nn.xyz,-nn.xyw);
        nn.z = l;
        nn.xy *= sqrt(l);
        return normalize(nn.xyz * 2 + vec3(0.0f,0.0f,-1.0f));
    }

    /// Utility function for writing into the GBuffer, Places all GBuffer writes into a single place
    ///     screenPos: screen position of the pixel
    ///     albedo: albedo
    ///     specular: specular color
    ///     wsNormals: surface normal in worldspace
    ///     depth: depth position of the pixel
    ///     roughness: roughness of the surface
    void WriteGBuffer(out vec4 oAlbedo, out vec4 oNormal, out vec4 oDepth, in vec3 viewDir, in vec2 screenPos, in vec4 albedo, in vec3 specular, in vec3 wsNormals, in float depth, in float roughness)
    {        
        // 2 channel normal
         oNormal.xy = EncodeNormals(wsNormals.xyz, viewDir.rgb);
         oNormal.z = roughness;
         oNormal.w = 0.0f;
        
        // Interleave Co/Cg in a checkboard pattern        
        vec3 ycocgSpec = YCoCg_FromRGB(specular); 
        vec3 ycocgAlbedo = YCoCg_FromRGB(albedo.rgb);
        
        float x = screenPos.x / cGBufferInvSize.x; // turn 0-1 into 0-width
        float y = screenPos.y / cGBufferInvSize.y; // turn 0-1 into 0-height
        bool pattern = mod(x, 2) == mod(y, 2);
        oAlbedo.r = ycocgAlbedo.r;
        oAlbedo.g = pattern ? ycocgAlbedo.b : ycocgAlbedo.g;
        oAlbedo.b = ycocgSpec.b;
        oAlbedo.a = pattern ? ycocgSpec.b : ycocgSpec.g;
        
        // Write depth
        oDepth = vec4(depth);
    }
    
    /// Extract YCoCg embedded abledo and specular 
    ///     screenPos: position in the screen
    ///     coded: the encoded colors RGBA
    ///     abledo: output for decoded albedo
    ///     specColor: output for decoded specular color
    void DecodeYCoCgElements(in vec2 screenPos, in vec4 coded, out vec3 albedo, out vec3 specColor)
    {
        vec2 interleaved = YCoCg_GetInterleaved(screenPos);
        vec3 ycocgAlbedo = vec3(coded.rg, interleaved.x);
        vec3 ycocgSpecular = vec3(coded.ba, interleaved.y);
        
        float x = screenPos.x ;
        float y = screenPos.y ;
        
        // swap channels if necessary
        bool pattern = mod(x, 2) == mod(y, 2);
        ycocgAlbedo.rgb = pattern ? ycocgAlbedo.rbg : ycocgAlbedo.rgb;
        ycocgSpecular.rgb = pattern ? ycocgSpecular.rbg : ycocgSpecular.rgb;
        
        albedo = YCoCg_ToRGB(ycocgAlbedo);
        specColor = YCoCg_ToRGB(ycocgSpecular);
    }
    
#endif