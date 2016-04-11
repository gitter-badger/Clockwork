var fs = require('fs-extra');
var bcommon = require("./BuildCommon");
var host = require("./Host");

var clockworkRoot = bcommon.clockworkRoot;
var srcDir = bcommon.artifactsRoot + "Build/EditorData/";
var dstDir = bcommon.artifactsRoot + "Build/MacEditor/";

namespace('build', function() {

  task('genmaceditorinstaller', ["genmaceditor"], {
    async: true
  }, function() {

      bcommon.cleanCreateDir(dstDir + "MacEditorInstaller");

      var editorAppFolder = dstDir + "ClockworkEditor.app";

      cmds = [
      "chmod +x " + editorAppFolder + "/Contents/MacOS/ClockworkEditor",
      "chmod +x " + editorAppFolder + "/Contents/Resources/ToolData/Deployment/MacOS/ClockworkPlayer.app/Contents/MacOS/ClockworkPlayer",
      "security unlock-keychain -p \"jenkins\" /Users/jenkins/Library/Keychains/login.keychain  ",
      "codesign --force --verify --verbose --sign \"Developer ID Application: THUNDERBEAST GAMES LLC (C7M5MAQQWR)\" " + editorAppFolder + "/Contents/Frameworks/ClockworkEditor\\ Helper.app",
      "codesign --force --verify --verbose --sign \"Developer ID Application: THUNDERBEAST GAMES LLC (C7M5MAQQWR)\" " + editorAppFolder + "/Contents//Frameworks/ClockworkEditor\\ Helper\\ EH.app",
      "codesign --force --verify --verbose --sign \"Developer ID Application: THUNDERBEAST GAMES LLC (C7M5MAQQWR)\" " + editorAppFolder + "/Contents/Frameworks/ClockworkEditor\\ Helper\\ NP.app",
      "codesign --force --verify --verbose --sign \"Developer ID Application: THUNDERBEAST GAMES LLC (C7M5MAQQWR)\" " + editorAppFolder + "/Contents/Frameworks/Chromium\\ Embedded\\ Framework.framework",
      "codesign --force --verify --verbose --sign \"Developer ID Application: THUNDERBEAST GAMES LLC (C7M5MAQQWR)\" " + editorAppFolder,      
      "cd " + dstDir + " && zip -r -X " + "./MacEditorInstaller/ClockworkEditor_MacOSX_" + bcommon.buildSHA + ".zip ./ClockworkEditor.app"
    ];

    jake.exec(cmds, function() {

      console.log("Built Mac Editor");

      complete();

    }, {
      printStdout: true
    });


  });

  task('genmaceditor', {
    async: true
  }, function() {

    bcommon.cleanCreateDir(dstDir);

    cmds = ["unzip " + srcDir + "EditorData.zip -d " + srcDir,
            "unzip " + srcDir + "EditorBinaries/Mac/ClockworkEditor.zip -d " + srcDir + "EditorBinaries/Mac"];

    jake.exec(cmds, function() {

      console.log("Generating Mac App Bundle");

      var editorAppFolder = dstDir + "ClockworkEditor.app/Contents/";

      fs.copySync(srcDir + "EditorBinaries/Mac/ClockworkEditor.app",
        dstDir + "ClockworkEditor.app");

      fs.copySync(srcDir + "Resources/CoreData",
        editorAppFolder + "Resources/CoreData");

      fs.copySync(srcDir + "Resources/EditorData",
        editorAppFolder + "Resources/EditorData");

      fs.copySync(srcDir + "Resources/PlayerData",
        editorAppFolder + "Resources/PlayerData");

      fs.copySync(srcDir + "Resources/ToolData",
        editorAppFolder + "Resources/ToolData");

      fs.copySync(srcDir + "ClockworkExamples",
        editorAppFolder + "Resources/ToolData/ClockworkExamples");

      fs.copySync(srcDir + "Docs",
        editorAppFolder + "Resources/ToolData/Docs/JSDocs");

      complete();

    }, {
      printStdout: true
    });

  });

}); // end of build namespace
