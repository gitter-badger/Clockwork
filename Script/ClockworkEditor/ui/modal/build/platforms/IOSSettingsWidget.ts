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

class IOSSettingsWidget extends Clockwork.UIWidget implements BuildSettingsWindow.BuildSettingsWidget {

    constructor() {

        super();

        this.load("ClockworkEditor/editor/ui/buildsettings_ios.tb.txt");

        this.settings = ToolCore.toolSystem.project.buildSettings.iOSBuildSettings;

        this.appNameEdit = <Clockwork.UIEditField>this.getWidget("app_name");
        this.packageNameEdit = <Clockwork.UIEditField>this.getWidget("app_package");
        this.productNameEdit = <Clockwork.UIEditField>this.getWidget("product_name");
        this.companyNameEdit = <Clockwork.UIEditField>this.getWidget("company_name");

        this.provisionPathEdit = <Clockwork.UIEditField>this.getWidget("provision_path");
        this.appIDPrefixField = <Clockwork.UITextField>this.getWidget("appid_prefix");

        this.refreshWidgets();

        this.subscribeToEvent(this, "WidgetEvent", (ev) => this.handleWidgetEvent(ev));

    }

    refreshWidgets() {

        this.appNameEdit.text = this.settings.appName;
        this.packageNameEdit.text = this.settings.packageName;
        this.productNameEdit.text = this.settings.productName;
        this.companyNameEdit.text = this.settings.companyName;

        this.provisionPathEdit.text = this.settings.provisionFile;
        this.appIDPrefixField.text = this.settings.appIDPrefix;

    }

    handleWidgetEvent(ev: Clockwork.UIWidgetEvent): boolean {

        if (ev.type == Clockwork.UI_EVENT_TYPE_CLICK) {

            if (ev.target.id == "choose_provision_path") {

              var fileUtils = new Editor.FileUtils();
              var path = fileUtils.getMobileProvisionPath();

              if (path.length) {

                  var platform = <ToolCore.PlatformIOS> ToolCore.toolSystem.getPlatformByName("IOS");
                  var prefix = platform.parseProvisionAppIdentifierPrefix(path);

                  if (prefix.length) {

                    this.provisionPathEdit.text = path;
                    this.appIDPrefixField.text = prefix;

                  }

              }

              return true;

            }
        }

        return true;
    }

    storeValues() {

        this.settings.appName = this.appNameEdit.text;
        this.settings.packageName = this.packageNameEdit.text;
        this.settings.productName = this.productNameEdit.text;
        this.settings.companyName = this.companyNameEdit.text;

        this.settings.provisionFile = this.provisionPathEdit.text;
        this.settings.appIDPrefix = this.appIDPrefixField.text;

    }

    settings: ToolCore.IOSBuildSettings;

    appNameEdit: Clockwork.UIEditField;
    packageNameEdit: Clockwork.UIEditField;
    productNameEdit: Clockwork.UIEditField;
    companyNameEdit: Clockwork.UIEditField;

    provisionPathEdit: Clockwork.UIEditField;
    appIDPrefixField: Clockwork.UITextField;

}

export = IOSSettingsWidget;
