var fs = require('fs-extra');
var bcommon = require("./BuildCommon");
var host = require("./Host");

var buildDir = bcommon.artifactsRoot + "Build/Mac/";

namespace('build', function() {

  task('mac_player', {
    async: true
  }, function() {

    var clockworkTool = host.getClockworkToolBinary();

    var playerBuildDir = buildDir + "ClockworkPlayer/";

    bcommon.cleanCreateDir(playerBuildDir);
    bcommon.cleanCreateDir(bcommon.getGenScriptRootDir("MACOSX"));

    process.chdir(playerBuildDir);

    var cmds = [
      clockworkTool + " bind " + bcommon.clockworkRoot + " Script/Packages/Clockwork/ MACOSX",
      clockworkTool + " bind " + bcommon.clockworkRoot + " Script/Packages/ClockworkPlayer/ MACOSX",
      "cmake -DCLOCKWORK_DEV_BUILD=0 -G Xcode ../../../../",
      "xcodebuild -target ClockworkPlayer -configuration Release -parallelizeTargets -jobs 4"
    ];

    jake.exec(cmds, function() {
      var macPlayerBinary = playerBuildDir + "Source/ClockworkPlayer/Application/Release/ClockworkPlayer.app/Contents/MacOS/ClockworkPlayer";
      fs.copySync(macPlayerBinary, buildDir + "Bin/ClockworkPlayer");
      console.log("Built Mac Player");
      complete();

    }, {
      printStdout: true
    });

  });

  task('mac_editor', {
    async: true
  }, function() {

    var clockworkTool = host.getClockworkToolBinary();

    var editorBuildDir = buildDir + "ClockworkEditor/";

    bcommon.cleanCreateDir(editorBuildDir);

    process.chdir(editorBuildDir);

    var cmds = [
      clockworkTool + " bind " + bcommon.clockworkRoot + " Script/Packages/Clockwork/ MACOSX",
      clockworkTool + " bind " + bcommon.clockworkRoot + " Script/Packages/ClockworkPlayer/ MACOSX",
      clockworkTool + " bind " + bcommon.clockworkRoot + " Script/Packages/ToolCore/ MACOSX",
      clockworkTool + " bind " + bcommon.clockworkRoot + " Script/Packages/Editor/ MACOSX",
      clockworkTool + " bind " + bcommon.clockworkRoot + " Script/Packages/ClockworkNET/ MACOSX",
      clockworkTool + " bind " + bcommon.clockworkRoot + " Script/Packages/WebView/ MACOSX",
      "cmake -DCLOCKWORK_DEV_BUILD=0 -G Xcode ../../../../",
      "xcodebuild -target ClockworkEditor -configuration Release -parallelizeTargets -jobs 4",
      "cd " + editorBuildDir + "Source/ClockworkEditor/Release && zip -r -X " + buildDir + "Bin/ClockworkEditor.zip ./ClockworkEditor.app"
    ];

    jake.exec(cmds, function() {

      console.log("Built Mac Editor");
      complete();

    }, {
      printStdout: true
    });

  });

}); // end of build namespace
