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

import EditorUI = require("../EditorUI");
import EditorEvents = require("../../editor/EditorEvents");
import ModalWindow = require("./ModalWindow");

class ResourceSelection extends ModalWindow {

    folderList: Clockwork.UIListView;
    callback: (returnObject: any, args: any) => void;
    args: any;
    resourceType: string;

    populate(importerType: string, resourceType: string) {

        var db = ToolCore.assetDatabase;
        var assets = db.getAssetsByImporterType(importerType, resourceType);

        this.folderList.addRootItem("None", "", "");

        for (var i in assets) {

            var asset = assets[i];

            if (importerType == "JavascriptImporter") {
                if (!(<ToolCore.JavascriptImporter>asset.importer).isComponentFile())
                    continue;
            }

            // TODO: Generalize this for other cache assets
            if (resourceType == "Animation") {

                var modelImporter = <ToolCore.ModelImporter>(asset.importer);
                var animations = modelImporter.getAnimations();

                if (animations.length) {

                    for (var i in animations) {

                        this.folderList.addRootItem(animations[i].animationName + " : " + asset.name, "", animations[i].name);

                    }


                }

            } else {

                this.folderList.addRootItem(asset.relativePath, "", asset.guid);

            }

        }

    }

    constructor(windowText: string, importerType: string, resourceType: string, callback: (asset: ToolCore.Asset, args: any) => void, args: any) {

        super();

        this.resourceType = resourceType;
        this.callback = callback;
        this.args = args;

        this.load("ClockworkEditor/editor/ui/resourceselection.tb.txt");

        var foldercontainer = this.getWidget("resourcecontainer");

        var folderList = this.folderList = new Clockwork.UIListView();

        folderList.rootList.id = "resourceList_";

        foldercontainer.addChild(folderList);

        this.populate(importerType, resourceType);

        this.text = windowText;
        this.setSize(800, 600);
        this.center();

    }

    handleWidgetEvent(ev: Clockwork.UIWidgetEvent) {

        if (ev.count >= 2) {
            var id = ev.target.id;
            if (id == this.folderList.rootList.id) {
                this.selectFile();
            }
        }

        if (ev.type == Clockwork.UI_EVENT_TYPE_CLICK) {
            var id = ev.target.id;

            if (id == "select") {

                return this.selectFile();

            }

            if (id == "cancel") {

                this.hide();

                return true;
            }

        }

    }

    selectFile(): boolean {

        var id = this.folderList.selectedItemID;

        if (id == "") {
            this.sendEvent(EditorEvents.RemoveCurrentAssetAssigned);
            this.hide();
            return true;
        }

        if (this.resourceType == "Animation") {

          if (id.length) {
              this.callback(Clockwork.cache.getResource("Animation", id), this.args);
              this.hide();
              return true;
          }

          this.hide();
          return true;

        } else {

            if (id.length) {
                this.callback(ToolCore.assetDatabase.getAssetByGUID(id), this.args);
                this.hide();
                return true;
            }
        }
        return false;
    }
}


export = ResourceSelection;
