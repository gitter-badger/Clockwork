/// Settings
String cubeMapGen_Name;
String cubeMapGen_Path;
int cubeMapGen_Size;

Array<EditorCubeCapture@> activeCubeCapture; // Editor capture tasks in progress
Array<Zone@> cloneZones; // Duplicate zones constructed to prevent IBL doubling
Array<Zone@> disabledZones; // Zones that were disabled to prevent IBL doubling
String cubemapOutputPath = "Data/Textures/Cubemaps";

void PrepareZonesForCubeRendering()
{
    // Only clone zones when we aren't actively processing
    if (cloneZones.length > 0)
        return;
        
    Array<Component@>@ zones = editorScene.GetComponents("Zone", true);
    for (int i = 0; i < zones.length; ++i)
    {
        Zone@ srcZone = cast<Zone>(zones[i]);
        if (zones[i].enabled)
        {
            Zone@ cloneZone = srcZone.node.CreateComponent("Zone");
            cloneZone.zoneMask = srcZone.zoneMask;
            cloneZone.priority = srcZone.priority;
            cloneZone.boundingBox = srcZone.boundingBox;
            
            cloneZone.ambientColor = srcZone.ambientColor;
            cloneZone.ambientGradient = srcZone.ambientGradient;
            
            cloneZone.fogColor = srcZone.fogColor;
            cloneZone.fogStart = srcZone.fogStart;
            cloneZone.fogEnd = srcZone.fogEnd;
            cloneZone.fogHeight = srcZone.fogHeight;
            cloneZone.fogHeightScale = srcZone.fogHeightScale;
            cloneZone.heightFog = srcZone.heightFog;
            
            srcZone.enabled = false;
            
            // Add the zones to our temporary lists
            cloneZones.Push(cloneZone);
            disabledZones.Push(srcZone);
        }
    }
}

void UnprepareZonesForCubeRendering()
{
    // Clean up the clones
    for (int i = 0; i < cloneZones.length; ++i)
        cloneZones[i].Remove();
    cloneZones.Clear();
        
    // Reenable anyone we disabled
    for (int i = 0; i < disabledZones.length; ++i)
        disabledZones[i].enabled = true;
    disabledZones.Clear();
}

class EditorCubeCapture : ScriptObject // script object in order to get events
{
    private String name_;
    private String path_;
    private Node@ camNode_;
    private Camera@ camera_;
    private Zone@ target_;
    private Viewport@ viewport_;
    private Texture2D@ renderImage_;
    private RenderSurface@ renderSurface_;
    int updateCycle_;
    String imagePath_;
    
    EditorCubeCapture(Zone@ forZone)
    {
        PrepareZonesForCubeRendering();
        
        // Store name and path because if we have a lot of zones it could take long enough to queue another go
        name_ = cubeMapGen_Name;
        path_ = cubeMapGen_Path;
    
        updateCycle_ = 0;
    
        target_ = forZone;
    
        camNode_ = scene.CreateChild("RenderCamera");
        camera_ = camNode_.GetOrCreateComponent("Camera");
        camera_.fov = 90.0f;
        camera_.nearClip = 0.0001f;
        //camera_.orthographic = true;
        camera_.aspectRatio = 1.0f;
        camNode_.worldPosition = forZone.node.worldPosition;
        camNode_.worldRotation = forZone.node.worldRotation;
        camNode_.worldDirection = forZone.node.worldDirection;
        
        viewport_ = Viewport(scene, camera_);
        viewport_.renderPath = renderer.viewports[0].renderPath;
        
        renderImage_ = Texture2D();
        renderImage_.SetSize(cubeMapGen_Size, cubeMapGen_Size, GetRGBAFormat(), TEXTURE_RENDERTARGET);
        
        renderSurface_ = renderImage_.renderSurface;
        renderSurface_.viewports[0] = viewport_;
        renderSurface_.updateMode = SURFACE_UPDATEALWAYS;
        
        updateCycle_ = 0;
    }
    
    ~EditorCubeCapture()
    {
        camNode_.Remove();
    }
    
    void Start()
    {
        SubscribeToEvent("BeginFrame", "HandlePreRender");
        SubscribeToEvent("EndFrame", "HandlePostRender");
        
        // Hide grid and debugIcons until bake 
        if (grid !is null)
            grid.viewMask = 0;
        if (debugIconsNode !is null)
            debugIconsNode.enabled = false;
        
    }
    
    private void Stop()
    {
        camNode_.Remove();
        camNode_ = null;
        viewport_ = null;
        renderSurface_ = null;
        
        UnsubscribeFromEvent("BeginFrame");
        UnsubscribeFromEvent("EndFrame");
        
        WriteXML();
        
        // Remove ourselves from the processing list and if necessary clean things up
        activeCubeCapture.Erase(activeCubeCapture.FindByRef(this));
        if (activeCubeCapture.length == 0)
            UnprepareZonesForCubeRendering();
        else
            activeCubeCapture[0].Start();
            
        // Show grid and debug icons    
        if (grid !is null)
            grid.viewMask = 0x80000000;
        if (debugIconsNode !is null)
            debugIconsNode.enabled = true;
    }
    
    // Position camera accordingly
    void HandlePreRender(StringHash eventType, VariantMap& eventData)
    {
        if (camNode_ !is null)
        {
            if (updateCycle_ <= 5) 
            {
                camNode_.worldRotation = target_.node.worldRotation;
                camNode_.worldDirection = target_.node.worldDirection;
                camNode_.worldRotation = RotationOf(GetFaceForCycle(updateCycle_));
            }
            else
                Stop();
                
            updateCycle_++;
        }
    }
    
    // Save our image
    void HandlePostRender(StringHash eventType, VariantMap& eventData)
    {
        Image@ img = renderImage_.GetImage();
        String sceneName = editorScene.name.length > 0 ? editorScene.name + "/" : "";
        String path = path_ + "/" + sceneName;
        fileSystem.CreateDir(path);
        path = path + "/" + String(target_.id) + "_" + GetFaceName(GetFaceForCycle(updateCycle_)) + ".png";
        img.SavePNG(path);
    }
    
    private void WriteXML()
    {
        String sceneName = editorScene.name.length > 0 ? editorScene.name + "/" : "";
        String basePath = path_ + "/" + sceneName;
        String cubeName = name_.length > 0 ? (name_ + "_") : "";
        String xmlPath = basePath + "/" + name_ + String(target_.id) + ".xml";
        XMLFile@ file = XMLFile();
        XMLElement rootElem = file.CreateRoot("cubemap");
        
        XMLElement posXElem = rootElem.CreateChild("face");
        posXElem.SetAttribute("name", basePath + "/" + cubeName + String(target_.id) + "_" + GetFaceFileName(FACE_POSITIVE_X) + ".png");
        
        XMLElement negXElem = rootElem.CreateChild("face");
        negXElem.SetAttribute("name", basePath + "/" + cubeName + String(target_.id) + "_" + GetFaceFileName(FACE_NEGATIVE_X) + ".png");
        
        XMLElement posYElem = rootElem.CreateChild("face");
        posYElem.SetAttribute("name", basePath + "/" + cubeName + String(target_.id) + "_" + GetFaceFileName(FACE_POSITIVE_Y) + ".png");
        
        XMLElement negYElem = rootElem.CreateChild("face");
        negYElem.SetAttribute("name", basePath + "/" + cubeName + String(target_.id) + "_" + GetFaceFileName(FACE_NEGATIVE_Y) + ".png");
        
        XMLElement posZElem = rootElem.CreateChild("face");
        posZElem.SetAttribute("name", basePath + "/" + cubeName + String(target_.id) + "_" + GetFaceFileName(FACE_POSITIVE_Z) + ".png");
        
        XMLElement negZElem = rootElem.CreateChild("face");
        negZElem.SetAttribute("name", basePath + "/" + cubeName + String(target_.id) + "_" + GetFaceFileName(FACE_NEGATIVE_Z) + ".png");
        
        file.Save(File(xmlPath, FILE_WRITE), "    ");
        
        ResourceRef ref;
        ref.type = StringHash("TextureCube");
        ref.name = xmlPath;
        target_.SetAttribute("Zone Texture", Variant(ref));
    }
    
    private CubeMapFace GetFaceForCycle(int cycle)
    {
        switch (cycle)
        {
        case 0:
            return FACE_POSITIVE_X;
        case 1:                                                
            return FACE_POSITIVE_Y;                                   
        case 2:                                                
            return FACE_POSITIVE_Z;
        case 3:
            return FACE_NEGATIVE_X;
        case 4:
            return FACE_NEGATIVE_Y;
        case 5:
            return FACE_NEGATIVE_Z;
        }
        return FACE_POSITIVE_X;
    }
    
    private String GetFaceName(CubeMapFace face)
    {
        switch (face)
        {
        case 0:
            return "PosX";
        case 1:                                  
            return "PosY";                                   
        case 2:                                  
            return "PosZ";
        case 3:
            return "NegX";
        case 4:
            return "NegY";
        case 5:
            return "NegZ";
        }
        return "None";
    }
    
    private String GetFaceFileName(CubeMapFace face)
    {
        switch (face)
        {
        case 0:
            return "PosX";
        case 1:                                  
            return "NegX";                                   
        case 2:                                  
            return "PosY";
        case 3:
            return "NegY";
        case 4:
            return "PosZ";
        case 5:
            return "NegZ";
        }
        return "None";
    }
    
    private Quaternion RotationOf(CubeMapFace face)
    {
        Quaternion result;
        switch (face)
        {
            //  Rotate camera according to probe rotation
            case FACE_POSITIVE_X:
                result = Quaternion(0.0f, 0.0f, 0.0f);
                break;
            case FACE_NEGATIVE_X:
                result = Quaternion(0.0f, -90.0f, 0.0f); 
                break;
            case FACE_POSITIVE_Y:
                result = Quaternion(90.0f, 0.0f, 0.0f); 
                break;
            case FACE_NEGATIVE_Y:
                result = Quaternion(0.0f, 180.0f, 0.0f); 
                break;
            case FACE_POSITIVE_Z:
                result = Quaternion(-90.0f, 0.0f, 0.0f);
                break;
            case FACE_NEGATIVE_Z:
                result = Quaternion(0.0f, 90.0f, 0.0f); 
                break;
        }
        return result;
    }
}