package clockwork;

@:native("Clockwork")
extern enum CSComponentMethod {
    CSComponentMethod_Start;
    CSComponentMethod_DelayedStart;
    CSComponentMethod_Update;
    CSComponentMethod_PostUpdate;
    CSComponentMethod_FixedUpdate;
    CSComponentMethod_PostFixedUpdate;
}
//Clockwork Haxe Definitions

extern class ClockworkNET {


}

//----------------------------------------------------
// MODULE: NETCore
//----------------------------------------------------


@:native("Clockwork.NETCore")
extern class NETCore extends AObject {


      // Construct.
    function new();

    function shutdown(): Void;
    function addAssemblyLoadPath(assemblyPath: String): Void;
    function execAssembly(assemblyName: String, args: Array<String>): Int;
    function waitForDebuggerConnect(): Void;
      // to get a reference from
    @:overload(function(): Clockwork.Context{})
    override function getContext(): Clockwork.Context;

}

@:native("Clockwork.NETVariantMap")
extern class NETVariantMap extends RefCounted {

    function new();

    function getBool(key: String): Bool;
    function getInt(key: String): Int;
    function getFloat(key: String): Float;
    function getVector3(key: String): Clockwork.Vector3;
    function getQuaternion(key: String): Clockwork.Quaternion;
    function getPtr(key: String): Clockwork.RefCounted;
    function getString(key: String): String;
    function getVariantType(key: String): VariantType;
    function getResourceFromRef(key: String): Clockwork.Resource;
    function contains(key: String): Bool;

}



//----------------------------------------------------
// MODULE: NETScript
//----------------------------------------------------


@:native("Clockwork.CSComponent")
extern class CSComponent extends ScriptComponent {

    var assemblyFile: CSComponentAssembly;

      // Construct.
    function new();

    @:overload(function(): Void{})
    override function applyAttributes(): Void;
      // Handle enabled/disabled state change. Changes update event subscription.
    @:overload(function(): Void{})
    override function onSetEnabled(): Void;
    function applyFieldValues(): Void;
    function setComponentClassName(name: String): Void;
    @:overload(function(): String{})
    override function getComponentClassName(): String;
    @:overload(function(): Clockwork.ScriptComponentFile{})
    override function getComponentFile(): Clockwork.ScriptComponentFile;
    function getAssemblyFile(): CSComponentAssembly;
    function setAssemblyFile(assemblyFile: CSComponentAssembly): Void;

}

@:native("Clockwork.CSComponentAssembly")
extern class CSComponentAssembly extends ScriptComponentFile {

    var classNames: Array<String>;

      // Construct.
    function new();

    function createCSComponent(classname: String): CSComponent;
      // Only valid in editor, as we don't inspect assembly at runtime
    function getClassNames(): Array<String>;

}

@:native("Clockwork.CSManaged")
extern class CSManaged extends AObject {

      // Construct.
    function new();

    function initialize(): Bool;
    function nETUpdate(timeStep: Float): Void;
    function cSComponentCreate(assemblyName: String, componentName: String): CSComponent;
    function cSComponentApplyFields(component: CSComponent, fieldMapPtr: NETVariantMap): Void;
    function cSComponentCallMethod(id: UInt, methodID: CSComponentMethod, ?value: Float): Void;

}

@:native("Clockwork.CSScriptObject")
extern class CSScriptObject extends AObject {

      // Construct.
    function new();


}


