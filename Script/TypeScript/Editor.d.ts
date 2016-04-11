//////////////////////////////////////////////////////////
// IMPORTANT: THIS FILE IS GENERATED, CHANGES WILL BE LOST
//////////////////////////////////////////////////////////

// Clockwork TypeScript Definitions

/// <reference path="Clockwork.d.ts" />

declare module Editor {


   // enum EditMode
   export type EditMode = number;
   export var EDIT_SELECT: EditMode;
   export var EDIT_MOVE: EditMode;
   export var EDIT_ROTATE: EditMode;
   export var EDIT_SCALE: EditMode;


   // enum AxisMode
   export type AxisMode = number;
   export var AXIS_WORLD: AxisMode;
   export var AXIS_LOCAL: AxisMode;


   // enum SceneEditType
   export type SceneEditType = number;
   export var SCENEEDIT_UNKNOWN: SceneEditType;
   export var SCENEEDIT_SELECTION: SceneEditType;


   export var FINDTEXT_FLAG_NONE: number;
   export var FINDTEXT_FLAG_CASESENSITIVE: number;
   export var FINDTEXT_FLAG_WHOLEWORD: number;
   export var FINDTEXT_FLAG_WRAP: number;
   export var FINDTEXT_FLAG_NEXT: number;
   export var FINDTEXT_FLAG_PREV: number;
   export var EDITOR_MODALERROR: number;
   export var EDITOR_MODALINFO: number;


//----------------------------------------------------
// MODULE: Editor
//----------------------------------------------------


   export class FileUtils extends Clockwork.AObject {

      mobileProvisionPath: string;

      // Construct.
      constructor();

      createDirs(folder: string): boolean;
      getMobileProvisionPath(): string;
      getAndroidSDKPath(defaultPath: string): string;
      getAntPath(defaultPath: string): string;
      getJDKRootPath(defaultPath: string): string;
      openProjectFileDialog(): string;
      newProjectFileDialog(): string;
      getBuildPath(defaultPath: string): string;
      revealInFinder(fullpath: string): void;

   }

   export class EditorMode extends Clockwork.AObject {

      // Construct.
      constructor();

      playProject(addArgs?: string, debug?: boolean): boolean;
      isPlayerEnabled(): boolean;

   }

   export class PlayerMode extends Clockwork.AObject {

      // Construct.
      constructor();

      launchedByEditor(): boolean;

   }

   export class JSResourceEditor extends ResourceEditor {

      constructor(fullpath: string, container: Clockwork.UITabContainer);

      findText(findText: string, flags: number): boolean;
      findTextClose(): void;
      gotoTokenPos(tokenPos: number): void;
      gotoLineNumber(lineNumber: number): void;
      formatCode(): void;
      setFocus(): void;
      save(): boolean;

   }

   export class ResourceEditor extends Clockwork.AObject {

      button: Clockwork.UIButton;
      fullPath: string;
      rootContentWidget: Clockwork.UIWidget;
      modified: boolean;

      constructor(fullpath: string, container: Clockwork.UITabContainer);

      getButton(): Clockwork.UIButton;
      hasUnsavedModifications(): boolean;
      setFocus(): void;
      close(navigateToAvailableResource?: boolean): void;
      findText(text: string, flags: number): boolean;
      findTextClose(): void;
      requiresInspector(): boolean;
      getFullPath(): string;
      undo(): void;
      redo(): void;
      save(): boolean;
      getRootContentWidget(): Clockwork.UIWidget;
      invokeShortcut(shortcut: string): void;
      requestClose(): void;
      setModified(modified: boolean): void;

   }

   export class Gizmo3D extends Clockwork.AObject {

      view: SceneView3D;
      axisMode: AxisMode;
      editMode: EditMode;
      gizmoNode: Clockwork.Node;
      snapTranslationX: number;
      snapTranslationY: number;
      snapTranslationZ: number;
      snapRotation: number;
      snapScale: number;

      constructor();

      setView(view3D: SceneView3D): void;
      setAxisMode(mode: AxisMode): void;
      getAxisMode(): AxisMode;
      setEditMode(mode: EditMode): void;
      selected(): boolean;
      show(): void;
      hide(): void;
      update(): void;
      getGizmoNode(): Clockwork.Node;
      getSnapTranslationX(): number;
      getSnapTranslationY(): number;
      getSnapTranslationZ(): number;
      getSnapRotation(): number;
      getSnapScale(): number;
      setSnapTranslationX(value: number): void;
      setSnapTranslationY(value: number): void;
      setSnapTranslationZ(value: number): void;
      setSnapRotation(value: number): void;
      setSnapScale(value: number): void;

   }

   export class SceneEditor3D extends ResourceEditor {

      selection: SceneSelection;
      sceneView3D: SceneView3D;
      scene: Clockwork.Scene;
      gizmo: Gizmo3D;

      constructor(fullpath: string, container: Clockwork.UITabContainer);

      getSelection(): SceneSelection;
      getSceneView3D(): SceneView3D;
      registerNode(node: Clockwork.Node): void;
      reparentNode(node: Clockwork.Node, newParent: Clockwork.Node): void;
      getScene(): Clockwork.Scene;
      getGizmo(): Gizmo3D;
      setFocus(): void;
      requiresInspector(): boolean;
      close(navigateToAvailableResource?: boolean): void;
      save(): boolean;
      undo(): void;
      redo(): void;
      cut(): void;
      copy(): void;
      paste(): void;
      invokeShortcut(shortcut: string): void;
      static getSceneEditor(scene: Clockwork.Scene): SceneEditor3D;

   }

   export class SceneSelection extends Clockwork.AObject {

      selectedNodeCount: number;

      constructor(sceneEditor: SceneEditor3D);

      cut(): void;
      copy(): void;
      paste(): void;
      delete(): void;
      // Add a node to the selection, if clear specified removes current nodes first
      addNode(node: Clockwork.Node, clear?: boolean): void;
      removeNode(node: Clockwork.Node, quiet?: boolean): void;
      getBounds(bbox: Clockwork.BoundingBox): void;
      contains(node: Clockwork.Node): boolean;
      getSelectedNode(index: number): Clockwork.Node;
      getSelectedNodeCount(): number;
      clear(): void;

   }

   export class SceneView3D extends Clockwork.UISceneView {

      pitch: number;
      yaw: number;
      debugRenderer: Clockwork.DebugRenderer;
      sceneEditor3D: SceneEditor3D;

      constructor(sceneEditor: SceneEditor3D);

      setPitch(pitch: number): void;
      setYaw(yaw: number): void;
      frameSelection(): void;
      enable(): void;
      disable(): void;
      isEnabled(): boolean;
      getDebugRenderer(): Clockwork.DebugRenderer;
      getSceneEditor3D(): SceneEditor3D;

   }

   export class CubemapGenerator extends EditorComponent {

      imageSize: number;

      // Construct.
      constructor();

      render(): boolean;
      getImageSize(): number;
      setImageSize(size: number): void;

   }

   export class EditorComponent extends Clockwork.Component {

      // Construct.
      constructor();


   }



}
