var fs = require('fs-extra');
var path = require("path");
var host = require("./Host");
var clockworkRoot = host.clockworkRoot;

var buildDir = host.artifactsRoot + "Build/Linux/";
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
    common.cleanCreateDir(host.getGenScriptRootDir("LINUX"));
  }

  // create the generated script files, so they will be picked up by cmake
  host.createGenScriptFiles("LINUX");

  process.chdir(buildDir);

  var cmds = [];

  cmds.push("cmake ../../../ -DCLOCKWORK_DEV_BUILD=0 -DCMAKE_BUILD_TYPE=Release");
  cmds.push("make -j4 GenerateScriptBindings")
  cmds.push("make -j4 ClockworkEditor ClockworkPlayer")

  jake.exec(cmds, function() {

      // Copy the Editor binaries
      fs.copySync(buildDir + "Source/ClockworkEditor/ClockworkEditor",
        host.artifactsRoot + "ClockworkEditor/ClockworkEditor");

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

      fs.copySync(buildDir +  "Source/ClockworkPlayer/Application/ClockworkPlayer",
        editorAppFolder + "Resources/ToolData/Deployment/Linux/ClockworkPlayer");

      // Copy CEF support files
      fs.copySync(clockworkRoot + "Submodules/CEF/Linux/Resources/icudtl.dat",
        editorAppFolder+"/icudtl.dat");
      fs.copySync(clockworkRoot + "Submodules/CEF/Linux/Release/natives_blob.bin",
        editorAppFolder+"/natives_blob.bin");
      fs.copySync(clockworkRoot + "Submodules/CEF/Linux/Release/snapshot_blob.bin",
        editorAppFolder+"/snapshot_blob.bin");

    console.log("\n\nClockwork Editor build to " + editorAppFolder + "\n\n");

    complete();

  }, {
    printStdout: true
  });

});

});// end of build namespace
