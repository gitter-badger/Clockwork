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

import EditorEvents = require("../../editor/EditorEvents");
import EditorUI = require("../EditorUI");
import ModalWindow = require("./ModalWindow");

import ProjectTemplates = require("../resources/ProjectTemplates");

class CreateProject extends ModalWindow {

    constructor(projectTemplate: ProjectTemplates.ProjectTemplateDefinition) {

        super();

        this.projectTemplate = projectTemplate;

        this.init("Create Project", "ClockworkEditor/editor/ui/createproject.tb.txt");

        this.projectPathField = <Clockwork.UIEditField>this.getWidget("project_path");
        this.projectNameField = <Clockwork.UIEditField>this.getWidget("project_name");
        this.projectLanguageField = <Clockwork.UISelectDropdown>this.getWidget("project_language");
        this.image = <Clockwork.UIImageWidget>this.getWidget("project_image");

        if (!projectTemplate.screenshot)
            this.image.visibility = Clockwork.UI_WIDGET_VISIBILITY_GONE;
        else
            this.image.image = projectTemplate.screenshot;


        var fileSystem = Clockwork.getFileSystem();

        var userDocuments = fileSystem.userDocumentsDir;

        if (Clockwork.platform == "MacOSX") {

            userDocuments += "Documents/ClockworkProjects";

        } else {

            userDocuments += "ClockworkProjects";

        }

        this.projectPathField.text = userDocuments;
        this.populateLanguageSelectionList();

        this.resizeToFitContent();
        this.center();

    }

    tryProjectCreate(): boolean {

        var name = this.projectNameField.text.trim();

        if (!name) {
            EditorUI.showModalError("New Project Editor Error", "Please enter a project name");
            return false;
        }

        var folder = this.projectPathField.text.trim();

        if (!folder) {
            EditorUI.showModalError("New Project Editor Error", "Please choose a root project folder");
            return false;
        }

        folder += "/" + name;

        var fileSystem = Clockwork.getFileSystem();

        if (fileSystem.dirExists(folder) || fileSystem.fileExists(folder)) {

            var message = folder + " exists\n\nPlease choose a different root folder or project name";
            EditorUI.showModalError("New Project Editor Error", message);
            return false;

        }


        folder = Clockwork.addTrailingSlash(folder);
        if (!fileSystem.dirExists(folder)) {

            // Determine if we have a language template for the selected language.
            let templateDetail: ProjectTemplates.ProjectTemplateDetail;
            let selectedLanguage = this.projectLanguageField.text;

            for (let i = 0; i < this.projectTemplate.templates.length; i++) {
                if (this.projectTemplate.templates[i].language === selectedLanguage) {
                    templateDetail = this.projectTemplate.templates[i];
                    break;
                }
            }

            // Do the creation!
            if (templateDetail && fileSystem.dirExists(templateDetail.folder)) {

                if (!fileSystem.copyDir(templateDetail.folder, folder)) {
                  var message = "Unable to copy folder: " + templateDetail.folder + " to " + folder;
                  EditorUI.showModalError("New Project Editor Error", message);
                  return false;
                }

                var utils = new Editor.FileUtils();

                utils.createDirs(folder + "Cache");

                if (!fileSystem.dirExists(folder)) {
                    var message = "Unable to create folder: " + folder + "\n\nPlease choose a different root folder or project name";
                    EditorUI.showModalError("New Project Editor Error", message);
                    return false;
                }

                // Look for the .clockwork project file and if it exists, then rename it
                let fileResults = fileSystem.scanDir(folder, "*.clockwork", Clockwork.SCAN_FILES, false);
                if (fileResults.length === 1) {
                    fileSystem.rename(folder + fileResults[0], folder + name + ".clockwork");
                } else {
                    // Just create the file.  We either don't have one existing, or we have more than one and don't know which one to rename
                    var file = new Clockwork.File(folder + name + ".clockwork", Clockwork.FILE_WRITE);
                    file.close();
                }

                this.hide();

                this.sendEvent(EditorEvents.LoadProject, { path: folder });
                return true;
            } else {
                let message = [
                    "Unable to create project for:",
                    "",
                    `language: ${selectedLanguage}`,
                    `template: ${templateDetail.folder}`,
                    "",
                    "Please choose a different language."
                ].join("\n");

                EditorUI.showModalError("New Project Editor Error", message);
                return false;
            }
        }
        return false;
    }

    handleWidgetEvent(ev: Clockwork.UIWidgetEvent) {

        if (ev.type == Clockwork.UI_EVENT_TYPE_CLICK) {

            var id = ev.target.id;

            if (id == "cancel") {
                this.hide();
                return true;
            }
            else if (id == "choose_path") {
                var utils = new Editor.FileUtils();
                var path = utils.newProjectFileDialog();

                if (path.length)
                    this.projectPathField.text = path;

                return true;
            }
            else if (id == "create") {

                this.tryProjectCreate();

                return true;

            }
        }
    }

    /**
     * Queries the json file for languages that are available to this template and populates the
     * list.
     */
    populateLanguageSelectionList() {
        this.projectLanguageFieldSource.clear();

        this.projectTemplate.templates.forEach(templateDetail => {
            this.projectLanguageFieldSource.addItem(new Clockwork.UISelectItem(templateDetail.language));
        });

        this.projectLanguageField.source = this.projectLanguageFieldSource;
        this.projectLanguageField.value = 0;
    }

    projectPathField: Clockwork.UIEditField;
    projectNameField: Clockwork.UIEditField;
    projectLanguageField: Clockwork.UISelectDropdown;
    projectLanguageFieldSource: Clockwork.UISelectItemSource = new Clockwork.UISelectItemSource();
    image: Clockwork.UIImageWidget;

    projectTemplate: ProjectTemplates.ProjectTemplateDefinition;
}


export = CreateProject;
