var fs = require('fs-extra');
var path = require("path");
var host = require("./Host");
var os = require('os');
var clockworkRoot = host.clockworkRoot;

var buildDir = host.artifactsRoot + "Build/Web/ClockworkPlayer";

// build command:
// ./Build/Mac/node/node ./Build/node_modules/jake/bin/cli.js -f ./Build/Scripts/Bootstrap.js build:web_player

namespace('build', function() {

  task('web_player'/*, ["build:clockworkeditor"]*/, {
    async: true
  }, function() {

    // TODO: add clean build option
    if (!fs.existsSync(buildDir)) {
      common.cleanCreateDir(buildDir);
    }

    process.chdir(buildDir);

    var cmds = [];

    var scriptModules = host.getScriptModules("WEB");

    var bindCmd = host.clockworkTool + " bind \"" + clockworkRoot + "\" ";

    // Generate bindings for each script package
    for (var pkgName in scriptModules) {
      cmds.push(bindCmd + "Script/Packages/" + pkgName + "/ WEB")
    }

    if (os.platform() == "win32") {

    }
    else {
      cmds.push("cmake -DEMSCRIPTEN=1 -DCMAKE_TOOLCHAIN_FILE=\"" + clockworkRoot + "Build/CMake/Toolchains/emscripten.toolchain.cmake\" -DCMAKE_BUILD_TYPE=Release ../../../../");
      cmds.push("make -j4");
      // -s ERROR_ON_UNDEFINED_SYMBOLS=1 (disabled for pthread errors currently on incoming)
      cmds.push("cd ./Source/ClockworkPlayer/Application && mv ./ClockworkPlayer ./ClockworkPlayer.bc && " +
      "emcc -O3 -s USE_PTHREADS=0 -s ASM_JS=1 -s VERBOSE=0 -s USE_SDL=2 -s TOTAL_MEMORY=134217728 -s AGGRESSIVE_VARIABLE_ELIMINATION=1 -s NO_EXIT_RUNTIME=1 ./ClockworkPlayer.bc -o ./ClockworkPlayer.html");
    }

    jake.exec(cmds, function() {

      var editorAppFolder = host.artifactsRoot + (os.platform() == "win32" ? "ClockworkEditor/" : "ClockworkEditor/ClockworkEditor.app/");

      var webPlayerBinary = buildDir + "/Source/ClockworkPlayer/Application/ClockworkPlayer.js";
      var webPlayerMemFile = buildDir + "/Source/ClockworkPlayer/Application/ClockworkPlayer.html.mem";

      // Install Deployment

      // Don't put into Artifact editor build as this requires a Emscripten install
      // fs.copySync(webPlayerBinary, editorAppFolder + "Resources/ToolData/Deployment/Web/ClockworkPlayer.js");
      // fs.copySync(webPlayerMemFile, editorAppFolder + "Resources/ToolData/Deployment/Web/ClockworkPlayer.html.mem");

      // Instead, use local build
      fs.copySync(webPlayerBinary, clockworkRoot + "Data/ClockworkEditor/Deployment/Web/ClockworkPlayer.js");
      fs.copySync(webPlayerMemFile, clockworkRoot + "Data/ClockworkEditor/Deployment/Web/ClockworkPlayer.html.mem");

      console.log("Built Web Player");

      complete();

    }, {
      printStdout: true,
      breakOnError : false
    });

  });

}); // end of build namespace
