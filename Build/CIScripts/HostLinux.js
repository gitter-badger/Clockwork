var fs = require('fs-extra');
var bcommon = require("./BuildCommon");

var buildDir = bcommon.artifactsRoot + "Build/Linux/";
var clockworkToolBinary = buildDir + "Bin/ClockworkTool";

function clean() {

  bcommon.testRemoveDir(buildDir);

}

function getClockworkToolBinary() {

  return clockworkToolBinary;

}

namespace('build', function() {

  task('clockworktool', {
    async: true
  }, function() {

    var toolBuildDir = buildDir + "ClockworkTool/";

    bcommon.cleanCreateDir(toolBuildDir);

    process.chdir(toolBuildDir);

    var cmds = [
        cmds.push("cmake ../../../ -DCLOCKWORK_DEV_BUILD=0 -DCMAKE_BUILD_TYPE=Release -DCLOCKWORK_BUILD_2D=0 -DLINUX=1 ");
        cmds.push("make ClockworkTool")
    ]

    jake.exec(cmds, function() {

      var srcToolBinary = toolBuildDir + "Source/ClockworkTool/Release/ClockworkTool"
      fs.copySync(srcToolBinary, clockworkToolBinary);
      console.log("Built Linux ClockworkTool");
      complete();

    }, {
      printStdout: true
    });

  });

}); // end of build namespace


exports.clean = clean;
exports.getClockworkToolBinary = getClockworkToolBinary;
