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

class TextureSelector extends Clockwork.UIWindow {

    constructor(parent: Clockwork.UIWidget) {

        super();

        this.text = "Select Texture";

        this.rect = [0, 0, 320, 512];

        var mainLayout = new Clockwork.UILayout();
        mainLayout.gravity = Clockwork.UI_GRAVITY_ALL;
        mainLayout.layoutDistribution = Clockwork.UI_LAYOUT_DISTRIBUTION_AVAILABLE;
        mainLayout.axis = Clockwork.UI_AXIS_Y;
        this.contentRoot.addChild(mainLayout);

        // really want a grid container
        var scrollContainer = new Clockwork.UIScrollContainer();
        scrollContainer.gravity = Clockwork.UI_GRAVITY_ALL;
        scrollContainer.scrollMode = Clockwork.UI_SCROLL_MODE_Y_AUTO;
        scrollContainer.adaptContentSize = true;

        var scrollLayout = new Clockwork.UILayout();
        scrollLayout.layoutPosition = Clockwork.UI_LAYOUT_POSITION_LEFT_TOP;
        scrollLayout.axis = Clockwork.UI_AXIS_Y;

        scrollContainer.contentRoot.addChild(scrollLayout);

        var db = ToolCore.getAssetDatabase();

        var textures = db.getAssetsByImporterType("TextureImporter", "Texture2D");

        for (var i in textures) {

            var thumbnail = textures[i].cachePath + "_thumbnail.png";
            var cache = Clockwork.getResourceCache();

            var textureWidget = new Clockwork.UITextureWidget();
            textureWidget.texture = <Clockwork.Texture2D> cache.getTempResource("Texture2D", thumbnail);

            var tlp = new Clockwork.UILayoutParams();
            tlp.width = 64;
            tlp.height = 64;
            textureWidget.layoutParams = tlp;

            scrollLayout.addChild(textureWidget);

        }

        mainLayout.addChild(scrollContainer);

        parent.addChild(this);

        this.center();

        this.subscribeToEvent("WidgetEvent", (data) => this.handleWidgetEvent(data));

    }

    handleWidgetEvent(ev: Clockwork.UIWidgetEvent) {

        if (ev.type == Clockwork.UI_EVENT_TYPE_CLICK) {

            if (ev.target != this && !this.isAncestorOf(ev.target)) {

                //this.close();

            }

        }

        return false;

    }

}

export = TextureSelector;
