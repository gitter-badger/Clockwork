var fs = require('fs-extra');
var bcommon = require("./BuildCommon");
var host = require("./Host");

var buildDir = bcommon.artifactsRoot + "Build/Android/";
var playerBuildDir = buildDir + "ClockworkPlayer/";

namespace('build', function() {

  task('android_player', {
    async: true
  }, function() {

    bcommon.cleanCreateDir(playerBuildDir);
    bcommon.cleanCreateDir(bcommon.getGenScriptRootDir("ANDROID"));

    var clockworkTool = host.getClockworkToolBinary();

    process.chdir(playerBuildDir);

    var cmds = [
      clockworkTool + " bind " + bcommon.clockworkRoot + " Script/Packages/Clockwork/ ANDROID",
      clockworkTool + " bind " + bcommon.clockworkRoot + " Script/Packages/ClockworkPlayer/ ANDROID"
    ];

    cmds.push("cmake -G \"Unix Makefiles\" -DCMAKE_TOOLCHAIN_FILE=" + bcommon.clockworkRoot + "Build//CMake/Toolchains/android.toolchain.cmake -DCMAKE_BUILD_TYPE=Release ../../../../");
    cmds.push("make -j4");

    jake.exec(cmds, function() {

      var androidPlayerBinary = playerBuildDir + "Source/ClockworkPlayer/Application/libClockworkPlayer.so";
      fs.copySync(androidPlayerBinary, buildDir + "Bin/libClockworkPlayer.so");

      console.log("Built Android Player");

      complete();

    }, {
      printStdout: true
    });

  });

}); // end of build namespace
