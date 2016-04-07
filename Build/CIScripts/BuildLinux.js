var fs = require('fs-extra');
var bcommon = require("./BuildCommon");
var host = require("./Host");

var buildDir = bcommon.artifactsRoot + "Build/Linux/";

namespace('build', function() {

  task('linux_player', {
    async: true
  }, function() {

    var clockworkTool = host.getClockworkToolBinary();

    var playerBuildDir = buildDir + "ClockworkPlayer/";

    bcommon.cleanCreateDir(playerBuildDir);
    bcommon.cleanCreateDir(bcommon.getGenScriptRootDir("LINUX"));

    process.chdir(playerBuildDir);

    var cmds = [
      clockworkTool + " bind " + bcommon.clockworkRoot + " Script/Packages/Clockwork/ LINUX",
      clockworkTool + " bind " + bcommon.clockworkRoot + " Script/Packages/ClockworkPlayer/ LINUX",
      "cmake -DCLOCKWORK_DEV_BUILD=0 -DLINUX=1 ../../../../",
      "make"
    ];

    jake.exec(cmds, function() {
      var macPlayerBinary = playerBuildDir + "Source/ClockworkPlayer/Application/Release/Contents/Linux/ClockworkPlayer";
      fs.copySync(macPlayerBinary, buildDir + "Bin/ClockworkPlayer");
      console.log("Built Linux Player");
      complete();

    }, {
      printStdout: true
    });

  });

  task('linux_editor', {
    async: true
  }, function() {

    var clockworkTool = host.getClockworkToolBinary();

    var editorBuildDir = buildDir + "ClockworkEditor/";

    bcommon.cleanCreateDir(editorBuildDir);

    process.chdir(editorBuildDir);

    var cmds = [
      clockworkTool + " bind " + bcommon.clockworkRoot + " Script/Packages/Clockwork/ LINUX",
      clockworkTool + " bind " + bcommon.clockworkRoot + " Script/Packages/ClockworkPlayer/ LINUX",
      clockworkTool + " bind " + bcommon.clockworkRoot + " Script/Packages/ToolCore/ LINUX",
      clockworkTool + " bind " + bcommon.clockworkRoot + " Script/Packages/Editor/ LINUX",
      clockworkTool + " bind " + bcommon.clockworkRoot + " Script/Packages/ClockworkNET/ LINUX",
      clockworkTool + " bind " + bcommon.clockworkRoot + " Script/Packages/WebView/ LINUX",
      "cmake -DCLOCKWORK_DEV_BUILD=0 -DLINUX=1 ../../../../",
      "make"
    ];

    jake.exec(cmds, function() {
      var macEditorBinary = editorBuildDir + "Source/ClockworkEditor/Release/Contents/Linux/ClockworkEditor";
      fs.copySync(macEditorBinary, buildDir + "Bin/ClockworkEditor");
      console.log("Built Linux Editor");
      complete();

    }, {
      printStdout: true
    });

  });

}); // end of build namespace
