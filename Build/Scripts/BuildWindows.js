var fs = require('fs-extra');
var path = require("path");
var host = require("./Host");
var clockworkRoot = host.clockworkRoot;

var buildDir = host.artifactsRoot + "Build/Windows/";
var editorAppFolder = host.artifactsRoot + "ClockworkEditor/";

namespace('build', function() {

  // Builds a standalone Clockwork Editor, which can be distributed out of build tree
  task('clockworkeditor', {
    async: true
  }, function() {

    // Clean build
    var cleanBuild = true;
    if (cleanBuild) {
      common.cleanCreateDir(buildDir);
      common.cleanCreateDir(editorAppFolder);
      common.cleanCreateDir(host.getGenScriptRootDir("WINDOWS"));
    }

    // create the generated script files, so they will be picked up by cmake
    host.createGenScriptFiles("WINDOWS");

    process.chdir(buildDir);

    var cmds = [];

    // Build the ClockworkEditor
    cmds.push(clockworkRoot + "Build/Scripts/Windows/CompileClockworkEditor.bat");

    jake.exec(cmds, function() {

      // Copy the Editor binaries
      fs.copySync(buildDir + "Source/ClockworkEditor/Release",
        host.artifactsRoot + "ClockworkEditor");

      // We need some resources to run
      fs.copySync(clockworkRoot + "Resources/CoreData",
        editorAppFolder + "Resources/CoreData");

      fs.copySync(clockworkRoot + "Resources/PlayerData",
        editorAppFolder + "Resources/PlayerData");

      fs.copySync(clockworkRoot + "Data/ClockworkEditor",
        editorAppFolder + "Resources/ToolData");

      fs.copySync(clockworkRoot + "Resources/EditorData",
        editorAppFolder + "Resources/EditorData");

      fs.copySync(clockworkRoot + "Artifacts/Build/Resources/EditorData/ClockworkEditor/EditorScripts",
        editorAppFolder + "Resources/EditorData/ClockworkEditor/EditorScripts");

      fs.copySync(buildDir +  "Source/ClockworkPlayer/Application/Release/ClockworkPlayer.exe",
        editorAppFolder + "Resources/ToolData/Deployment/Windows/x64/ClockworkPlayer.exe");

      fs.copySync(buildDir +  "Source/ClockworkPlayer/Application/Release/D3DCompiler_47.dll",
        editorAppFolder + "Resources/ToolData/Deployment/Windows/x64/D3DCompiler_47.dll");

      console.log("Clockwork Editor build to ", editorAppFolder);

      complete();

    }, {
      printStdout: true
    });

  });

  // Generate a Visual Studio 2015 solution
  task('genvs2015', {
    async: true
  }, function() {

    var slnRoot = path.resolve(clockworkRoot, "") + "-VS2015\\";

    if (!fs.existsSync(slnRoot)) {
        jake.mkdirP(slnRoot);
    }

    // create the generated script files, so they will be picked up by cmake
    host.createGenScriptFiles("WINDOWS");

    process.chdir(slnRoot);

    var cmds = [];

    cmds.push(clockworkRoot + "Build/Scripts/Windows/GenerateVS2015.bat " + clockworkRoot);

    jake.exec(cmds, function() {

      var task = jake.Task['build:genscripts']

      task.addListener('complete', function () {
          console.log("\n\nVisual Studio Solution generated in ", slnRoot);
          complete();
        });

      task.invoke("WINDOWS");

    }, {
      printStdout: true
    });

  });

}); // end of build namespace
