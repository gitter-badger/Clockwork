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

import strings = require("../../EditorStrings");
import EditorEvents = require("../../../editor/EditorEvents");
import EditorUI = require("../../EditorUI");
import MenuItemSources = require("./MenuItemSources");
import Preferences = require("editor/Preferences");

class MainFrameMenu extends Clockwork.ScriptObject {

    constructor() {

        super();

        MenuItemSources.createMenuItemSource("menu edit", editItems);
        MenuItemSources.createMenuItemSource("menu file", fileItems);
        MenuItemSources.createMenuItemSource("menu build", buildItems);
        MenuItemSources.createMenuItemSource("menu tools", toolsItems);
        MenuItemSources.createMenuItemSource("menu developer", developerItems);
        MenuItemSources.createMenuItemSource("menu help", helpItems);

    }

    handlePopupMenu(target: Clockwork.UIWidget, refid: string): boolean {

        if (target.id == "menu edit popup") {

            if (refid == "edit play") {
                EditorUI.getShortcuts().invokePlayOrStopPlayer();
                return true;
            }

            if (refid == "edit play debug") {
                EditorUI.getShortcuts().invokePlayOrStopPlayer(true);
                return true;
            }

            if (refid == "edit format code") {
                EditorUI.getShortcuts().invokeFormatCode();
                return true;
            }

            if (refid == "edit undo") {
                EditorUI.getShortcuts().invokeUndo();
                return true;
            }

            if (refid == "edit redo") {
                EditorUI.getShortcuts().invokeRedo();
                return true;
            }

            if (refid == "edit cut") {
                EditorUI.getShortcuts().invokeCut();
                return true;
            }

            if (refid == "edit copy") {
                EditorUI.getShortcuts().invokeCopy();
                return true;
            }

            if (refid == "edit paste") {
                EditorUI.getShortcuts().invokePaste();
                return true;
            }

            if (refid == "edit select all") {
                EditorUI.getShortcuts().invokeSelectAll();
                return true;
            }

            if (refid == "edit snap settings") {
                EditorUI.getModelOps().showSnapSettings();
                return true;
            }

            if (refid == "edit frame selected") {
                EditorUI.getShortcuts().invokeFrameSelected();
                return true;
            }


            return false;

        } else if (target.id == "menu file popup") {
            if (refid == "quit") {

                this.sendEvent("ExitRequested");
                return true;

            }
            if (refid == "file new project") {

                if (ToolCore.toolSystem.project) {

                    EditorUI.showModalError("Project Open",
                        "Please close the current project before creating a new one");

                    return true;
                }

                var mo = EditorUI.getModelOps();
                mo.showNewProject();

                return true;

            }
            if (refid == "file open project") {

                var utils = new Editor.FileUtils();
                var path = utils.openProjectFileDialog();

                if (path == "") {

                    return true;

                }

                var openProject = () => this.sendEvent(EditorEvents.LoadProject, { path: path });

                if (ToolCore.toolSystem.project) {

                    this.subscribeToEvent(EditorEvents.ProjectClosed, () => {

                        this.unsubscribeFromEvent(EditorEvents.ProjectClosed);
                        openProject();

                    });

                    this.sendEvent(EditorEvents.CloseProject);

                } else {

                    openProject();

                }

                return true;

            }

            if (refid == "file close project") {

                this.sendEvent(EditorEvents.CloseProject);

                return true;

            }

            if (refid == "file save file") {
                EditorUI.getShortcuts().invokeFileSave();
                return true;
            }

            if (refid == "file close file") {
                EditorUI.getShortcuts().invokeFileClose();
                return true;
            }

            if (refid == "file save all") {
                this.sendEvent(EditorEvents.SaveAllResources);
                return true;
            }

            return false;

        } else if (target.id == "menu developer popup") {

            if (refid == "developer show console") {
                Clockwork.ui.showConsole(true);
                return true;
            }

            if (refid == "developer show uidebugger") {

                if (Clockwork.engine.debugBuild) {
                    Clockwork.UI.debugShowSettingsWindow(EditorUI.getView());
                }
                else {
                    EditorUI.showModalError("Debug Build Required",
                        "UIDebugger currently requires a Debug engine build");
                }

                return true;
            }

            if (refid == "developer assetdatabase scan") {

              ToolCore.assetDatabase.scan();

            }

            if (refid == "developer assetdatabase force") {

              ToolCore.assetDatabase.reimportAllAssets();

            }

            //Sets all value in prefs.json to default and shuts down the editor.
            if (refid == "developer clear preferences") {
                var myPrefs = Preferences.getInstance();
                myPrefs.useDefaultConfig();
                myPrefs.saveEditorWindowData(myPrefs.editorWindow);
                myPrefs.savePlayerWindowData(myPrefs.playerWindow);
                Clockwork.getEngine().exit();
            }

        } else if (target.id == "menu tools popup") {

            if (refid == "tools toggle profiler") {
                Clockwork.ui.toggleDebugHud();
                return true;
            }

        } else if (target.id == "menu build popup") {

            if (refid == "build build") {

                EditorUI.getModelOps().showBuild();

                return true;

            } else if (refid == "build settings") {

                EditorUI.getModelOps().showBuildSettings();
                return true;

            }

        } else if (target.id == "menu help popup") {
            if (refid == "about clockwork editor") {
                EditorUI.getModelOps().showAbout();
                return true;
            }
            if (refid == "manage license") {
                EditorUI.getModelOps().showManageLicense();
                return true;
            }
            if (refid == "help forums") {
                Clockwork.fileSystem.systemOpen("http://clockworkgameengine.com/forum/");
                return true;
            } else if (refid == "help chat") {
                Clockwork.fileSystem.systemOpen("https://gitter.im/ClockworkGameEngine/ClockworkGameEngine/");
                return true;
            } else if (refid == "help github") {
                Clockwork.fileSystem.systemOpen("https://github.com/ClockworkGameEngine/ClockworkGameEngine/");
                return true;
            } else if (refid == "help api") {
                var url = "file://" + ToolCore.toolEnvironment.toolDataDir + "Docs/JSDocs/Clockwork.html";
                Clockwork.fileSystem.systemOpen(url);
                return true;
            }

        }

    }

}

export = MainFrameMenu;

// initialization
var StringID = strings.StringID;

var editItems = {

    "Undo": ["edit undo", StringID.ShortcutUndo],
    "Redo": ["edit redo", StringID.ShortcutRedo],
    "-1": null,
    "Cut": ["edit cut", StringID.ShortcutCut],
    "Copy": ["edit copy", StringID.ShortcutCopy],
    "Paste": ["edit paste", StringID.ShortcutPaste],
    "Select All": ["edit select all", StringID.ShortcutSelectAll],
    "-2": null,
    "Frame Selected": ["edit frame selected", StringID.ShortcutFrameSelected],
    "-3": null,
    "Find": ["edit find", StringID.ShortcutFind],
    "Find Next": ["edit find next", StringID.ShortcutFindNext],
    "Find Prev": ["edit find prev", StringID.ShortcutFindPrev],
    "-4": null,
    "Format Code": ["edit format code", StringID.ShortcutBeautify],
    "-5": null,
    "Play": ["edit play", StringID.ShortcutPlay],
    "Debug (C# Project)": ["edit play debug", StringID.ShortcutPlayDebug],
    "-6": null,
    "Snap Settings": ["edit snap settings"]

};

var toolsItems = {

    "Toggle Profiler": ["tools toggle profiler"]

};

var buildItems = {

    "Build": ["build build", StringID.ShortcutBuild],
    "Build Settings": ["build settings", StringID.ShortcutBuildSettings]
};


var developerItems = {

    "Show Console": ["developer show console"],
    "Clear Preferences": ["developer clear preferences"], //Adds clear preference to developer menu items list
    "Debug": {
        "UI Debugger": ["developer show uidebugger"],
        "Asset Database": {
            "Scan": ["developer assetdatabase scan"],
            "Force Reimport": ["developer assetdatabase force"]
        }
    }


};

var fileItems = {

    "New Project": ["file new project"],
    "Open Project": ["file open project"],
    "Save Project": ["file save project"],
    "-1": null,
    "Close Project": ["file close project"],
    "-2": null,
    "Save File": ["file save file", StringID.ShortcutSaveFile],
    "Save All Files": ["file save all"],
    "Close File": ["file close file", StringID.ShortcutCloseFile],
    "-3": null,
    "Quit": "quit"
};

var helpItems = {

    "Check for Updates": "check update",
    "API Documentation": ["help api"],
    "-1": null,
    "Clockwork Chat": ["help chat"],
    "Clockwork Forums": ["help forums"],
    "-2": null,
    "Clockwork Game Engine on GitHub": ["help github"],
    "About Clockwork Editor": "about clockwork editor"
};