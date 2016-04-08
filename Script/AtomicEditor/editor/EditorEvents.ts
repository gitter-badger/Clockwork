//
// Copyright (c) 2014-2016 THUNDERBEAST GAMES LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

export const ModalError = "ModalError";
export interface ModalErrorEvent {

  title: string;
  message: string;

}

export const PlayerStarted = "EditorPlayerStarted";
export const PlayerStopped = "EditorPlayerStopped";
export const PlayerLog = "EditorPlayerLog";
export interface PlayerLogEvent {

  message: string;
  level: number;

}


export const ActiveSceneEditorChange = "EditorActiveSceneEditorChange";
export interface ActiveSceneEditorChangeEvent {

  sceneEditor: Editor.SceneEditor3D;

}


export const SceneClosed = "EditorSceneClosed";
export interface SceneClosedEvent {

  scene: Atomic.Scene;

}

export const ContentFolderChanged = "ContentFolderChanged";
export interface ContentFolderChangedEvent {

  path: string;

}

export const CloseProject = "EditorCloseProject";
export const ProjectClosed = "EditorProjectClosed";

export const LoadProject = "EditorLoadProject";
export interface LoadProjectEvent {

  // The full path to the .atomic file
  path: string;

}

export const SaveAllResources = "EditorSaveAllResources";

export const SaveResource = "EditorSaveResource";
export interface SaveResourceEvent {

  // The full path to the resource to save
  // empty or undefined for current
  path: string;

}

export const EditorResourceClose = "EditorResourceClose";
export interface EditorCloseResourceEvent {

  editor:Editor.ResourceEditor;
  navigateToAvailableResource:boolean;

}

export const EditResource = "EditorEditResource";
export interface EditResourceEvent {

  // The full path to the resource to edit
  path: string;

}

export const SceneEditStateChange = "SceneEditStateChange";
export interface SceneEditStateChangeEvent {

  serializable: Atomic.Serializable;

}

export const InspectorProjectReference = "InspectorProjectReference";
export interface InspectorProjectReferenceEvent {

    // The full path to the resource to edit
    path: string;

}

export const RemoveCurrentAssetAssigned = "RemoveCurrentAssetAssigned";
export interface RemoveCurrentAssetAssignedEvent {

}
