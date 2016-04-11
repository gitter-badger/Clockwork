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

import EditorUI = require("../../EditorUI");
import CreateComponentButton = require("./CreateComponentButton");
import ScriptWidget = require("ui/ScriptWidget");
import EditorEvents = require("editor/EditorEvents");
import SerializableEditType = require("./SerializableEditType");
import SelectionSection = require("./SelectionSection");
import SelectionPrefabWidget = require("./SelectionPrefabWidget");
import AttributeInfoEdit = require("./AttributeInfoEdit");

class NodeSection extends SelectionSection {

    prefabWidget: SelectionPrefabWidget;

    transformEdits: AttributeInfoEdit[] = [];

    updateDelta: number = 0.0;

    constructor(editType: SerializableEditType) {

        super(editType);

        this.prefabWidget = new SelectionPrefabWidget();
        this.attrLayout.addChild(this.prefabWidget);

        this.transformEdits.push(this.attrEdits["Position"]);
        this.transformEdits.push(this.attrEdits["Rotation"]);
        this.transformEdits.push(this.attrEdits["Scale"]);

        this.subscribeToEvent("Update", (ev) => this.handleUpdate(ev));

    }

    handleUpdate(ev) {

        this.updateDelta -= ev.timeStep;

        if (this.updateDelta < 0.0) {

            this.updateDelta = 0.1;

            Clockwork.ui.blockChangedEvents = true;

            for (var i in this.transformEdits) {
                this.transformEdits[i].refresh();
            }

            Clockwork.ui.blockChangedEvents = false;

        }


    }


}

class ComponentSection extends SelectionSection {

    constructor(editType: SerializableEditType, inspector: SelectionInspector) {

        super(editType);

        var deleteButton = new Clockwork.UIButton();
        deleteButton.text = "Delete Component";
        deleteButton.fontDescription = SelectionSection.fontDesc;

        deleteButton.onClick = () => {

            inspector.onComponentDelete(editType);
            return true;

        };

        this.attrLayout.addChild(deleteButton);

    }

}

class SceneSection extends SelectionSection {

    constructor(editType: SerializableEditType) {

        super(editType);

    }

}

interface AttributeEditResourceChangedEvent {

    attrInfoEdit: AttributeInfoEdit;
    resource: Clockwork.Resource;

}

class JSComponentSection extends ComponentSection {

    constructor(editType: SerializableEditType, inspector: SelectionInspector) {

        super(editType, inspector);

        this.hasDynamicAttr = true;

        this.subscribeToEvent(this, "AttributeEditResourceChanged", (ev) => this.handleAttributeEditResourceChanged(ev));

    }

    private handleAttributeEditResourceChanged(ev: AttributeEditResourceChangedEvent) {

        var jsc = <Clockwork.JSComponent>this.editType.getFirstObject();

        if (!jsc)
            return;

        var attrInfos = jsc.getAttributes();
        this.updateDynamicAttrInfos(attrInfos);

    }

}


// Node Inspector + Component Inspectors

class SelectionInspector extends ScriptWidget {

    constructor(sceneEditor: Editor.SceneEditor3D) {

        super();

        this.sceneEditor = sceneEditor;

        var mainLayout = this.mainLayout = new Clockwork.UILayout();
        mainLayout.spacing = 4;

        var lp = new Clockwork.UILayoutParams();
        lp.width = 304;

        mainLayout.layoutDistribution = Clockwork.UI_LAYOUT_DISTRIBUTION_GRAVITY;
        mainLayout.layoutPosition = Clockwork.UI_LAYOUT_POSITION_LEFT_TOP;
        mainLayout.layoutParams = lp;
        mainLayout.axis = Clockwork.UI_AXIS_Y;

        this.addChild(mainLayout);

        var noticeLayout = this.multipleSelectNotice = new Clockwork.UILayout();
        noticeLayout.axis = Clockwork.UI_AXIS_Y;
        noticeLayout.layoutParams = lp;
        var noticeText = new Clockwork.UITextField();
        noticeText.textAlign = Clockwork.UI_TEXT_ALIGN_CENTER;
        noticeText.skinBg = "InspectorTextAttrName";
        noticeText.text = "Multiple Selection - Some components are hidden";
        noticeText.fontDescription = SelectionSection.fontDesc;
        noticeText.gravity = Clockwork.UI_GRAVITY_LEFT_RIGHT;
        noticeText.layoutParams = lp;
        noticeLayout.addChild(noticeText);
        noticeLayout.visibility = Clockwork.UI_WIDGET_VISIBILITY_GONE;
        mainLayout.addChild(noticeLayout);

        this.createComponentButton = new CreateComponentButton();
        mainLayout.addChild(this.createComponentButton);

        this.subscribeToEvent(sceneEditor.scene, "SceneEditStateChangesBegin", (data) => this.handleSceneEditStateChangesBeginEvent());
        this.subscribeToEvent("SceneEditStateChange", (data) => this.handleSceneEditStateChangeEvent(data));
        this.subscribeToEvent(sceneEditor.scene, "SceneEditStateChangesEnd", (data) => this.handleSceneEditStateChangesEndEvent());

        this.subscribeToEvent(sceneEditor.scene, "SceneEditNodeRemoved", (ev: Editor.SceneEditNodeRemovedEvent) => this.handleSceneEditNodeRemoved(ev));
        this.subscribeToEvent(sceneEditor.scene, "SceneEditComponentAddedRemoved", (ev) => this.handleSceneEditComponentAddedRemovedEvent(ev));

        this.subscribeToEvent(this.createComponentButton, "SelectionCreateComponent", (data) => this.handleSelectionCreateComponent(data));

    }

    pruneSections() {

        var remove: SelectionSection[] = [];

        for (var i in this.sections) {

            var section = this.sections[i];

            var editType = section.editType;

            if (editType.typeName == "Node") {
                continue;
            }

            if (editType.typeName == "Scene") {
                var gotone = false;
                for (var j in this.nodes) {
                    if (this.nodes[j].typeName == "Scene") {
                        gotone = true;
                        break;
                    }
                }
                if (gotone)
                    continue;
            }

            if (!editType.nodes.length) {

                remove.push(section);

            }

        }

        if (remove.length) {

            for (var i in remove) {

                var section = remove[i];
                this.removeSection(section);

            }

            this.suppressSections();
        }

    }

    suppressSections() {

        this.multipleSelectNotice.visibility = Clockwork.UI_WIDGET_VISIBILITY_GONE;

        for (var i in this.sections) {

            var section = this.sections[i];
            var editType = section.editType;

            if (editType.typeName == "Node" || editType.typeName == "Scene") {
                continue;
            }

            var suppressed = false;

            for (var j in this.nodes) {
                if (editType.nodes.indexOf(this.nodes[j]) == -1) {
                    suppressed = true;
                    break;
                }
            }

            if (suppressed)
                this.multipleSelectNotice.visibility = Clockwork.UI_WIDGET_VISIBILITY_VISIBLE;

            section.suppress(suppressed);

        }

    }

    refresh() {

        Clockwork.ui.blockChangedEvents = true;

        this.pruneSections();
        this.suppressSections();

        for (var i in this.sections) {

            this.sections[i].refresh();

        }

        if (this.nodeSection) {
            this.nodeSection.prefabWidget.updateSelection(this.nodes);
        }

        Clockwork.ui.blockChangedEvents = false;

    }

    addSection(editType: SerializableEditType) {

        var section: SelectionSection;

        if (editType.typeName == "Node") {

            this.nodeSection = new NodeSection(editType);
            section = this.nodeSection;

        } else if (editType.typeName == "Scene") {

            section = new SceneSection(editType);

        } else if (editType.typeName == "JSComponent") {
            section = new JSComponentSection(editType, this);
        }
        else {

            section = new ComponentSection(editType, this);

        }

        section.value = SelectionInspector.sectionStates[editType.typeName] ? 1 : 0;

        this.mainLayout.removeChild(this.createComponentButton, false);
        this.mainLayout.removeChild(this.multipleSelectNotice, false);

        // sort it in alphabetically

        this.sections.push(section);

        this.sections.sort(function(a, b) {

            if (a.editType.typeName == "Node" && b.editType.typeName == "Scene")
                return -1;

            if (a.editType.typeName == "Scene" && b.editType.typeName == "Node")
                return 1;

            if (a.editType.typeName == "Node" || a.editType.typeName == "Scene")
                return -1;

            if (b.editType.typeName == "Node" || b.editType.typeName == "Scene")
                return 1;

            return a.editType.typeName.localeCompare(b.editType.typeName);
        });

        var idx = this.sections.indexOf(section);

        if (idx == 0) {

            if (this.sections.length == 1) {
                this.mainLayout.addChild(section);
            } else {
                this.mainLayout.addChildBefore(section, this.sections[1]);
            }
        }
        else if (idx == this.sections.length - 1) {

            this.mainLayout.addChild(section);
        }
        else {
            this.mainLayout.addChildAfter(section, this.sections[idx - 1]);
        }

        // move the create component button down

        this.mainLayout.addChild(this.multipleSelectNotice);
        this.mainLayout.addChild(this.createComponentButton);

    }

    removeSection(section: SelectionSection) {

        SelectionInspector.sectionStates[section.editType.typeName] = section.value ? true : false;
        var index = this.sections.indexOf(section);
        this.sections.splice(index, 1);
        this.mainLayout.removeChild(section);

    }

    removeSerializable(serial: Clockwork.Serializable) {

        for (var i in this.sections) {

            var section = this.sections[i];

            var e = section.editType;

            var index = e.objects.indexOf(serial);

            if (index != -1) {

                e.objects.splice(index, 1);

            }

            if (serial.typeName == "Node") {

                index = e.nodes.indexOf(<Clockwork.Node>serial);

                if (index != -1) {

                    e.nodes.splice(index, 1);

                }
            }

        }

    }

    addSerializable(serial: Clockwork.Serializable): SerializableEditType {

        var editType = this.getEditType(serial);

        // does it already exist?
        for (var i in this.sections) {

            var section = this.sections[i];

            var e = section.editType;

            if (e.compareTypes(editType, this.nodes.length > 1)) {
                e.addSerializable(serial);
                return e;
            }

        }

        this.addSection(editType);

        return editType;

    }

    getEditType(serial: Clockwork.Serializable): SerializableEditType {

        var typeName = serial.typeName;


        if (SelectionInspector._editTypes[typeName]) {
            return new SelectionInspector._editTypes[typeName](serial);
        }

        return new SerializableEditType(serial);

    }

    addNode(node: Clockwork.Node) {

        var index = this.nodes.indexOf(node);

        if (index == -1) {
            this.nodes.push(node);
            this.addSerializable(node);
            var components = node.getComponents();
            for (var i in components) {

                if (this.filterComponent(components[i]))
                    continue;

                var editType = this.addSerializable(components[i]);
                editType.addNode(node);
            }
            this.refresh();
        }
    }

    removeNode(node: Clockwork.Node) {

        var index = this.nodes.indexOf(node);

        if (index != -1) {

            this.nodes.splice(index, 1);
            this.removeSerializable(node);
            var components = node.getComponents();
            for (var i in components) {

                if (this.filterComponent(components[i]))
                    continue;

                this.removeSerializable(components[i]);
            }

            this.refresh();
        }

        // save node section state
        if (!this.nodes.length && this.nodeSection)
            SelectionInspector.sectionStates["Node"] = this.nodeSection.value ? true : false;

    }

    handleSceneEditStateChangesBeginEvent() {

        this.stateChangesInProgress = true;

    }

    handleSceneEditNodeRemoved(ev: Editor.SceneEditNodeRemovedEvent) {

        this.removeNode(ev.node);

    }

    handleSceneEditComponentAddedRemovedEvent(ev: Editor.SceneEditComponentAddedRemovedEvent) {

        if (this.filterComponent(ev.component)) {
            // still refresh as may affect UI (for example PrefabComponents)
            this.refresh();
            return;
        }

        var editType;
        if (!ev.removed) {

            editType = this.addSerializable(ev.component);
            editType.addNode(ev.node);

        } else {

            for (var i in this.sections) {

                var section = this.sections[i];
                editType = section.editType;

                var index = editType.objects.indexOf(ev.component);
                if (index != -1) {

                    editType.objects.splice(index, 1);
                    index = editType.nodes.indexOf(ev.node);
                    if (index != -1) {
                        editType.nodes.splice(index, 1);
                    }
                    break;

                }

            }

        }

        this.refresh();

    }

    onComponentDelete(editType: SerializableEditType) {

        var removed: Clockwork.Component[] = [];

        for (var i in editType.objects) {

            var c = <Clockwork.Component>editType.objects[i];
            removed.push(c);

        }

        for (var i in removed) {

            var c = removed[i];

            var node = c.node;
            c.remove();

            this.removeSerializable(removed[i]);

            var index = editType.nodes.indexOf(node);
            if (index != -1) {
                editType.nodes.splice(index, 1);
            }

        }

        if (removed.length) {

            this.sceneEditor.scene.sendEvent("SceneEditEnd");
            this.refresh();

        }

    }

    handleSelectionCreateComponent(ev) {

        var valid = true;

        if (ev.componentTypeName != "JSComponent") {

            for (var i in this.nodes) {

                var node = this.nodes[i];
                if (node.getComponent(ev.componentTypeName, false)) {
                    valid = false;
                    break;
                }
            }
        }

        if (!valid) {

            EditorUI.showModalError("Component Create", "Unable to create component, a node with an existing " + ev.componentTypeName + " component is selected");
            return;

        }

        for (var i in this.nodes) {

            var node = this.nodes[i];

            var c = node.createComponent(ev.componentTypeName);

            if (!c) {
                console.log("ERROR: unable to create component ", ev.componentTypeName);
                return;
            }

            var editType = this.addSerializable(c);
            editType.addNode(node);

            for (var i in this.sections) {
                if (this.sections[i].editType == editType) {
                    this.sections[i].value = 1;
                    break;
                }

            }

        }

        this.refresh();

    }


    handleSceneEditStateChangeEvent(ev: Editor.SceneEditStateChangeEvent) {

        if (!this.stateChangesInProgress)
            return;

        if (this.stateChanges.indexOf(ev.serializable) == -1) {
            this.stateChanges.push(ev.serializable);
        }

    }

    getPrefabComponent(node: Clockwork.Node): Clockwork.PrefabComponent {

        if (node.getComponent("PrefabComponent"))
            return <Clockwork.PrefabComponent>node.getComponent("PrefabComponent");

        if (node.parent)
            return this.getPrefabComponent(node.parent);

        return null;

    }

    filterComponent(component: Clockwork.Component): boolean {

        if (component.typeName == "PrefabComponent") {
            return true;
        }

        return false;

    }

    handleSceneEditStateChangesEndEvent() {

        Clockwork.ui.blockChangedEvents = true;

        var sections: SelectionSection[] = [];

        for (var i in this.stateChanges) {

            var serial = this.stateChanges[i];

            for (var j in this.sections) {

                var section = this.sections[j];

                if (sections.indexOf(section) != -1)
                    continue;

                if (section.editType.objects.indexOf(serial) != -1) {

                    sections.push(section);

                    if (section.hasDynamicAttr) {

                        var object = section.editType.getFirstObject();
                        if (object) {
                            var attrInfos = object.getAttributes();
                            section.updateDynamicAttrInfos(attrInfos);
                        }
                    }

                    section.refresh();
                }

            }

        }

        Clockwork.ui.blockChangedEvents = false;
        this.stateChanges = [];
        this.stateChangesInProgress = false;

    }


    mainLayout: Clockwork.UILayout;

    multipleSelectNotice: Clockwork.UILayout;

    sceneEditor: Editor.SceneEditor3D;
    nodes: Clockwork.Node[] = [];
    sections: SelectionSection[] = [];

    createComponentButton: CreateComponentButton;
    nodeSection: NodeSection;

    stateChangesInProgress: boolean = false;
    stateChanges: Clockwork.Serializable[] = [];

    // ------------------------------------

    static registerEditType(typeName: string, type: typeof SerializableEditType) {

        SelectionInspector._editTypes[typeName] = type;

    }

    private static sectionStates: { [typeName: string]: boolean } = {};
    private static _editTypes: { [typeName: string]: typeof SerializableEditType } = {};

    private static Ctor = (() => {

        SelectionInspector.sectionStates["Node"] = true;

    })();

}

export = SelectionInspector;
