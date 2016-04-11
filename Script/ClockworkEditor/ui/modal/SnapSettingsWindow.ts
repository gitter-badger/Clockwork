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

import ModalWindow = require("./ModalWindow");

class SnapSettingsWindow extends ModalWindow {

    constructor() {

        super();

        this.init("Snap Settings", "ClockworkEditor/editor/ui/snapsettings.tb.txt");

        this.transXEditField = <Clockwork.UIEditField>this.getWidget("trans_x");
        this.transYEditField = <Clockwork.UIEditField>this.getWidget("trans_y");
        this.transZEditField = <Clockwork.UIEditField>this.getWidget("trans_z");
        this.rotateEditField = <Clockwork.UIEditField>this.getWidget("rotation");
        this.scaleEditField = <Clockwork.UIEditField>this.getWidget("scale");

        this.refreshWidgets();

    }

    apply() {

        var userPrefs = ToolCore.toolSystem.project.userPrefs;
        userPrefs.snapTranslationX = Number(this.transXEditField.text);
        userPrefs.snapTranslationY = Number(this.transYEditField.text);
        userPrefs.snapTranslationZ = Number(this.transZEditField.text);
        userPrefs.snapRotation = Number(this.rotateEditField.text);
        userPrefs.snapScale = Number(this.scaleEditField.text);

        ToolCore.toolSystem.project.saveUserPrefs();

    }

    refreshWidgets() {

        var userPrefs = ToolCore.toolSystem.project.userPrefs;

        this.transXEditField.text = parseFloat(userPrefs.snapTranslationX.toFixed(5)).toString();
        this.transYEditField.text = parseFloat(userPrefs.snapTranslationY.toFixed(5)).toString();
        this.transZEditField.text = parseFloat(userPrefs.snapTranslationZ.toFixed(5)).toString();
        this.rotateEditField.text = parseFloat(userPrefs.snapRotation.toFixed(5)).toString();
        this.scaleEditField.text = parseFloat(userPrefs.snapScale.toFixed(5)).toString();

    }

    handleWidgetEvent(ev: Clockwork.UIWidgetEvent) {

        if (ev.type == Clockwork.UI_EVENT_TYPE_CLICK) {

            var id = ev.target.id;

            if (id == "apply") {

                this.apply();
                this.hide();
                return true;

            }

            if (id == "cancel") {

                this.hide();

                return true;
            }

        }

    }

    transXEditField: Clockwork.UIEditField;
    transYEditField: Clockwork.UIEditField;
    transZEditField: Clockwork.UIEditField;
    rotateEditField: Clockwork.UIEditField;
    scaleEditField: Clockwork.UIEditField;

}

export = SnapSettingsWindow;
