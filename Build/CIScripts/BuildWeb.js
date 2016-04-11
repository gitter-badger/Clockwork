var fs = require('fs-extra');
var bcommon = require("./BuildCommon");
var host = require("./Host");

var buildDir = bcommon.artifactsRoot + "Build/Web/";

namespace('build', function() {

  task('web_player', {
    async: true
  }, function() {

    var clockworkTool = host.getClockworkToolBinary();

    var playerBuildDir = buildDir + "ClockworkPlayer/";

    bcommon.cleanCreateDir(playerBuildDir);
    bcommon.cleanCreateDir(bcommon.getGenScriptRootDir("WEB"));

    process.chdir(playerBuildDir);

    var cmds = [

      clockworkTool + " bind " + bcommon.clockworkRoot + " Script/Packages/Clockwork/ WEB",
      clockworkTool + " bind " + bcommon.clockworkRoot + " Script/Packages/ClockworkPlayer/ WEB",
      "cmake -DEMSCRIPTEN=1 -DCMAKE_TOOLCHAIN_FILE=" + bcommon.clockworkRoot + "Build/CMake/Toolchains/emscripten.toolchain.cmake -DCMAKE_BUILD_TYPE=Release ../../../../",
      "make -j4",
      "cd ./Source/ClockworkPlayer/Application && mv ./ClockworkPlayer ./ClockworkPlayer.bc && " +
      // -s ERROR_ON_UNDEFINED_SYMBOLS=1 (disabled for pthread errors currently on incoming)
      "emcc -O3 -s USE_PTHREADS=0 -s ASM_JS=1 -s VERBOSE=0 -s USE_SDL=2 -s TOTAL_MEMORY=134217728 -s AGGRESSIVE_VARIABLE_ELIMINATION=1 -s NO_EXIT_RUNTIME=1 ./ClockworkPlayer.bc -o  ./ClockworkPlayer.html"

    ]

    jake.exec(cmds, function() {

      var webPlayerBinary = playerBuildDir + "Source/ClockworkPlayer/Application/ClockworkPlayer.js";
      var webPlayerMemFile = playerBuildDir + "Source/ClockworkPlayer/Application/ClockworkPlayer.html.mem";

      fs.copySync(webPlayerBinary, buildDir + "Bin/ClockworkPlayer.js");
      fs.copySync(webPlayerMemFile, buildDir + "Bin/ClockworkPlayer.html.mem");

      console.log("Built Web Player");

      complete();
    }, {
      printStdout: true
    });

  });

}); // end of build namespace
