var fs = require('fs-extra');
var bcommon = require("./BuildCommon");
var host = require("./Host");

var clockworkRoot = bcommon.clockworkRoot;
var srcDir = bcommon.artifactsRoot + "Build/EditorData/";
var dstDir = bcommon.artifactsRoot + "Build/WindowsEditor/";

namespace('build', function() {

  task('genwindowsinstaller', ["build:genwindowseditor"], {
    async: true
  }, function() {

    console.log("Creating Windows Installer");

    bcommon.cleanCreateDir(bcommon.artifactsRoot + "Build/WindowsInstaller");

    var installerName = "ClockworkEditorSetup_" + bcommon.buildSHA + ".exe";
    var installerPath = bcommon.artifactsRoot + "Build/WindowsInstaller/" + installerName;

    var nsisDefines = "/DCLOCKWORK_ROOT=" + clockworkRoot;
    nsisDefines += " /DEDITOR_VERSION=1";
    nsisDefines += " /DINSTALLER_NAME=" + installerName;

    var makeNSISCmd = clockworkRoot + "\\Build\\CIScripts\\Windows\\CreateInstaller.bat";

    makeNSISCmd += " " + nsisDefines + " " + clockworkRoot + "/Build/CIScripts/Windows/Installer/ClockworkEditor.nsi";

    var editorExe = dstDir + "ClockworkEditor/ClockworkEditor.exe";

    var pfxFile = process.env.CLOCKWORK_PFX_FILE;
    var pfxPW = process.env.CLOCKWORK_PFX_PW;

    var signBaseCmd = "signtool.exe sign /f " + pfxFile;
    signBaseCmd += " /p " + pfxPW;
    signBaseCmd += " /t http://timestamp.verisign.com/scripts/timestamp.dll";
    signBaseCmd += " /v ";

    var signEditorCmd = signBaseCmd + editorExe;
    var signInstallerCmd = signBaseCmd + installerPath;

    var cmds = [signEditorCmd, makeNSISCmd, signInstallerCmd];

    jake.exec(cmds, function() {
      complete();
    }, {
      printStdout: true
    });

  });

  task('genwindowseditor', {
    async: true
  }, function() {

    bcommon.cleanCreateDir(dstDir);

    process.chdir(srcDir);

    cmds = [ clockworkRoot + "Build/Windows/7z/7z.exe x -y EditorData.zip"];

    jake.exec(cmds, function() {

      console.log("Generating Windows Editor");

      var editorAppFolder = dstDir + "ClockworkEditor/";

      fs.copySync(srcDir + "EditorBinaries/Windows/ClockworkEditor.exe",
      editorAppFolder + "ClockworkEditor.exe");

      fs.copySync(srcDir + "EditorBinaries/Windows/D3DCompiler_47.dll",
      editorAppFolder + "D3DCompiler_47.dll");

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

      // CEF
      var cefRoot = clockworkRoot + "Submodules/CEF/Windows/64bit/";

      fs.copySync(cefRoot + "Release", editorAppFolder);
      fs.copySync(cefRoot + "Resources", editorAppFolder);

      complete();

    }, {
      printStdout: true
    });

  });

}); // end of build namespace
