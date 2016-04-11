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

import HierarchyFrameMenu = require("./menus/HierarchyFrameMenu");
import MenuItemSources = require("./menus/MenuItemSources");
import EditorEvents = require("editor/EditorEvents");
import EditorUI = require("ui/EditorUI");

var IconTemporary = "ComponentBitmap";

class HierarchyFrame extends Clockwork.UIWidget {

    scene: Clockwork.Scene = null;
    sceneEditor: Editor.SceneEditor3D;
    hierList: Clockwork.UIListView;
    menu: HierarchyFrameMenu;
    nodeIDToItemID = {};

    constructor(parent: Clockwork.UIWidget) {

        super();

        this.menu = new HierarchyFrameMenu();

        this.load("ClockworkEditor/editor/ui/hierarchyframe.tb.txt");

        this.gravity = Clockwork.UI_GRAVITY_TOP_BOTTOM;

        var hierarchycontainer = parent.getWidget("hierarchycontainer");
        hierarchycontainer.addChild(this);

        hierarchycontainer = this.getWidget("hierarchycontainer");

        var hierList = this.hierList = new Clockwork.UIListView();
        hierList.multiSelect = true;
        hierList.rootList.id = "hierList_";

        hierList.subscribeToEvent("UIListViewSelectionChanged", (event: Clockwork.UIListViewSelectionChangedEvent) => this.handleHierListSelectionChangedEvent(event));

        hierarchycontainer.addChild(hierList);

        this.subscribeToEvent(this, "WidgetEvent", (data) => this.handleWidgetEvent(data));

        this.subscribeToEvent(EditorEvents.ActiveSceneEditorChange, (data) => this.handleActiveSceneEditorChanged(data));

        // handle dropping on hierarchy, moving node, dropping prefabs, etc
        this.subscribeToEvent(this.hierList.rootList, "DragEnded", (data) => this.handleDragEnded(data));
        // on mouse up clear the list's drag object
        this.subscribeToEvent("MouseButtonUp", () => {

          this.hierList.rootList.dragObject = null;

        });


        this.subscribeToEvent(EditorEvents.SceneClosed, (ev: EditorEvents.SceneClosedEvent) => {

            if (ev.scene == this.scene) {

                this.unsubscribeFromEvents(this.scene);
                this.scene = null;
                this.populate();

            }

        });

        this.subscribeToEvent("ComponentAdded", (ev: Clockwork.ComponentAddedEvent) => {

            if (!ev.component || ev.component.typeName != "PrefabComponent") return;

            var node = ev.node;

            var itemID = this.nodeIDToItemID[node.id];

            if (itemID) {

                this.hierList.setItemTextSkin(node.id.toString(), "HierarchyPrefabText");

            }

        });

        this.subscribeToEvent("ComponentRemoved", (ev: Clockwork.ComponentRemovedEvent) => {

            if (!ev.component || ev.component.typeName != "PrefabComponent") return;

            var node = ev.node;

            var itemID = this.nodeIDToItemID[node.id];

            if (itemID) {

                this.hierList.setItemTextSkin(node.id.toString(), "Folder");
            }

        });

        this.subscribeToEvent("TemporaryChanged", (ev: Clockwork.TemporaryChangedEvent) => {

            // this can happen on a temporary status change on a non-scripted class instance
            if (!ev.serializable) {
                return;
            }

            if (ev.serializable.typeName == "Node") {

                var node = <Clockwork.Node>ev.serializable;

                var itemID = this.nodeIDToItemID[node.id];

                if (itemID) {

                    this.hierList.setItemIcon(node.id.toString(), node.isTemporary() ? IconTemporary : "");

                }

            }

        });

    }

    handleSceneEditNodeAdded(ev: Editor.SceneEditNodeAddedEvent) {

        var node = ev.node;

        if (this.filterNode(node))
            return;

        var parentID = this.nodeIDToItemID[node.parent.id];
        var childItemID = this.recursiveAddNode(parentID, node);

        this.nodeIDToItemID[node.id] = childItemID;
    }

    handleSceneEditNodeRemoved(ev: Editor.SceneEditNodeRemovedEvent) {

        // on close
        if (!this.scene)
            return;

        var node = ev.node;

        if (this.filterNode(node))
            return;

        delete this.nodeIDToItemID[node.id];
        this.hierList.deleteItemByID(node.id.toString());

    }

    handleActiveSceneEditorChanged(event: EditorEvents.ActiveSceneEditorChangeEvent) {

        if (this.scene)
            this.unsubscribeFromEvents(this.scene);

        this.sceneEditor = null;
        this.scene = null;

        if (!event.sceneEditor)
            return;

        this.sceneEditor = event.sceneEditor;
        this.scene = event.sceneEditor.scene;

        this.populate();

        if (this.scene) {

            this.subscribeToEvent(this.scene, "SceneNodeSelected", (event: Editor.SceneNodeSelectedEvent) => this.handleSceneNodeSelected(event));
            this.subscribeToEvent(this.scene, "SceneEditNodeAdded", (ev: Editor.SceneEditNodeAddedEvent) => this.handleSceneEditNodeAdded(ev));
            this.subscribeToEvent(this.scene, "SceneEditNodeRemoved", (ev: Editor.SceneEditNodeRemovedEvent) => this.handleSceneEditNodeRemoved(ev));
            this.subscribeToEvent(this.scene, "NodeNameChanged", (ev: Clockwork.NodeNameChangedEvent) => {

                this.hierList.setItemText(ev.node.id.toString(), ev.node.name);

            });

            this.subscribeToEvent(this.scene, "SceneEditNodeReparent", (ev) => {

                if (!ev.added) {
                    delete this.nodeIDToItemID[ev.node.id];
                    this.hierList.deleteItemByID(ev.node.id.toString());
                } else {
                  var parentID = this.nodeIDToItemID[ev.node.parent.id];
                  var childItemID = this.recursiveAddNode(parentID, ev.node);
                  this.nodeIDToItemID[ev.node.id] = childItemID;

                }

            });


        }

    }

    filterNode(node: Clockwork.Node): boolean {

        if (!node) return false;

        if (node.name == "__clockwork_sceneview3d_camera") return true;

        return false;

    }

    recursiveAddNode(parentID: number, node: Clockwork.Node): number {

        if (this.filterNode(node))
            return;

        var name = node.name;

        if (!name.length)
            name = "(Anonymous)";

        var icon = "";

        if (node.isTemporary())
            icon = IconTemporary;

        var childItemID = this.hierList.addChildItem(parentID, name, icon, node.id.toString());

        if (node.getComponent("PrefabComponent")) {

            this.hierList.setItemTextSkin(node.id.toString(), "HierarchyPrefabText");

        }

        this.nodeIDToItemID[node.id] = childItemID;

        for (var i = 0; i < node.getNumChildren(false); i++) {

            this.recursiveAddNode(childItemID, node.getChildAtIndex(i));

        }

        return childItemID;

    }

    populate() {

        this.nodeIDToItemID = {};
        this.hierList.deleteAllItems();

        if (!this.scene)
            return;

        var parentID = this.hierList.addRootItem("Scene", "", this.scene.id.toString());

        this.nodeIDToItemID[this.scene.id] = parentID;

        for (var i = 0; i < this.scene.getNumChildren(false); i++) {

            this.recursiveAddNode(parentID, this.scene.getChildAtIndex(i));

        }

        this.hierList.rootList.value = -1;
        this.hierList.setExpanded(parentID, true);

    }

    handleDragEnded(ev: Clockwork.DragEndedEvent) {

        var typeName = ev.dragObject.object.typeName;

        var dropNode: Clockwork.Node = this.scene.getNode(Number(this.hierList.hoverItemID));

        if (!dropNode) return;

        if (typeName == "Node") {

            var dragNode = <Clockwork.Node>ev.dragObject.object;

            this.sceneEditor.reparentNode(dragNode, dropNode);

        } else if (typeName == "Asset") {

            var asset = <ToolCore.Asset>ev.dragObject.object;
            var newNode = asset.instantiateNode(dropNode, asset.name);
            if (newNode) {
                this.sceneEditor.registerNode(newNode);
                // getting a click event after this (I think) which
                // is causing the dropNode to be selected
                this.sceneEditor.selection.addNode(newNode, true);
            }
        }
    }

    handleSceneNodeSelected(ev: Editor.SceneNodeSelectedEvent) {

        this.hierList.selectItemByID(ev.node.id.toString(), ev.selected);

    }

    handleHierListSelectionChangedEvent(event: Clockwork.UIListViewSelectionChangedEvent) {

        if (!this.scene)
            return;

        var node = this.scene.getNode(Number(event.refid));

        if (node) {

            if (event.selected)
                this.sceneEditor.selection.addNode(node);
            else
                this.sceneEditor.selection.removeNode(node);

        }
    }

    handleWidgetEvent(data: Clockwork.UIWidgetEvent): boolean {

        if (data.type == Clockwork.UI_EVENT_TYPE_KEY_UP) {

            // node deletion
            if (data.key == Clockwork.KEY_DELETE || data.key == Clockwork.KEY_BACKSPACE) {
                this.sceneEditor.selection.delete();
            }

        } else if (data.type == Clockwork.UI_EVENT_TYPE_POINTER_DOWN) {

            if (data.target == this.hierList.rootList) {

                var node = this.scene.getNode(Number(data.refid));

                if (node) {

                    // set the widget's drag object
                    var dragObject = new Clockwork.UIDragObject(node, node.name.length ? "Node: " + node.name : "Node: (Anonymous)");
                    this.hierList.rootList.dragObject = dragObject;

                }

            }

        } else if (data.type == Clockwork.UI_EVENT_TYPE_CLICK) {

            if (this.menu.handleNodeContextMenu(data.target, data.refid)) {
                return true;
            }

            var id = data.target.id;

            if (id == "create popup") {

                var selectedId = Number(this.hierList.rootList.selectedItemID);
                var node = this.scene.getNode(selectedId);
                if (!node)
                    node = this.scene;
                if (this.menu.handlePopupMenu(data.target, data.refid, node))
                    return true;

            }

            // create
            if (id == "menu create") {
                if (!ToolCore.toolSystem.project) return;
                var src = MenuItemSources.getMenuItemSource("hierarchy create items");
                var menu = new Clockwork.UIMenuWindow(data.target, "create popup");
                menu.show(src);
                return true;

            }


        } else if (data.type == Clockwork.UI_EVENT_TYPE_RIGHT_POINTER_UP) {

            var id = data.target.id;
            var node: Clockwork.Node;

            if (id == "hierList_")
                node = this.scene.getNode(Number(this.hierList.hoverItemID));
            else
                node = this.scene.getNode(Number(id));

            if (node) {

                this.menu.createNodeContextMenu(this, node, data.x, data.y);

            }

        }

        return false;
    }

}

export = HierarchyFrame;