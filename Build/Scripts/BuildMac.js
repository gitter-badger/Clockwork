var fs = require('fs-extra');
var path = require("path");
var host = require("./Host");
var clockworkRoot = host.clockworkRoot;

var buildDir = host.artifactsRoot + "Build/Mac/";
var editorAppFolder = host.artifactsRoot + "/ClockworkEditor/ClockworkEditor.app/";

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
    common.cleanCreateDir(host.getGenScriptRootDir("MACOSX"));
  }

  // create the generated script files, so they will be picked up by cmake
  host.createGenScriptFiles("MACOSX");

  process.chdir(buildDir);

  var cmds = [];

  cmds.push("cmake ../../../ -DCLOCKWORK_DEV_BUILD=0 -G Xcode");
  cmds.push("xcodebuild -target GenerateScriptBindings -configuration Release -parallelizeTargets -jobs 4")
  cmds.push("xcodebuild -target ClockworkEditor -target ClockworkPlayer -configuration Release -parallelizeTargets -jobs 4")

  jake.exec(cmds, function() {

    fs.copySync(buildDir + "Source/ClockworkEditor/Release/ClockworkEditor.app", editorAppFolder);

    var resourceDest = editorAppFolder + "/Contents/Resources/"

    // We need some resources to run
    fs.copySync(clockworkRoot + "Resources/CoreData",
      resourceDest + "CoreData");

    fs.copySync(clockworkRoot + "Resources/PlayerData",
      resourceDest + "PlayerData");

    fs.copySync(clockworkRoot + "Data/ClockworkEditor",
      resourceDest + "ToolData");

    fs.copySync(clockworkRoot + "Resources/EditorData",
      resourceDest + "EditorData");

    fs.copySync(clockworkRoot + "Artifacts/Build/Resources/EditorData/ClockworkEditor/EditorScripts",
      resourceDest + "EditorData/ClockworkEditor/EditorScripts");

    // copy the mac player binary to deployment
    var playerBinary =  buildDir +  "Source/ClockworkPlayer/Application/Release/ClockworkPlayer.app/Contents/MacOS/ClockworkPlayer";

    fs.copySync(playerBinary,
      resourceDest + "ToolData/Deployment/MacOS/ClockworkPlayer.app/Contents/MacOS/ClockworkPlayer");

    console.log("\n\nClockwork Editor build to " + editorAppFolder + "\n\n");

    complete();

  }, {
    printStdout: true
  });

});

// Generate a XCode Workspace
task('genxcode', {
  async: true
}, function() {

  var xcodeRoot = path.resolve(clockworkRoot, "") + "-XCode";

  if (!fs.existsSync(xcodeRoot)) {
      jake.mkdirP(xcodeRoot);
  }

  // create the generated script files, so they will be picked up by cmake
  host.createGenScriptFiles("MACOSX");

  process.chdir(xcodeRoot);

  var cmds = [];

  cmds.push("cmake ../ClockworkGameEngine -DCLOCKWORK_DEV_BUILD=1 -G Xcode");
  cmds.push("xcodebuild -target GenerateScriptBindings -configuration Debug")

  jake.exec(cmds, function() {

    var task = jake.Task['build:genscripts']

    task.addListener('complete', function () {
        console.log("\n\nXCode workspace generated in " +  xcodeRoot + "\n\n");
        complete();
      });

    task.invoke("MACOSX");

  }, {
    printStdout: true
  });

});


});// end of build namespace
