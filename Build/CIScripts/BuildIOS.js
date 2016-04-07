var fs = require('fs-extra');
var bcommon = require("./BuildCommon");
var host = require("./Host");

var buildDir = bcommon.artifactsRoot + "Build/IOS/";

namespace('build', function() {

  task('ios_player', {
    async: true
  }, function() {

    var clockworkTool = host.getClockworkToolBinary();

    var playerBuildDir = buildDir + "ClockworkPlayer/";

    bcommon.cleanCreateDir(playerBuildDir);
    bcommon.cleanCreateDir(bcommon.getGenScriptRootDir("IOS"));

    process.chdir(playerBuildDir);

    var cmds = [
      clockworkTool + " bind " + bcommon.clockworkRoot + " Script/Packages/Clockwork/ IOS",
      clockworkTool + " bind " + bcommon.clockworkRoot + " Script/Packages/ClockworkPlayer/ IOS",
      "cmake -DIOS=1 -G Xcode ../../../../"
    ];

    if (bcommon.jenkinsBuild) {

      cmds.push("security -v list-keychains -d system -s /Users/jenkins/Library/Keychains/codesign.keychain");
      cmds.push("security -v unlock-keychain /Users/jenkins/Library/Keychains/codesign.keychain");

    }

    cmds.push("xcodebuild -configuration Release -parallelizeTargets -jobs 4");

    jake.exec(cmds, function() {
      var iosPlayerBinary = playerBuildDir + "Source/ClockworkPlayer/Application/Release-iphoneos/ClockworkPlayer.app/ClockworkPlayer";
      fs.copySync(iosPlayerBinary, buildDir + "Bin/ClockworkPlayer");
      console.log("Built IOS Player");
      complete();

    }, {
      printStdout: true
    });

  });

}); // end of build namespace
