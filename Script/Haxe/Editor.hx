package atomic;

@:native("Atomic")
extern enum EditMode {
    EDIT_SELECT;
    EDIT_MOVE;
    EDIT_ROTATE;
    EDIT_SCALE;
}
@:native("Atomic")
extern enum AxisMode {
    AXIS_WORLD;
    AXIS_LOCAL;
}
@:native("Atomic")
extern enum SceneEditType {
    SCENEEDIT_UNKNOWN;
    SCENEEDIT_SELECTION;
}
//Atomic Haxe Definitions

extern class Editor {


    public static var FINDTEXT_FLAG_NONE: UInt;
    public static var FINDTEXT_FLAG_CASESENSITIVE: UInt;
    public static var FINDTEXT_FLAG_WHOLEWORD: UInt;
    public static var FINDTEXT_FLAG_WRAP: UInt;
    public static var FINDTEXT_FLAG_NEXT: UInt;
    public static var FINDTEXT_FLAG_PREV: UInt;
    public static var EDITOR_MODALERROR: UInt;
    public static var EDITOR_MODALINFO: UInt;


}

//----------------------------------------------------
// MODULE: Editor
//----------------------------------------------------


@:native("Atomic.FileUtils")
extern class FileUtils extends AObject {

    var mobileProvisionPath: String;

      // Construct.
    function new();

    function createDirs(folder: String): Bool;
    function getMobileProvisionPath(): String;
    function getAndroidSDKPath(defaultPath: String): String;
    function getAntPath(defaultPath: String): String;
    function getJDKRootPath(defaultPath: String): String;
    function openProjectFileDialog(): String;
    function newProjectFileDialog(): String;
    function getBuildPath(defaultPath: String): String;
    function revealInFinder(fullpath: String): Void;

}

@:native("Atomic.EditorMode")
extern class EditorMode extends AObject {

      // Construct.
    function new();

    function playProject(?addArgs: String, ?debug: Bool): Bool;
    function isPlayerEnabled(): Bool;

}

@:native("Atomic.PlayerMode")
extern class PlayerMode extends AObject {

      // Construct.
    function new();

    function launchedByEditor(): Bool;

}

@:native("Atomic.JSResourceEditor")
extern class JSResourceEditor extends ResourceEditor {

    function new(fullpath: String, container: Atomic.UITabContainer);

    @:overload(function(findText: String, flags: UInt): Bool{})
    override function findText(text: String, flags: UInt): Bool;
    @:overload(function(): Void{})
    override function findTextClose(): Void;
    function gotoTokenPos(tokenPos: Int): Void;
    function gotoLineNumber(lineNumber: Int): Void;
    function formatCode(): Void;
    @:overload(function(): Void{})
    override function setFocus(): Void;
    @:overload(function(): Bool{})
    override function save(): Bool;

}

@:native("Atomic.ResourceEditor")
extern class ResourceEditor extends AObject {

    var button: Atomic.UIButton;
    var fullPath: String;
    var rootContentWidget: Atomic.UIWidget;
    var modified: Bool;

    function new(fullpath: String, container: Atomic.UITabContainer);

    function getButton(): Atomic.UIButton;
    function hasUnsavedModifications(): Bool;
    function setFocus(): Void;
    function close(?navigateToAvailableResource: Bool): Void;
    function findText(text: String, flags: UInt): Bool;
    function findTextClose(): Void;
    function requiresInspector(): Bool;
    function getFullPath(): String;
    function undo(): Void;
    function redo(): Void;
    function save(): Bool;
    function getRootContentWidget(): Atomic.UIWidget;
    function invokeShortcut(shortcut: String): Void;
    function requestClose(): Void;
    function setModified(modified: Bool): Void;

}

@:native("Atomic.Gizmo3D")
extern class Gizmo3D extends AObject {

    var view: SceneView3D;
    var axisMode: AxisMode;
    var editMode: EditMode;
    var gizmoNode: Atomic.Node;
    var snapTranslationX: Float;
    var snapTranslationY: Float;
    var snapTranslationZ: Float;
    var snapRotation: Float;
    var snapScale: Float;

    function new();

    function setView(view3D: SceneView3D): Void;
    function setAxisMode(mode: AxisMode): Void;
    function getAxisMode(): AxisMode;
    function setEditMode(mode: EditMode): Void;
    function selected(): Bool;
    function show(): Void;
    function hide(): Void;
    function update(): Void;
    function getGizmoNode(): Atomic.Node;
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

@:native("Atomic.SceneEditor3D")
extern class SceneEditor3D extends ResourceEditor {

    var selection: SceneSelection;
    var sceneView3D: SceneView3D;
    var scene: Atomic.Scene;
    var gizmo: Gizmo3D;

    function new(fullpath: String, container: Atomic.UITabContainer);

    function getSelection(): SceneSelection;
    function getSceneView3D(): SceneView3D;
    function registerNode(node: Atomic.Node): Void;
    function reparentNode(node: Atomic.Node, newParent: Atomic.Node): Void;
    function getScene(): Atomic.Scene;
    function getGizmo(): Gizmo3D;
    @:overload(function(): Void{})
    override function setFocus(): Void;
    @:overload(function(): Bool{})
    override function requiresInspector(): Bool;
    @:overload(function(?navigateToAvailableResource: Bool): Void{})
    override function close(?navigateToAvailableResource: Bool): Void;
    @:overload(function(): Bool{})
    override function save(): Bool;
    @:overload(function(): Void{})
    override function undo(): Void;
    @:overload(function(): Void{})
    override function redo(): Void;
    function cut(): Void;
    function copy(): Void;
    function paste(): Void;
    @:overload(function(shortcut: String): Void{})
    override function invokeShortcut(shortcut: String): Void;
    function getSceneEditor(scene: Atomic.Scene): SceneEditor3D;

}

@:native("Atomic.SceneSelection")
extern class SceneSelection extends AObject {

    var selectedNodeCount: UInt;

    function new(sceneEditor: SceneEditor3D);

    function cut(): Void;
    function copy(): Void;
    function paste(): Void;
    function delete(): Void;
      // Add a node to the selection, if clear specified removes current nodes first
    function addNode(node: Atomic.Node, ?clear: Bool): Void;
    function removeNode(node: Atomic.Node, ?quiet: Bool): Void;
    function getBounds(bbox: Atomic.BoundingBox): Void;
    function contains(node: Atomic.Node): Bool;
    function getSelectedNode(index: UInt): Atomic.Node;
    function getSelectedNodeCount(): UInt;
    function clear(): Void;

}

@:native("Atomic.SceneView3D")
extern class SceneView3D extends UISceneView {

    var pitch: Float;
    var yaw: Float;
    var debugRenderer: Atomic.DebugRenderer;
    var sceneEditor3D: SceneEditor3D;

    function new(sceneEditor: SceneEditor3D);

    function setPitch(pitch: Float): Void;
    function setYaw(yaw: Float): Void;
    function frameSelection(): Void;
    @:overload(function(): Void{})
    override function enable(): Void;
    @:overload(function(): Void{})
    override function disable(): Void;
    function isEnabled(): Bool;
    function getDebugRenderer(): Atomic.DebugRenderer;
    function getSceneEditor3D(): SceneEditor3D;

}

@:native("Atomic.CubemapGenerator")
extern class CubemapGenerator extends EditorComponent {

    var imageSize: Int;

      // Construct.
    function new();

    function render(): Bool;
    function getImageSize(): Int;
    function setImageSize(size: Int): Void;

}

@:native("Atomic.EditorComponent")
extern class EditorComponent extends Component {

      // Construct.
    function new();


}


