var fs = require('fs-extra');
var bcommon = require("./BuildCommon");
var host = require("./Host");

var clockworkRoot = bcommon.clockworkRoot;
var buildDir = bcommon.artifactsRoot + "Build/Windows/";
var clockworkToolBinary = buildDir + "Source/ClockworkTool/Release/ClockworkTool.exe";

namespace('build', function() {

  task('clockwork', {
    async: true
  }, function() {

    bcommon.cleanCreateDir(buildDir);
    bcommon.cleanCreateDir(bcommon.getGenScriptRootDir("WINDOWS"));

    process.chdir(buildDir);

    var bindCmd = clockworkToolBinary + " bind \"" + clockworkRoot + "\" ";

    var cmds = [
      clockworkRoot + "/Build/CIScripts/Windows/CompileClockworkTool.bat",
      bindCmd + "Script/Packages/Clockwork/ WINDOWS",
      bindCmd + "Script/Packages/ClockworkPlayer/ WINDOWS",
      bindCmd + "Script/Packages/ToolCore/ WINDOWS",
      bindCmd + "Script/Packages/Editor/ WINDOWS",
      bindCmd + "Script/Packages/ClockworkNET/ WINDOWS",
      bindCmd + "Script/Packages/WebView/ WINDOWS",
      clockworkRoot + "/Build/CIScripts/Windows/CompileClockwork.bat",
    ]

    jake.exec(cmds, function() {

      var winEditorBinary = buildDir + "Source/ClockworkEditor/Release/ClockworkEditor.exe";
      var winEditorD3DBinary = buildDir + "Source/ClockworkEditor/Release/D3DCompiler_47.dll";
      var winPlayerBinary = buildDir + "Source/ClockworkPlayer/Application/Release/ClockworkPlayer.exe";
      var winPlayerD3DBinary = buildDir + "Source/ClockworkPlayer/Application/Release/D3DCompiler_47.dll";

      fs.copySync(winEditorBinary, buildDir + "Bin/ClockworkEditor.exe");
      fs.copySync(winEditorD3DBinary, buildDir + "Bin/D3DCompiler_47.dll");
      fs.copySync(winPlayerBinary, buildDir + "Bin/ClockworkPlayer.exe");

      console.log("Built Windows ClockworkEditor & Player");

      complete();

    }, {
      printStdout: true
    });

  });

}); // end of build namespace
