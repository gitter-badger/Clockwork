var fs = require('fs-extra');
var path = require("path");
var host = require("./Host");
var os = require('os');
var clockworkRoot = host.clockworkRoot;

var buildDir = host.artifactsRoot + "Build/Android/";

namespace('build', function() {

  task('android_player', ["build:clockworkeditor"], {
    async: true
  }, function() {

    // Clean build
    common.cleanCreateDir(buildDir);

    process.chdir(buildDir);

    var cmds = [];

    var scriptModules = host.getScriptModules("ANDROID");
    var bindCmd = host.clockworkTool + " bind \"" + clockworkRoot + "\" ";

    // Generate bindings for each script package
    for (var pkgName in scriptModules) {
      cmds.push(bindCmd + "Script/Packages/" + pkgName + "/ ANDROID")
    }

    if (os.platform() == "win32") {
      cmds.push(clockworkRoot + "Build/Scripts/Windows/CompileAndroid.bat");
    }
    else {
      cmds.push("cmake -G \"Unix Makefiles\" -DCMAKE_TOOLCHAIN_FILE=../../../Build/CMake/Toolchains/android.toolchain.cmake -DCMAKE_BUILD_TYPE=Release ../../../");
      cmds.push("make -j4");
    }

    jake.exec(cmds, function() {

      var editorAppFolder = host.artifactsRoot + (os.platform() == "win32" ? "ClockworkEditor/" : "ClockworkEditor/ClockworkEditor.app/");

      // Install Deployment
      fs.copySync(buildDir + "Source/ClockworkPlayer/Application/libClockworkPlayer.so",
        editorAppFolder + "Resources/ToolData/Deployment/Android/libs/armeabi-v7a/libClockworkPlayer.so");

      complete();

    }, {
      printStdout: true,
      breakOnError : false
    });

  });

}); // end of build namespace
