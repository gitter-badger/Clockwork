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

import BuildSettingsWindow = require("../BuildSettingsWindow");

class AndroidSettingsWidget extends Clockwork.UIWidget implements BuildSettingsWindow.BuildSettingsWidget {

    constructor() {

        super();

        this.load("ClockworkEditor/editor/ui/buildsettings_android.tb.txt");

        this.settings = ToolCore.toolSystem.project.buildSettings.androidBuildSettings;

        this.sdkTargetSelect = <Clockwork.UISelectDropdown>this.getWidget("sdk_target_select");
        this.appNameEdit = <Clockwork.UIEditField>this.getWidget("app_name");
        this.packageNameEdit = <Clockwork.UIEditField>this.getWidget("app_package");
        this.productNameEdit = <Clockwork.UIEditField>this.getWidget("product_name");
        this.companyNameEdit = <Clockwork.UIEditField>this.getWidget("company_name");


        this.jdkRootChooseButton = <Clockwork.UIButton>this.getWidget("choose_jdk_root");
        this.jdkRootEdit = <Clockwork.UIEditField>this.getWidget("jdk_root");

        var jdkRootText = <Clockwork.UITextField>this.getWidget("jdk_root_text");
        var antPathText = <Clockwork.UITextField>this.getWidget("ant_path_text");

        if (Clockwork.platform == "Windows") {

            jdkRootText.text = "JDK Root: (Ex. C:\\Program Files\\Java\\jdk1.8.0_31)";
            antPathText.text = "Ant Path: (The folder that contains ant.bat)";

        } else {

            jdkRootText.visibility = Clockwork.UI_WIDGET_VISIBILITY_GONE;
            this.jdkRootChooseButton.visibility = Clockwork.UI_WIDGET_VISIBILITY_GONE;
            this.jdkRootEdit.visibility = Clockwork.UI_WIDGET_VISIBILITY_GONE;

        }

        this.antPathEdit = <Clockwork.UIEditField>this.getWidget("ant_path");

        this.refreshWidgets();

        this.subscribeToEvent(this, "WidgetEvent", (ev) => this.handleWidgetEvent(ev));

    }

    handleWidgetEvent(ev: Clockwork.UIWidgetEvent): boolean {

        if (ev.type == Clockwork.UI_EVENT_TYPE_CLICK) {

            if (ev.target.id == "choose_sdk_path") {

                var fileUtils = new Editor.FileUtils();
                var path = fileUtils.getAndroidSDKPath("");

                if (path.length) {

                    var toolPrefs = ToolCore.toolEnvironment.toolPrefs;
                    if (toolPrefs.androidSDKPath != path) {
                        toolPrefs.androidSDKPath = path;
                        toolPrefs.save();
                    }

                    this.refreshWidgets();

                }

                return true;

            } else if (ev.target.id == "choose_ant_path") {

                var fileUtils = new Editor.FileUtils();
                var path = fileUtils.getAntPath("");

                if (path.length) {

                    var toolPrefs = ToolCore.toolEnvironment.toolPrefs;
                    if (toolPrefs.antPath != path) {
                        toolPrefs.antPath = path;
                        toolPrefs.save();
                    }

                    this.refreshWidgets();

                }

                return true;

            }  else if (ev.target.id == "choose_jdk_root") {

                var fileUtils = new Editor.FileUtils();
                var path = fileUtils.getJDKRootPath("");

                if (path.length) {

                    var toolPrefs = ToolCore.toolEnvironment.toolPrefs;
                    if (toolPrefs.jDKRootPath != path) {
                        toolPrefs.jDKRootPath = path;
                        toolPrefs.save();
                    }

                    this.refreshWidgets();

                }

                return true;

            } else if (ev.target.id == "refresh_sdk_targets") {

                this.refreshAndroidTargets();

            }

        }

        return false;
    }

    refreshAndroidTargets() {

        var platform = <ToolCore.PlatformAndroid>ToolCore.toolSystem.getPlatformByName("ANDROID");

        platform.refreshAndroidTargets();

        this.subscribeToEvent(platform, "AndroidTargetsRefreshed", (ev) => {

            this.sdkTargetSource.clear();

            var targets: string[] = platform.androidTargets;

            for (var i in targets) {

                this.sdkTargetSource.addItem(new Clockwork.UISelectItem(targets[i]));
            }

            this.sdkTargetSelect.source = this.sdkTargetSource;

            // force a refresh
            this.sdkTargetSelect.value = -1;
            this.sdkTargetSelect.value = 0;

        });

    }

    refreshWidgets() {

        var sdkPathEdit = <Clockwork.UIEditField>this.getWidget("sdk_path");
        var antPathEdit = <Clockwork.UIEditField>this.getWidget("ant_path");
        var jdkRootEdit = <Clockwork.UIEditField>this.getWidget("jdk_root");

        var toolPrefs = ToolCore.toolEnvironment.toolPrefs;

        sdkPathEdit.text = toolPrefs.androidSDKPath;
        antPathEdit.text = toolPrefs.antPath;
        jdkRootEdit.text = toolPrefs.jDKRootPath;

        this.appNameEdit.text = this.settings.appName;
        this.packageNameEdit.text = this.settings.packageName;
        this.productNameEdit.text = this.settings.productName;
        this.companyNameEdit.text = this.settings.companyName;

        this.sdkTargetSelect.text = this.settings.sDKVersion;

    }

    storeValues() {

        this.settings.appName = this.appNameEdit.text;
        this.settings.packageName = this.packageNameEdit.text;
        this.settings.productName = this.productNameEdit.text;
        this.settings.companyName = this.companyNameEdit.text;

        if (this.antPathEdit.text.length && this.antPathEdit.text != ToolCore.toolEnvironment.toolPrefs.antPath)
            ToolCore.toolEnvironment.toolPrefs.antPath = this.antPathEdit.text;

        this.settings.sDKVersion = this.sdkTargetSelect.text;

    }

    settings: ToolCore.AndroidBuildSettings;
    sdkTargetSource: Clockwork.UISelectItemSource = new Clockwork.UISelectItemSource();
    sdkTargetSelect: Clockwork.UISelectDropdown;

    jdkRootChooseButton: Clockwork.UIButton;
    jdkRootEdit: Clockwork.UIEditField;

    antPathEdit: Clockwork.UIEditField;

    appNameEdit: Clockwork.UIEditField;
    packageNameEdit: Clockwork.UIEditField;
    productNameEdit: Clockwork.UIEditField;
    companyNameEdit: Clockwork.UIEditField;


}

export = AndroidSettingsWidget;
