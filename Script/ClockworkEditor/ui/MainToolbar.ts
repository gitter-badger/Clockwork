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

import EditorUI = require("./EditorUI");
import EditorEvents = require("../editor/EditorEvents");

class MainToolbar extends Clockwork.UIWidget {

    translateButton: Clockwork.UIButton;
    rotateButton: Clockwork.UIButton;
    scaleButton: Clockwork.UIButton;
    axisButton: Clockwork.UIButton;
    playButton: Clockwork.UIButton;

    constructor(parent: Clockwork.UIWidget) {

        super();

        this.load("ClockworkEditor/editor/ui/maintoolbar.tb.txt");

        this.translateButton = <Clockwork.UIButton>this.getWidget("3d_translate");
        this.rotateButton = <Clockwork.UIButton>this.getWidget("3d_rotate");
        this.scaleButton = <Clockwork.UIButton>this.getWidget("3d_scale");

        this.axisButton = <Clockwork.UIButton>this.getWidget("3d_axismode");

        this.playButton = <Clockwork.UIButton>this.getWidget("maintoolbar_play");

        this.translateButton.value = 1;

        parent.addChild(this);

        this.subscribeToEvent("GizmoAxisModeChanged", (ev) => this.handleGizmoAxisModeChanged(ev));
        this.subscribeToEvent("GizmoEditModeChanged", (ev) => this.handleGizmoEditModeChanged(ev));

        this.subscribeToEvent(this, "WidgetEvent", (data) => this.handleWidgetEvent(data));

        this.subscribeToEvent(EditorEvents.PlayerStarted, (data) => {
            var skin = <Clockwork.UISkinImage> this.playButton.getWidget("skin_image");
            skin.setSkinBg("StopButton");
        });
        this.subscribeToEvent(EditorEvents.PlayerStopped, (data) => {
            var skin = <Clockwork.UISkinImage> this.playButton.getWidget("skin_image");
            skin.setSkinBg("PlayButton");
        });
    }

    handleGizmoAxisModeChanged(ev: Editor.GizmoAxisModeChangedEvent) {
        if (ev.mode) {
            this.axisButton.value = 0;
            this.axisButton.text = "Local";
        } else {
            this.axisButton.value = 1;
            this.axisButton.text = "World";
        }
    }

    handleGizmoEditModeChanged(ev: Editor.GizmoEditModeChangedEvent) {

        this.translateButton.value = 0;
        this.rotateButton.value = 0;
        this.scaleButton.value = 0;

        switch (ev.mode) {
            case 1:
                this.translateButton.value = 1;
                break;
            case 2:
                this.rotateButton.value = 1;
                break;
            case 3:
                this.scaleButton.value = 1;
                break;
        }

    }

    handleWidgetEvent(ev: Clockwork.UIWidgetEvent) {

        if (ev.type == Clockwork.UI_EVENT_TYPE_CLICK && ev.target) {

            if (ev.target.id == "3d_translate" || ev.target.id == "3d_rotate" || ev.target.id == "3d_scale") {

                var mode = 1;
                if (ev.target.id == "3d_rotate")
                    mode = 2;
                else if (ev.target.id == "3d_scale")
                    mode = 3;

                this.sendEvent("GizmoEditModeChanged", { mode: mode });

                return true;

            } else if (ev.target.id == "3d_axismode") {

                EditorUI.getShortcuts().toggleGizmoAxisMode();
                return true;

            } else if (ev.target.id == "maintoolbar_play") {
                EditorUI.getShortcuts().invokePlayOrStopPlayer();
                return true;
            }

        }

    }

}



export = MainToolbar;
