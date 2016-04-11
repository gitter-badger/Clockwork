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

import EditorEvents = require("editor/EditorEvents");
import EditorUI = require("ui/EditorUI");
import ScriptWidget = require("ui/ScriptWidget");
import Preferences = require("editor/Preferences");
import ProjectTemplates = require("resources/ProjectTemplates");

class WelcomeFrame extends ScriptWidget {

    constructor(parent: Clockwork.UIWidget) {

        super();

        this.load("ClockworkEditor/editor/ui/welcomeframe.tb.txt");

        var recentProjects = <Clockwork.UILayout>this.getWidget("recentprojects");
        this.gravity = Clockwork.UI_GRAVITY_ALL;

        this.recentList = new Clockwork.UIListView();
        this.recentList.rootList.id = "recentList";

        recentProjects.addChild(this.recentList);

        var container = <Clockwork.UILayout>parent.getWidget("resourceviewcontainer");

        container.addChild(this);

        this.updateRecentProjects();

        this.subscribeToEvent(EditorEvents.CloseProject, () => {
            this.updateRecentProjects();
        });

        this.initExampleBrowser();

    }

    handleClickedExample(example: ProjectTemplates.ProjectTemplateDefinition) {

        var ops = EditorUI.getModelOps();
        var env = ToolCore.toolEnvironment;
        ops.showCreateProject(example);
    }

    addExample(example: ProjectTemplates.ProjectTemplateDefinition) {

        var fileSystem = Clockwork.getFileSystem();

        // Verify that at least one of the projects for this example exists, otherwise bounce out
        let exists = false;
        example.templates.forEach(template => {
            if (fileSystem.dirExists(template.folder)) {
                exists = true;
            }
        });

        if (!exists) {
            return;
        }

        var exlayout = <Clockwork.UILayout>this.getWidget("examples_layout");

        if (!this.currentExampleLayout) {
            this.currentExampleLayout = new Clockwork.UILayout();
            this.currentExampleLayout.spacing = 8;
            exlayout.addChild(this.currentExampleLayout);
        }

        // 200x150

        var exampleLayout = new Clockwork.UILayout();
        exampleLayout.skinBg = "StarCondition";
        exampleLayout.axis = Clockwork.UI_AXIS_Y;
        exampleLayout.layoutDistribution = Clockwork.UI_LAYOUT_DISTRIBUTION_GRAVITY;
        exampleLayout.layoutSize = Clockwork.UI_LAYOUT_SIZE_AVAILABLE;

        // IMAGE BUTTON

        var id = example.name;

        var button = new Clockwork.UIButton();
        button.skinBg = "StarButton";
        button.id = id;
        var image = new Clockwork.UIImageWidget();

        button.onClick = () => {

            this.handleClickedExample(example);

        };

        image.image = example.screenshot;
        image.skinBg = "ImageFrame";
        var rect = [0, 0, image.imageWidth / 2, image.imageHeight / 2];
        image.rect = rect;

        // NAME FIELD
        var nameField = new Clockwork.UITextField();
        nameField.skinBg = "ImageCaption";
        nameField.text = example.name;

        var nameRect = [0, image.imageHeight / 2 - 16, image.imageWidth / 2, image.imageHeight / 2];

        nameField.rect = nameRect;

        nameField.gravity = Clockwork.UI_GRAVITY_BOTTOM;

        image.addChild(nameField);

        button.addChild(image);

        var lp = new Clockwork.UILayoutParams();
        lp.minWidth = image.imageWidth / 2;
        lp.minHeight = image.imageHeight / 2;

        button.layoutParams = lp;

        button.gravity = Clockwork.UI_GRAVITY_LEFT;

        exampleLayout.addChild(button);

        // DESC TEXT

        var descField = new Clockwork.UIEditField();
        descField.styling = true;
        descField.multiline = true;
        descField.readOnly = true;
        descField.wrapping = true;

        var styleDesc = "<color #A9A9A9>" + example.desc + "</color>";

        descField.text = styleDesc;

        descField.adaptToContentSize = true;

        lp.height = 42;

        lp.width = image.imageWidth / 2;

        descField.layoutParams = lp;

        exampleLayout.addChild(descField);

        this.currentExampleLayout.addChild(exampleLayout);

        this.exampleCount++;
        // three across, todo, be smarter about this
        if (!(this.exampleCount % 3)) {
            this.currentExampleLayout = null;
        }

    }

    initExampleBrowser() {
        let examples = ProjectTemplates.getExampleProjectTemplateDefinitions();
        for (var i = 0; i < examples.length; i++) {
            this.addExample(examples[i]);
        }
    }

    handleWidgetEvent(ev: Clockwork.UIWidgetEvent) {

        if (ev.type == Clockwork.UI_EVENT_TYPE_RIGHT_POINTER_UP) {
            if (ev.target.id == "recentList") {
                this.openFrameMenu(ev.x, ev.y);
            }
        }

        if (ev.type == Clockwork.UI_EVENT_TYPE_CLICK) {

            var id = ev.target.id;

            if (id == "open project") {

                var utils = new Editor.FileUtils();
                var path = utils.openProjectFileDialog();
                if (path) {

                    this.sendEvent(EditorEvents.LoadProject, { path: path });

                }

                return true;
            }

            if (id == "new project") {

                var mo = EditorUI.getModelOps();
                mo.showNewProject();
                return true;

            }

            if (id == "recentList") {
                var path: string = this.recent[this.recentList.getSelectedItemID()];
                this.sendEvent(EditorEvents.LoadProject, { path: path });
            }

            if (id == "recentProjectsContextMenu") {
                var prefs = Preferences.getInstance();
                if (ev.refid == "clear recent projects") {
                    prefs.deleteRecentProjects();
                    this.updateRecentProjects();
                }
            }
        }
    }

    updateRecentProjects() {

        this.recentList.deleteAllItems();

        var prefs = Preferences.getInstance();
        prefs.updateRecentProjects();

        this.recent = prefs.recentProjects;

        for (var i in this.recent) {
            this.recentList.addRootItem(this.recent[i], "Folder.icon", i);
        }

    }

    private openFrameMenu(x: number, y: number) {
        var menu = new Clockwork.UIMenuWindow(this, "recentProjectsContextMenu");
        var menuButtons = new Clockwork.UISelectItemSource();
        menuButtons.addItem(new Clockwork.UISelectItem("Clear Recent Projects", "clear recent projects"));
        menu.show(menuButtons, x, y);
    }

    // examples
    exampleCount = 0;
    currentExampleLayout: Clockwork.UILayout;

    recent: string[] = [];
    recentList: Clockwork.UIListView;

}

export = WelcomeFrame;
