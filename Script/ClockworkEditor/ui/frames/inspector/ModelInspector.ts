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

import InspectorWidget = require("./InspectorWidget");
import ArrayEditWidget = require("./ArrayEditWidget");
import InspectorUtils = require("./InspectorUtils");
import EditorEvents = require("editor/EditorEvents");

class ModelInspector extends InspectorWidget {

    constructor() {

        super();

        this.subscribeToEvent(this, "WidgetEvent", (data) => this.handleWidgetEvent(data));

    }

    handleWidgetEvent(ev: Clockwork.UIWidgetEvent):boolean {

      return false;

    }

    onApply() {

        this.importer.scale = Number(this.scaleEdit.text);

        this.importer.importAnimations = this.importAnimationBox.value ? true : false;

        for (var i = 0; i < this.importer.animationCount; i++) {

          var info = this.importer.getAnimationInfo(i);

          var nameEdit = this.nameEdits[i];
          var startEdit = this.startEdits[i];
          var endEdit = this.endEdits[i];

          info.name = nameEdit.text;

          // guard against NAN
          var _startTime = Number(startEdit.text);
          var _endTime = Number(endEdit.text);

          if (isNaN(_startTime)) _startTime = 0;
          if (isNaN(_endTime)) _endTime = 0;

          info.startTime = _startTime;
          info.endTime = _endTime;

        }

        this.asset.import();
        this.asset.save();

    }

    inspect(asset: ToolCore.Asset) {

        this.asset = asset;
        this.importer = <ToolCore.ModelImporter> asset.importer;

        // node attr layout
        var rootLayout = this.rootLayout;

        // Model Section
        var modelLayout = this.createSection(rootLayout, "Model", 1);

        var editField = InspectorUtils.createAttrEditField("Name", modelLayout);

        var lp = new Clockwork.UILayoutParams();
        editField.readOnly = true;
        editField.text = asset.name;

        //This should preferably be onClick
        editField.subscribeToEvent(editField, "UIWidgetFocusChanged", (ev: Clockwork.UIWidgetFocusChangedEvent) => {

            if (ev.widget == editField && editField.focus) {
                this.sendEvent(EditorEvents.InspectorProjectReference, { "path": asset.getRelativePath() });
            }

        });

        this.scaleEdit = InspectorUtils.createAttrEditField("Scale", modelLayout);
        this.scaleEdit.text = this.importer.scale.toString();

        // Animations Section
        var animationLayout = this.createSection(rootLayout, "Animation", 1);

        this.importAnimationBox = this.createAttrCheckBox("Import Animations", animationLayout);
        this.importAnimationBox.value = this.importer.importAnimations ? 1 : 0;

        this.importAnimationArray = new ArrayEditWidget("Animation Count");
        animationLayout.addChild(this.importAnimationArray);

        this.importAnimationArray.onCountChanged = (count) => this.onAnimationCountChanged(count);

        var nlp = new Clockwork.UILayoutParams();
        nlp.width = 310;

        var animLayout = this.animationInfoLayout = new Clockwork.UILayout();

        animLayout.spacing = 4;

        animLayout.layoutDistribution = Clockwork.UI_LAYOUT_DISTRIBUTION_GRAVITY;
        animLayout.layoutPosition = Clockwork.UI_LAYOUT_POSITION_LEFT_TOP;
        animLayout.layoutParams = nlp;
        animLayout.axis = Clockwork.UI_AXIS_Y;
        animLayout.gravity = Clockwork.UI_GRAVITY_ALL;

        animationLayout.addChild(animLayout);

        this.createAnimationEntries();

        // apply button
        rootLayout.addChild(this.createApplyButton());

    }

    createAnimationEntries() {

        var layout = this.animationInfoLayout;
        layout.deleteAllChildren();

        var count = this.importer.animationCount;

        this.importAnimationArray.countEdit.text = count.toString();

        this.nameEdits = [];
        this.startEdits = [];
        this.endEdits = [];

        for (var i = 0; i < count; i++) {

            var animInfo = this.importer.getAnimationInfo(i);

            var name = InspectorUtils.createAttrName("Animation " + i.toString() + ":");
            layout.addChild(name);

            var nameEdit = InspectorUtils.createAttrEditField("Name", layout);
            nameEdit.text = animInfo.name;

            var startEdit = InspectorUtils.createAttrEditField("Start", layout);
            startEdit.text = animInfo.startTime.toString();

            var endEdit = InspectorUtils.createAttrEditField("End", layout);
            endEdit.text = animInfo.endTime.toString();

            this.nameEdits.push(nameEdit);
            this.startEdits.push(startEdit);
            this.endEdits.push(endEdit);

            InspectorUtils.createSeparator(layout);

        }

    }

    onAnimationCountChanged(count: number) {

        if (this.importer.animationCount == count) {
            return;
        }

        this.importer.animationCount = count;

        this.createAnimationEntries();

    }

    // model
    scaleEdit: Clockwork.UIEditField;

    // animation
    importAnimationBox: Clockwork.UICheckBox;
    importAnimationArray: ArrayEditWidget;
    animationInfoLayout: Clockwork.UILayout;

    nameEdits: Clockwork.UIEditField[];
    startEdits: Clockwork.UIEditField[];
    endEdits: Clockwork.UIEditField[];

    asset: ToolCore.Asset;
    importer: ToolCore.ModelImporter;

}

export = ModelInspector;
