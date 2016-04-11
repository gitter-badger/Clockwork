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

import EditorUI = require("ui/EditorUI");
import UIEvents = require("ui/UIEvents");
import PlayMode = require("ui/playmode/PlayMode");
import EditorLicense = require("./EditorLicense");
import EditorEvents = require("./EditorEvents");
import Preferences = require("./Preferences");

class Editor extends Clockwork.ScriptObject {

    project: ToolCore.Project;
    editorLicense: EditorLicense;
    playMode: PlayMode;

    static instance: Editor;

    projectCloseRequested: boolean;
    exitRequested: boolean;

    constructor() {

        super();

        // limit the framerate to limit CPU usage
        Clockwork.getEngine().maxFps = 60;

        Clockwork.getEngine().autoExit = false;

        Editor.instance = this;

        Preferences.getInstance().read();

        this.initUI();

        this.editorLicense = new EditorLicense();

        EditorUI.initialize();

        this.playMode = new PlayMode();

        Clockwork.getResourceCache().autoReloadResources = true;

        this.subscribeToEvent(EditorEvents.LoadProject, (data) => this.handleEditorLoadProject(data));
        this.subscribeToEvent(EditorEvents.CloseProject, (data) => this.handleEditorCloseProject(data));
        this.subscribeToEvent("ProjectUnloaded", (data) => {
            Clockwork.graphics.windowTitle = "ClockworkEditor";
            this.handleProjectUnloaded(data);
        });

        this.subscribeToEvent("IPCPlayerWindowChanged", (data) => {
            var playerWindow = Preferences.getInstance().playerWindow;
            //if player window is maximized, then we want keep the window size from the previous state
            if (data.maximized) {
                playerWindow.x = data.posX;
                playerWindow.y = data.posY;
                playerWindow.monitor = data.monitor;
                playerWindow.maximized = true;
            } else {
                playerWindow = {x: data.posX, y: data.posY, width: data.width, height: data.height, monitor: data.monitor, maximized: data.maximized};
            }
            Preferences.getInstance().savePlayerWindowData(playerWindow);
        });

        this.subscribeToEvent("ScreenMode", (data:Clockwork.ScreenModeEvent) => this.saveWindowPreferences(data));
        this.subscribeToEvent("WindowPos", (data:Clockwork.ScreenModeEvent) => this.saveWindowPreferences(data));

        this.subscribeToEvent("ExitRequested", (data) => this.handleExitRequested(data));

        this.subscribeToEvent("ProjectLoaded", (data) => {
            Clockwork.graphics.windowTitle = "ClockworkEditor - " + data.projectPath;
            Preferences.getInstance().registerRecentProject(data.projectPath);
        });

        this.subscribeToEvent("EditorResourceCloseCanceled", (data) => {
            //if user canceled closing the resource, then user has changes that he doesn't want to lose
            //so cancel exit/project close request and unsubscribe from event to avoid closing all the editors again
            this.exitRequested = false;
            this.projectCloseRequested = false;
            this.unsubscribeFromEvent(EditorEvents.EditorResourceClose);
        });

        this.parseArguments();
    }

    initUI() {

      var ui = Clockwork.ui;
      ui.loadSkin("ClockworkEditor/resources/default_skin/skin.tb.txt", "ClockworkEditor/editor/skin/skin.tb.txt");
      ui.addFont("ClockworkEditor/resources/vera.ttf", "Vera");
      ui.addFont("ClockworkEditor/resources/MesloLGS-Regular.ttf", "Monaco");
      ui.setDefaultFont("Vera", 12);

    }

    saveWindowPreferences(data: Clockwork.ScreenModeEvent): boolean {
        var graphics = Clockwork.getGraphics();
        if (!graphics) return false;

        var pos = graphics.getWindowPosition();
        var width = graphics.getWidth();
        var height = graphics.getHeight();
        var monitor = graphics.getCurrentMonitor();

        var editorWindowData = Preferences.getInstance().editorWindow;

        if (graphics.getMaximized()) {
            editorWindowData.x = pos[0];
            editorWindowData.y = pos[1];
            editorWindowData.maximized = true;
            editorWindowData.monitor = monitor;
        } else {
            editorWindowData = {x: pos[0], y: pos[1], width: width, height: height, monitor: monitor, maximized: false};
        }

        Preferences.getInstance().saveEditorWindowData(editorWindowData);

        return true;
    }

    handleEditorLoadProject(event: EditorEvents.LoadProjectEvent): boolean {

        var system = ToolCore.getToolSystem();
        if (system.project) {

            this.sendEvent(UIEvents.MessageModalEvent,
                { type: "error", title: "Project already loaded", message: "Project already loaded" });

            return false;

        }
        return system.loadProject(event.path);
    }

    closeAllResourceEditors() {
        var editor = EditorUI.getCurrentResourceEditor();
        if (!editor) {
          if (this.exitRequested) {
              this.exit();
          } else if (this.projectCloseRequested) {
              this.closeProject();
          }
          return;
        }
        //wait when we close resource editor to check another resource editor for unsaved changes and close it
        this.subscribeToEvent(EditorEvents.EditorResourceClose, (data) => {
            this.closeAllResourceEditors();
        });
        editor.requestClose();
    }

    handleEditorCloseProject(event) {
        this.projectCloseRequested = true;
        this.closeAllResourceEditors();
    }

    closeProject() {
        this.sendEvent("IPCPlayerExitRequest");
        var system = ToolCore.getToolSystem();

        if (system.project) {

            system.closeProject();
            this.sendEvent(EditorEvents.ProjectClosed);
            this.projectCloseRequested = false;
            this.unsubscribeFromEvent(EditorEvents.EditorResourceClose);
        }

    }

    handleProjectUnloaded(event) {

        this.sendEvent(EditorEvents.ActiveSceneEditorChange, { sceneEditor: null });

    }

    parseArguments() {

        var args = Clockwork.getArguments();

        var idx = 0;

        while (idx < args.length) {

            if (args[idx] == "--project") {

                this.sendEvent(EditorEvents.LoadProject, { path: args[idx + 1] });

            }

            idx++;

        }

    }

    // event handling
    handleExitRequested(data) {
        if (this.exitRequested) return;
        this.sendEvent("IPCPlayerExitRequest");
        this.exitRequested = true;
        this.closeAllResourceEditors();
    }

    exit() {
        //Preferences.getInstance().write();
        EditorUI.shutdown();
        Clockwork.getEngine().exit();
    }


}

export = Editor;
