package clockwork;

@:native("Clockwork")
extern enum PlatformID {
    PLATFORMID_UNDEFINED;
    PLATFORMID_WINDOWS;
    PLATFORMID_MAC;
    PLATFORMID_ANDROID;
    PLATFORMID_IOS;
    PLATFORMID_WEB;
}
//Clockwork Haxe Definitions

extern class ToolCore {


    public static var PROJECTFILE_VERSION: Int;


}

//----------------------------------------------------
// MODULE: ToolCore
//----------------------------------------------------


@:native("Clockwork.ToolEnvironment")
extern class ToolEnvironment extends AObject {

    var rootSourceDir: String;
    var toolPrefs: ToolPrefs;
    var rootBuildDir: String;
    var editorBinary: String;
    var playerBinary: String;
    var toolBinary: String;
    var coreDataDir: String;
    var playerDataDir: String;
    var editorDataDir: String;
    var nETCoreCLRAbsPath: String;
    var nETAssemblyLoadPaths: String;
    var nETTPAPaths: String;
    var clockworkNETEngineAssemblyPath: String;
    var deploymentDataDir: String;
    var toolDataDir: String;
    var devConfigFilename: String;
    var playerAppFolder: String;
    var iOSDeployBinary: String;

    function new();

    function initFromPackage(): Bool;
    function initFromJSON(?clockworkTool: Bool): Bool;
      // Root source and build directories for development source tree builds
    function setRootSourceDir(sourceDir: String): Void;
    function setRootBuildDir(buildDir: String, ?setBinaryPaths: Bool): Void;
    function getToolPrefs(): ToolPrefs;
    function saveToolPrefs(): Void;
    function loadToolPrefs(): Void;
    function getRootSourceDir(): String;
    function getRootBuildDir(): String;
      // Binaries
    function getEditorBinary(): String;
    function getPlayerBinary(): String;
    function getToolBinary(): String;
      // Resource directories
    function getCoreDataDir(): String;
    function getPlayerDataDir(): String;
    function getEditorDataDir(): String;
      // ClockworkNET
    function getNETCoreCLRAbsPath(): String;
    function getNETAssemblyLoadPaths(): String;
    function getNETTPAPaths(): String;
    function getClockworkNETEngineAssemblyPath(): String;
      // Data directories
    function getDeploymentDataDir(): String;
    function getToolDataDir(): String;
    function getDevConfigFilename(): String;
    function getPlayerAppFolder(): String;
    function getIOSDeployBinary(): String;
    function dump(): Void;

}

@:native("Clockwork.ToolPrefs")
extern class ToolPrefs extends AObject {

    var androidSDKPath: String;
    var jDKRootPath: String;
    var antPath: String;
    var prefsPath: String;

    function new();

    function getAndroidSDKPath(): String;
    function getJDKRootPath(): String;
    function getAntPath(): String;
    function setAndroidSDKPath(path: String): Void;
    function setJDKRootPath(path: String): Void;
    function setAntPath(path: String): Void;
    function getPrefsPath(): String;
    function load(): Void;
    function save(): Void;

}

@:native("Clockwork.ToolSystem")
extern class ToolSystem extends AObject {

    var project: Project;
    var dataPath: String;
    var currentPlatform: Platform;

    function new();

    function loadProject(fullpath: String): Bool;
    function getProject(): Project;
    function closeProject(): Void;
    function getDataPath(): String;
    function setDataPath(path: String): Void;
    function registerPlatform(platform: Platform): Void;
    function getPlatformByID(platform: PlatformID): Platform;
    function getPlatformByName(name: String): Platform;
    function setCurrentPlatform(platform: PlatformID): Void;
    function getCurrentPlatform(): Platform;
    function setCLI(): Void;
    function isCLI(): Bool;

}

@:native("Clockwork.Project")
extern class Project extends AObject {

    var resourcePath: String;
    var componentsPath: String;
    var scriptsPath: String;
    var modulesPath: String;
    var buildSettings: ProjectBuildSettings;
    var userPrefs: ProjectUserPrefs;
    var projectPath: String;
    var projectFilePath: String;
    var userPrefsFullPath: String;
    var buildSettingsFullPath: String;
    var version: String;

      // Construct.
    function new();

    function load(fullpath: String): Bool;
    function save(?fullpath: String): Void;
      // Paths
    function getResourcePath(): String;
    function setResourcePath(resourcePath: String): Void;
    function getComponentsPath(): String;
    function getScriptsPath(): String;
    function getModulesPath(): String;
    function isComponentsDirOrFile(fullPath: String): Bool;
    function isScriptsDirOrFile(fullPath: String): Bool;
    function isModulesDirOrFile(fullPath: String): Bool;
    function addPlatform(platformID: PlatformID): Void;
    function containsPlatform(platformID: PlatformID): Bool;
    function removePlatform(platformID: PlatformID): Void;
    function isDirty(): Bool;
    function setDirty(): Void;
    function getBuildSettings(): ProjectBuildSettings;
    function getUserPrefs(): ProjectUserPrefs;
    function getProjectPath(): String;
    function getProjectFilePath(): String;
    function getUserPrefsFullPath(): String;
    function getBuildSettingsFullPath(): String;
    function getVersion(): String;
    function setVersion(version: String): Void;
    function saveBuildSettings(): Void;
    function loadBuildSettings(): Bool;
    function saveUserPrefs(): Void;
    function loadUserPrefs(): Bool;

}

@:native("Clockwork.MacBuildSettings")
extern class MacBuildSettings extends RefCounted {

    var appName: String;
    var packageName: String;
    var companyName: String;
    var productName: String;

    function new();

    function getAppName(): String;
    function getPackageName(): String;
    function getCompanyName(): String;
    function getProductName(): String;
    function setAppName(name: String): Void;
    function setPackageName(packageName: String): Void;
    function setCompanyName(companyName: String): Void;
    function setProductName(productName: String): Void;

}

@:native("Clockwork.WebBuildSettings")
extern class WebBuildSettings extends RefCounted {

    var appName: String;
    var packageName: String;
    var companyName: String;
    var productName: String;

    function new();

    function getAppName(): String;
    function getPackageName(): String;
    function getCompanyName(): String;
    function getProductName(): String;
    function setAppName(name: String): Void;
    function setPackageName(packageName: String): Void;
    function setCompanyName(companyName: String): Void;
    function setProductName(productName: String): Void;

}

@:native("Clockwork.WindowsBuildSettings")
extern class WindowsBuildSettings extends RefCounted {

    var appName: String;
    var packageName: String;
    var companyName: String;
    var productName: String;

    function new();

    function getAppName(): String;
    function getPackageName(): String;
    function getCompanyName(): String;
    function getProductName(): String;
    function setAppName(name: String): Void;
    function setPackageName(packageName: String): Void;
    function setCompanyName(companyName: String): Void;
    function setProductName(productName: String): Void;

}

@:native("Clockwork.AndroidBuildSettings")
extern class AndroidBuildSettings extends RefCounted {

    var appName: String;
    var packageName: String;
    var companyName: String;
    var productName: String;
    var sDKVersion: String;
    var minSDKVersion: String;
    var activityName: String;

    function new();

    function getAppName(): String;
    function getPackageName(): String;
    function getCompanyName(): String;
    function getProductName(): String;
    function getSDKVersion(): String;
    function getMinSDKVersion(): String;
    function getActivityName(): String;
    function setAppName(name: String): Void;
    function setPackageName(packageName: String): Void;
    function setCompanyName(companyName: String): Void;
    function setProductName(productName: String): Void;
    function setSDKVersion(value: String): Void;
    function setMinSDKVersion(value: String): Void;
    function setActivityName(value: String): Void;

}

@:native("Clockwork.IOSBuildSettings")
extern class IOSBuildSettings extends RefCounted {

    var appName: String;
    var packageName: String;
    var companyName: String;
    var productName: String;
    var provisionFile: String;
    var appIDPrefix: String;

    function new();

    function getAppName(): String;
    function getPackageName(): String;
    function getCompanyName(): String;
    function getProductName(): String;
    function getProvisionFile(): String;
    function getAppIDPrefix(): String;
    function setAppName(name: String): Void;
    function setPackageName(packageName: String): Void;
    function setCompanyName(companyName: String): Void;
    function setProductName(productName: String): Void;
    function setProvisionFile(value: String): Void;
    function setAppIDPrefix(value: String): Void;

}

@:native("Clockwork.ProjectBuildSettings")
extern class ProjectBuildSettings extends AObject {

    var macBuildSettings: MacBuildSettings;
    var windowsBuildSettings: WindowsBuildSettings;
    var webBuildSettings: WebBuildSettings;
    var androidBuildSettings: AndroidBuildSettings;
    var iOSBuildSettings: IOSBuildSettings;

      // Construct.
    function new();

    function getMacBuildSettings(): MacBuildSettings;
    function getWindowsBuildSettings(): WindowsBuildSettings;
    function getWebBuildSettings(): WebBuildSettings;
    function getAndroidBuildSettings(): AndroidBuildSettings;
    function getIOSBuildSettings(): IOSBuildSettings;
    function load(path: String): Bool;
    function save(path: String): Void;

}

@:native("Clockwork.ProjectFile")
extern class ProjectFile extends AObject {

      // Construct.
    function new();

    function save(project: Project): Void;
    function load(project: Project): Bool;
    function writeNewProject(fullpath: String): Void;

}

@:native("Clockwork.ProjectUserPrefs")
extern class ProjectUserPrefs extends AObject {

    var defaultPlatform: PlatformID;
    var lastBuildPath: String;
    var snapTranslationX: Float;
    var snapTranslationY: Float;
    var snapTranslationZ: Float;
    var snapRotation: Float;
    var snapScale: Float;

      // Construct.
    function new();

    function getDefaultPlatform(): PlatformID;
    function getLastBuildPath(): String;
    function setLastBuildPath(path: String): Void;
    function getSnapTranslationX(): Float;
    function getSnapTranslationY(): Float;
    function getSnapTranslationZ(): Float;
    function getSnapRotation(): Float;
    function getSnapScale(): Float;
    function setSnapTranslationX(value: Float): Void;
    function setSnapTranslationY(value: Float): Void;
    function setSnapTranslationZ(value: Float): Void;
    function setSnapRotation(value: Float): Void;
    function setSnapScale(value: Float): Void;

}

@:native("Clockwork.Platform")
extern class Platform extends AObject {

    var license: Bool;
    var name: String;
    var platformID: PlatformID;

    function new();

    function getLicense(): Bool;
    function getName(): String;
    function getPlatformID(): PlatformID;
    function newBuild(project: Project): BuildBase;

}

@:native("Clockwork.PlatformAndroid")
extern class PlatformAndroid extends Platform {

    var androidCommand: String;
    var aDBCommand: String;
    var androidTargets: Array<String>;

    function new();

    @:overload(function(): Bool{})
    override function getLicense(): Bool;
    @:overload(function(): String{})
    override function getName(): String;
    @:overload(function(): PlatformID{})
    override function getPlatformID(): PlatformID;
    function getAndroidCommand(): String;
    function getADBCommand(): String;
    function refreshAndroidTargets(): Void;
    function getAndroidTargets(): Array<String>;
    @:overload(function(project: Project): BuildBase{})
    override function newBuild(project: Project): BuildBase;

}

@:native("Clockwork.PlatformIOS")
extern class PlatformIOS extends Platform {


    function new();

    @:overload(function(): String{})
    override function getName(): String;
    @:overload(function(): PlatformID{})
    override function getPlatformID(): PlatformID;
    @:overload(function(project: Project): BuildBase{})
    override function newBuild(project: Project): BuildBase;
    function parseProvisionAppIdentifierPrefix(provisionFile: String): String;
    @:overload(function(): Bool{})
    override function getLicense(): Bool;

}

@:native("Clockwork.PlatformMac")
extern class PlatformMac extends Platform {


    function new();

    @:overload(function(): String{})
    override function getName(): String;
    @:overload(function(): PlatformID{})
    override function getPlatformID(): PlatformID;
    @:overload(function(project: Project): BuildBase{})
    override function newBuild(project: Project): BuildBase;
    @:overload(function(): Bool{})
    override function getLicense(): Bool;

}

@:native("Clockwork.PlatformWeb")
extern class PlatformWeb extends Platform {


    function new();

    @:overload(function(): String{})
    override function getName(): String;
    @:overload(function(): PlatformID{})
    override function getPlatformID(): PlatformID;
    @:overload(function(project: Project): BuildBase{})
    override function newBuild(project: Project): BuildBase;
    @:overload(function(): Bool{})
    override function getLicense(): Bool;

}

@:native("Clockwork.PlatformWindows")
extern class PlatformWindows extends Platform {


    function new();

    @:overload(function(): String{})
    override function getName(): String;
    @:overload(function(): PlatformID{})
    override function getPlatformID(): PlatformID;
    @:overload(function(project: Project): BuildBase{})
    override function newBuild(project: Project): BuildBase;
    @:overload(function(): Bool{})
    override function getLicense(): Bool;

}

@:native("Clockwork.Command")
extern class Command extends AObject {

    function new();

    function parse(command: String): Bool;
    function run(): Void;
    function finished(): Void;
    function error(errorMsg: String): Void;
    function cancel(): Void;
    function requiresProjectLoad(): Bool;
    function requiresLicenseValidation(): Bool;

}

@:native("Clockwork.PlayCmd")
extern class PlayCmd extends Command {

    function new();

    @:overload(function(): Void{})
    override function run(): Void;

}

@:native("Clockwork.OpenAssetImporter")
extern class OpenAssetImporter extends AObject {

    var errorMessage: String;
    var importNode: Clockwork.Node;
    var startTime: Float;
    var endTime: Float;
    var scale: Float;
    var exportAnimations: Bool;
    var verboseLog: Bool;

    function new();

    function load(assetPath: String): Bool;
    function getErrorMessage(): String;
    function exportModel(outName: String, ?animName: String, ?animationOnly: Bool): Bool;
    function setImportNode(node: Clockwork.Node): Void;
    function setStartTime(startTime: Float): Void;
    function setEndTime(endTime: Float): Void;
    function setScale(scale: Float): Void;
    function setExportAnimations(exportAnimations: Bool): Void;
    function setVerboseLog(verboseLog: Bool): Void;

}

@:native("Clockwork.Asset")
extern class Asset extends AObject {

    var guid: String;
    var name: String;
    var path: String;
    var extension: String;
    var relativePath: String;
    var cachePath: String;
    var importerType: String;
    var importerTypeName: String;
    var importer: AssetImporter;
    var parent: Asset;
    var dirty: Bool;
    var fileTimestamp: UInt;
    var dotAssetFilename: String;

      // Construct.
    function new();

    function import(): Bool;
    function preload(): Bool;
    function setPath(path: String): Bool;
    function getGUID(): String;
    function getName(): String;
    function getPath(): String;
    function getExtension(): String;
      // Get the path relative to project
    function getRelativePath(): String;
    function getCachePath(): String;
    function getResource(?typeName: String): Clockwork.Resource;
    function getImporterType(): String;
    function getImporterTypeName(): String;
    function getImporter(): AssetImporter;
    function postImportError(message: String): Void;
    function getParent(): Asset;
    function setDirty(dirty: Bool): Void;
    function isDirty(): Bool;
      // Get the last timestamp as seen by the AssetDatabase
    function getFileTimestamp(): UInt;
      // Sets the time stamp to the asset files current time
    function updateFileTimestamp(): Void;
    function getDotAssetFilename(): String;
      // Rename the asset, which depending on the asset type may be nontrivial
    function rename(newName: String): Bool;
      // Move the asset, which depending on the asset type may be nontrivial
    function move(newPath: String): Bool;
    function isFolder(): Bool;
    function load(): Bool;
    function save(): Bool;
      // Instantiate a node from the asset
    function instantiateNode(parent: Clockwork.Node, name: String): Clockwork.Node;

}

@:native("Clockwork.AssetDatabase")
extern class AssetDatabase extends AObject {

    var cachePath: String;

      // Construct.
    function new();

    function getAssetByGUID(guid: String): Asset;
    function getAssetByPath(path: String): Asset;
    function getAssetByCachePath(cachePath: String): Asset;
    function generateAssetGUID(): String;
    function registerGUID(guid: String): Void;
    function getCachePath(): String;
    function deleteAsset(asset: Asset): Void;
    function scan(): Void;
    function reimportAllAssets(): Void;
    function getResourceImporterName(resourceTypeName: String): String;
    function getDotAssetFilename(path: String): String;

}

@:native("Clockwork.AssetImporter")
extern class AssetImporter extends AObject {

    var asset: Asset;

      // Construct.
    function new(asset: Asset);

    function setDefaults(): Void;
    function preload(): Bool;
    function getAsset(): Asset;
    function getResource(?typeName: String): Clockwork.Resource;
    function requiresCacheFile(): Bool;
      // Instantiate a node from the asset
    function instantiateNode(parent: Clockwork.Node, name: String): Clockwork.Node;
    function rename(newName: String): Bool;
    function move(newPath: String): Bool;

}

@:native("Clockwork.AudioImporter")
extern class AudioImporter extends AssetImporter {

      // Construct.
    function new(asset: Asset);

    @:overload(function(): Void{})
    override function setDefaults(): Void;
    @:overload(function(?typeName: String): Clockwork.Resource{})
    override function getResource(?typeName: String): Clockwork.Resource;

}

@:native("Clockwork.JavascriptImporter")
extern class JavascriptImporter extends AssetImporter {

      // Construct.
    function new(asset: Asset);

    @:overload(function(): Void{})
    override function setDefaults(): Void;
    function isComponentFile(): Bool;
    @:overload(function(?typeName: String): Clockwork.Resource{})
    override function getResource(?typeName: String): Clockwork.Resource;

}

@:native("Clockwork.JSONImporter")
extern class JSONImporter extends AssetImporter {

      // Construct.
    function new(asset: Asset);

    @:overload(function(): Void{})
    override function setDefaults(): Void;
    @:overload(function(?typeName: String): Clockwork.Resource{})
    override function getResource(?typeName: String): Clockwork.Resource;

}

@:native("Clockwork.MaterialImporter")
extern class MaterialImporter extends AssetImporter {

      // Construct.
    function new(asset: Asset);

    @:overload(function(): Void{})
    override function setDefaults(): Void;
    function saveMaterial(): Void;
    @:overload(function(?typeName: String): Clockwork.Resource{})
    override function getResource(?typeName: String): Clockwork.Resource;

}

@:native("Clockwork.AnimationImportInfo")
extern class AnimationImportInfo extends AObject {

    var name: String;
    var startTime: Float;
    var endTime: Float;

    function new();

    function getName(): String;
    function getStartTime(): Float;
    function getEndTime(): Float;
    function setName(name: String): Void;
    function setStartTime(time: Float): Void;
    function setEndTime(time: Float): Void;

}

@:native("Clockwork.ModelImporter")
extern class ModelImporter extends AssetImporter {

    var scale: Float;
    var importAnimations: Bool;
    var animationCount: UInt;

      // Construct.
    function new(asset: Asset);

    @:overload(function(): Void{})
    override function setDefaults(): Void;
    function getScale(): Float;
    function setScale(scale: Float): Void;
    function getImportAnimations(): Bool;
    function setImportAnimations(importAnimations: Bool): Void;
    function getAnimationCount(): UInt;
    function setAnimationCount(count: UInt): Void;
    @:overload(function(?typeName: String): Clockwork.Resource{})
    override function getResource(?typeName: String): Clockwork.Resource;
    function getAnimationInfo(index: UInt): AnimationImportInfo;
      // Instantiate a node from the asset
    @:overload(function(parent: Clockwork.Node, name: String): Clockwork.Node{})
    override function instantiateNode(parent: Clockwork.Node, name: String): Clockwork.Node;

}

@:native("Clockwork.NETAssemblyImporter")
extern class NETAssemblyImporter extends AssetImporter {

      // Construct.
    function new(asset: Asset);

    @:overload(function(): Void{})
    override function setDefaults(): Void;
    @:overload(function(?typeName: String): Clockwork.Resource{})
    override function getResource(?typeName: String): Clockwork.Resource;

}

@:native("Clockwork.PEXImporter")
extern class PEXImporter extends AssetImporter {

      // Construct.
    function new(asset: Asset);

    @:overload(function(): Void{})
    override function setDefaults(): Void;
    @:overload(function(typeName: String): Clockwork.Resource{})
    override function getResource(?typeName: String): Clockwork.Resource;

}

@:native("Clockwork.PrefabImporter")
extern class PrefabImporter extends AssetImporter {

      // Construct.
    function new(asset: Asset);

    @:overload(function(): Void{})
    override function setDefaults(): Void;
    @:overload(function(): Bool{})
    override function preload(): Bool;
      // Instantiate a node from the asset
    @:overload(function(parent: Clockwork.Node, name: String): Clockwork.Node{})
    override function instantiateNode(parent: Clockwork.Node, name: String): Clockwork.Node;

}

@:native("Clockwork.SpriterImporter")
extern class SpriterImporter extends AssetImporter {

      // Construct.
    function new(asset: Asset);

    @:overload(function(): Void{})
    override function setDefaults(): Void;
    @:overload(function(?typeName: String): Clockwork.Resource{})
    override function getResource(?typeName: String): Clockwork.Resource;
    @:overload(function(parent: Clockwork.Node, name: String): Clockwork.Node{})
    override function instantiateNode(parent: Clockwork.Node, name: String): Clockwork.Node;

}

@:native("Clockwork.TextureImporter")
extern class TextureImporter extends AssetImporter {

      // Construct.
    function new(asset: Asset);

    @:overload(function(): Void{})
    override function setDefaults(): Void;
    @:overload(function(?typeName: String): Clockwork.Resource{})
    override function getResource(?typeName: String): Clockwork.Resource;
    @:overload(function(parent: Clockwork.Node, name: String): Clockwork.Node{})
    override function instantiateNode(parent: Clockwork.Node, name: String): Clockwork.Node;

}

@:native("Clockwork.LicenseSystem")
extern class LicenseSystem extends AObject {

    var sourceBuild: Bool;
    var licenseWindows: Bool;
    var licenseMac: Bool;
    var licenseAndroid: Bool;
    var licenseIOS: Bool;
    var licenseHTML5: Bool;
    var licenseModule3D: Bool;
    var key: String;
    var email: String;

      // Construct.
    function new();

    function initialize(): Void;
    function getSourceBuild(): Bool;
    function getLicenseWindows(): Bool;
    function getLicenseMac(): Bool;
    function getLicenseAndroid(): Bool;
    function getLicenseIOS(): Bool;
    function getLicenseHTML5(): Bool;
    function getLicenseModule3D(): Bool;
      // Returns whether there are any platform licenses available
    function isStandardLicense(): Bool;
      // Returns true if request to deactivate is made
    function deactivate(): Bool;
    function resetLicense(): Void;
    function loadLicense(): Bool;
      // Basic key validation
    function validateKey(key: String): Bool;
      // Activate on server
    function requestServerActivation(key: String): Void;
    function getKey(): String;
    function generateMachineID(): String;
    function getEmail(): String;
    function licenseAgreementConfirmed(): Void;

}

@:native("Clockwork.BuildAndroid")
extern class BuildAndroid extends BuildBase {


    function new(project: Project);

    @:overload(function(buildPath: String): Void{})
    override function build(buildPath: String): Void;
    @:overload(function(): String{})
    override function getBuildSubfolder(): String;

}

@:native("Clockwork.BuildBase")
extern class BuildBase extends AObject {

    var buildSubfolder: String;

    function new(project: Project, platform: PlatformID);

    function build(buildPath: String): Void;
    function useResourcePackager(): Bool;
    function getBuildSubfolder(): String;
    function addResourceDir(dir: String): Void;
    function buildLog(message: String, ?sendEvent: Bool): Void;
    function buildWarn(warning: String, ?sendEvent: Bool): Void;
    function buildError(error: String, ?sendEvent: Bool): Void;
      // Fail the current build
    function failBuild(message: String): Void;

}

@:native("Clockwork.BuildIOS")
extern class BuildIOS extends BuildBase {


    function new(project: Project);

    @:overload(function(): String{})
    override function getBuildSubfolder(): String;
    @:overload(function(buildPath: String): Void{})
    override function build(buildPath: String): Void;

}

@:native("Clockwork.BuildMac")
extern class BuildMac extends BuildBase {


    function new(project: Project);

    @:overload(function(): String{})
    override function getBuildSubfolder(): String;
    @:overload(function(buildPath: String): Void{})
    override function build(buildPath: String): Void;

}

@:native("Clockwork.BuildSystem")
extern class BuildSystem extends AObject {

    var buildPath: String;

      // Construct.
    function new();

    function setBuildPath(path: String): Void;
    function getBuildPath(): String;
    function queueBuild(buildBase: BuildBase): Void;
    function startNextBuild(): Bool;
    function buildComplete(platform: PlatformID, buildFolder: String, ?success: Bool, ?buildMessage: String): Void;

}

@:native("Clockwork.BuildWeb")
extern class BuildWeb extends BuildBase {


    function new(project: Project);

    @:overload(function(buildPath: String): Void{})
    override function build(buildPath: String): Void;
    @:overload(function(): String{})
    override function getBuildSubfolder(): String;

}

@:native("Clockwork.BuildWindows")
extern class BuildWindows extends BuildBase {


    function new(project: Project);

    @:overload(function(): String{})
    override function getBuildSubfolder(): String;
    @:overload(function(buildPath: String): Void{})
    override function build(buildPath: String): Void;

}

@:native("Clockwork.Subprocess")
extern class Subprocess extends AObject {

      // Construct.
    function new();


}

@:native("Clockwork.SubprocessSystem")
extern class SubprocessSystem extends AObject {

      // Construct.
    function new();

    function launch(command: String, args: Array<String>, ?initialDirectory: String): Subprocess;

}


