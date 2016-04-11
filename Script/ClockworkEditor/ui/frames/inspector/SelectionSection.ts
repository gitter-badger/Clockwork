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

import SerializableEditType = require("./SerializableEditType");
import AttributeInfoEdit = require("./AttributeInfoEdit");
import SelectionSectionUI = require("./SelectionSectionUI");

import "./ComponentAttributeUI";

abstract class SelectionSection extends Clockwork.UISection {

    hasDynamicAttr: boolean = false;
    editType: SerializableEditType;
    attrLayout: Clockwork.UILayout;
    suppressed: boolean = false;
    customUI: SelectionSectionUI;

    attrEdits: { [name: string]: AttributeInfoEdit } = {};

    constructor(editType: SerializableEditType) {

        super();

        this.editType = editType;

        this.text = editType.typeName;

        this.createUI();

    }

    contains(serial:Clockwork.Serializable):boolean {

        return this.editType.objects.indexOf(serial) == -1;

    }

    refresh() {

        for (var name in this.attrEdits) {

            this.attrEdits[name].refresh();

        }

        if (this.customUI)
            this.customUI.refresh();

    }

    suppress(value: boolean) {

        if (this.suppressed == value) {
            return;
        }

        this.suppressed = value;
        if (value) {
            this.visibility = Clockwork.UI_WIDGET_VISIBILITY_GONE;
        } else {
            this.visibility = Clockwork.UI_WIDGET_VISIBILITY_VISIBLE;
        }

    }

    updateDynamicAttrInfos(attrInfos: Clockwork.AttributeInfo[]) {

        Clockwork.ui.blockChangedEvents = true;

        this.editType.attrInfos = attrInfos;

        var attrEdit: AttributeInfoEdit;
        var remove: AttributeInfoEdit[] = [];

        var addWidget: Clockwork.UIWidget;

        for (var name in this.attrEdits) {

            attrEdit = this.attrEdits[name];

            if (attrEdit.attrInfo.dynamic) {
                remove.push(attrEdit);
            } else {
                addWidget = attrEdit;
            }

        }

        for (var i in remove) {

            var attrEdit = remove[i];
            attrEdit.remove();
            delete this.attrEdits[attrEdit.attrInfo.name];

        }

        for (var i in attrInfos) {

            var attr = attrInfos[i];

            if (!attr.dynamic) {
                continue;
            }

            if (attr.mode & Clockwork.AM_NOEDIT)
                continue;

            var attrEdit = AttributeInfoEdit.createAttrEdit(this.editType, attr);

            if (!attrEdit)
                continue;

            this.attrEdits[attr.name] = attrEdit;

            if (!addWidget) {
                this.attrLayout.addChild(attrEdit);
                addWidget = attrEdit;
            } else {
                this.attrLayout.addChildAfter(attrEdit, addWidget);
                addWidget = attrEdit;
            }

        }

        this.refresh();

        Clockwork.ui.blockChangedEvents = false;

    }

    createUI() {

        var attrLayout = this.attrLayout = new Clockwork.UILayout(Clockwork.UI_AXIS_Y);
        attrLayout.spacing = 3;
        attrLayout.layoutPosition = Clockwork.UI_LAYOUT_POSITION_LEFT_TOP;
        attrLayout.layoutSize = Clockwork.UI_LAYOUT_SIZE_AVAILABLE;

        this.contentRoot.addChild(attrLayout);

        for (var i in this.editType.attrInfos) {

            var attr = this.editType.attrInfos[i];

            if (attr.mode & Clockwork.AM_NOEDIT)
                continue;

            var attrEdit = AttributeInfoEdit.createAttrEdit(this.editType, attr);

            if (!attrEdit)
                continue;

            this.attrEdits[attr.name] = attrEdit;

            attrLayout.addChild(attrEdit);

        }

        if (SelectionSection.customSectionUI[this.editType.typeName]) {

            this.customUI = new SelectionSection.customSectionUI[this.editType.typeName]();
            this.customUI.createUI(this.editType);
            attrLayout.addChild(this.customUI);

        }

    }

    static fontDesc: Clockwork.UIFontDescription;

    static customSectionUI: { [typeName: string]: typeof SelectionSectionUI } = {};

    static registerCustomSectionUI(typeName: string, ui: typeof SelectionSectionUI) {

        SelectionSection.customSectionUI[typeName] = ui;

    }


    private static Ctor = (() => {

        var fd = SelectionSection.fontDesc = new Clockwork.UIFontDescription();
        fd.id = "Vera";
        fd.size = 11;

    })();

}

export = SelectionSection;
