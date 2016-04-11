package clockwork;

@:native("Clockwork")
extern enum EditMode {
    EDIT_SELECT;
    EDIT_MOVE;
    EDIT_ROTATE;
    EDIT_SCALE;
}
@:native("Clockwork")
extern enum AxisMode {
    AXIS_WORLD;
    AXIS_LOCAL;
}
@:native("Clockwork")
extern enum SceneEditType {
    SCENEEDIT_UNKNOWN;
    SCENEEDIT_SELECTION;
}
//Clockwork Haxe Definitions

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


@:native("Clockwork.FileUtils")
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

@:native("Clockwork.EditorMode")
extern class EditorMode extends AObject {

      // Construct.
    function new();

    function playProject(?addArgs: String, ?debug: Bool): Bool;
    function isPlayerEnabled(): Bool;

}

@:native("Clockwork.PlayerMode")
extern class PlayerMode extends AObject {

      // Construct.
    function new();

    function launchedByEditor(): Bool;

}

@:native("Clockwork.JSResourceEditor")
extern class JSResourceEditor extends ResourceEditor {

    function new(fullpath: String, container: Clockwork.UITabContainer);

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

@:native("Clockwork.ResourceEditor")
extern class ResourceEditor extends AObject {

    var button: Clockwork.UIButton;
    var fullPath: String;
    var rootContentWidget: Clockwork.UIWidget;
    var modified: Bool;

    function new(fullpath: String, container: Clockwork.UITabContainer);

    function getButton(): Clockwork.UIButton;
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
    function getRootContentWidget(): Clockwork.UIWidget;
    function invokeShortcut(shortcut: String): Void;
    function requestClose(): Void;
    function setModified(modified: Bool): Void;

}

@:native("Clockwork.Gizmo3D")
extern class Gizmo3D extends AObject {

    var view: SceneView3D;
    var axisMode: AxisMode;
    var editMode: EditMode;
    var gizmoNode: Clockwork.Node;
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
    function getGizmoNode(): Clockwork.Node;
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

@:native("Clockwork.SceneEditor3D")
extern class SceneEditor3D extends ResourceEditor {

    var selection: SceneSelection;
    var sceneView3D: SceneView3D;
    var scene: Clockwork.Scene;
    var gizmo: Gizmo3D;

    function new(fullpath: String, container: Clockwork.UITabContainer);

    function getSelection(): SceneSelection;
    function getSceneView3D(): SceneView3D;
    function registerNode(node: Clockwork.Node): Void;
    function reparentNode(node: Clockwork.Node, newParent: Clockwork.Node): Void;
    function getScene(): Clockwork.Scene;
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
    function getSceneEditor(scene: Clockwork.Scene): SceneEditor3D;

}

@:native("Clockwork.SceneSelection")
extern class SceneSelection extends AObject {

    var selectedNodeCount: UInt;

    function new(sceneEditor: SceneEditor3D);

    function cut(): Void;
    function copy(): Void;
    function paste(): Void;
    function delete(): Void;
      // Add a node to the selection, if clear specified removes current nodes first
    function addNode(node: Clockwork.Node, ?clear: Bool): Void;
    function removeNode(node: Clockwork.Node, ?quiet: Bool): Void;
    function getBounds(bbox: Clockwork.BoundingBox): Void;
    function contains(node: Clockwork.Node): Bool;
    function getSelectedNode(index: UInt): Clockwork.Node;
    function getSelectedNodeCount(): UInt;
    function clear(): Void;

}

@:native("Clockwork.SceneView3D")
extern class SceneView3D extends UISceneView {

    var pitch: Float;
    var yaw: Float;
    var debugRenderer: Clockwork.DebugRenderer;
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
    function getDebugRenderer(): Clockwork.DebugRenderer;
    function getSceneEditor3D(): SceneEditor3D;

}

@:native("Clockwork.CubemapGenerator")
extern class CubemapGenerator extends EditorComponent {

    var imageSize: Int;

      // Construct.
    function new();

    function render(): Bool;
    function getImageSize(): Int;
    function setImageSize(size: Int): Void;

}

@:native("Clockwork.EditorComponent")
extern class EditorComponent extends Component {

      // Construct.
    function new();


}


