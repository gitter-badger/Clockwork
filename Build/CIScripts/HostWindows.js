var fs = require('fs-extra');
var bcommon = require("./BuildCommon");

var buildDir = bcommon.artifactsRoot + "Build/Windows/";
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
      'cmake ../../../../ -DCLOCKWORKTOOL_NOGEN=1 -G "Visual Studio "',
      'xcodebuild -target ClockworkTool -configuration Release'
    ]

    jake.exec(cmds, function() {

      var srcToolBinary = toolBuildDir + "Source/ClockworkTool/Release/ClockworkTool"
      fs.copySync(srcToolBinary, clockworkToolBinary);
      console.log("Built MacOSX ClockworkTool");
      complete();

    }, {
      printStdout: true
    });

  });

}); // end of build namespace


exports.clean = clean;
exports.getClockworkToolBinary = getClockworkToolBinary;
