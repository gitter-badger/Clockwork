var fs = require('fs-extra');
var bcommon = require("./BuildCommon");
var host = require("./Host");

var buildDir = bcommon.artifactsRoot + "Build/EditorData/";
var jsDocFolder = bcommon.artifactsRoot + "Build/JSDoc/";
var clockworkRoot = bcommon.clockworkRoot;
var clockworkTool = host.getClockworkToolBinary();
var glob = require("glob");
var Tslint = require("tslint");

namespace('build', function() {

  task('genscriptbindings', {
    async: true
  }, function() {

    bcommon.cleanCreateDir(buildDir);
    bcommon.cleanCreateDir(jsDocFolder);

    var bindCmd = clockworkTool + " bind \"" + clockworkRoot + "\" ";

    var cmds = [
      bindCmd + "Script/Packages/Clockwork/ WINDOWS",
      bindCmd + "Script/Packages/ClockworkPlayer/ WINDOWS",
      bindCmd + "Script/Packages/ToolCore/ WINDOWS",
      bindCmd + "Script/Packages/Editor/ WINDOWS",
      bindCmd + "Script/Packages/ClockworkNET/ WINDOWS",
      bindCmd + "Script/Packages/WebView/ WINDOWS",
    ];

    jake.exec(cmds, function() {

      console.log("Built Script Bindings");

      complete();

    }, {
      printStdout: true
    });

  });

  task('gendocs', ["build:genscriptbindings"], {
    async: true
  }, function() {

    console.log("Generating Examples & JSDocs");

    fs.copySync(clockworkRoot + "Build/Docs/Readme.md", jsDocFolder + "/Readme.md");
    fs.copySync(clockworkRoot + "Build/Docs/jsdoc.conf", jsDocFolder + "/jsdoc.conf");

    cmds = [
      "git clone https://github.com/ClockworkGameEngine/ClockworkExamples " + buildDir + "ClockworkExamples && rm -rf " + buildDir + "ClockworkExamples/.git",
      "cd " + jsDocFolder + " && npm install git+https://github.com/jsdoc3/jsdoc",
      "cd " + jsDocFolder + " && git clone https://github.com/ClockworkGameEngine/jaguarjs-jsdoc && cd jaguarjs-jsdoc && git checkout clockwork_master",
      "cd " + jsDocFolder + " && ./node_modules/.bin/jsdoc ./Clockwork.js -t ./jaguarjs-jsdoc/ -c ./jsdoc.conf Readme.md",
    ];

    jake.exec(cmds, function() {

      fs.copySync(jsDocFolder + "out", buildDir + "Docs");
      complete();

    }, {
      printStdout: true
    });


  });

  task('ios_deploy', {
    async: true
  }, function() {

    var iosDeploybuildDir = bcommon.artifactsRoot + "Build/IOSDeploy/";

    bcommon.cleanCreateDir(iosDeploybuildDir);

    process.chdir(iosDeploybuildDir);

    jake.exec("git clone https://github.com/ClockworkGameEngine/ios-deploy && cd ios-deploy && make ios-deploy",
      function() {

        complete();
      }, {
        printStdout: true
      });

  });

  // Linting task
  task('lint_typescript', {
      async: true
  }, function(fileMask, failOnError) {

    console.log("TSLINT: Linting files in " + fileMask);
    var lintConfig = JSON.parse(fs.readFileSync("./Script/tslint.json"));
    var options = {
        configuration: lintConfig,
        formatter: "prose"
    };

    // lint
    // Since TSLint does not yet support recursively searching for files, then we need to
    // create a command per file.  The main issue with this is that it will abort on the first error instead
    // of listing out all lint errors
    glob(fileMask, function(err, results) {
      var lintErrors = [];
      results.forEach(function(filename) {

        var contents = fs.readFileSync(filename, "utf8");

        var ll = new Tslint(filename, contents, options);
        var result = ll.lint();
        if (result.failureCount > 0) {
            lintErrors.push(result.output);
        }
      });
      if (lintErrors.length > 0) {
          console.warn("TSLINT: WARNING - Lint errors detected");
          console.warn(lintErrors.join(''));
          if (failOnError) {
              fail("TSLint errors detected");
          }
      }
      complete();
    });
  });

  task('compileeditorscripts', ["build:genscriptbindings"],{
    async: true
  }, function() {

    console.log("Compiling Editor Scripts");

    process.chdir(clockworkRoot);

    var tsc = "./Build/node_modules/typescript/lib/tsc";

    cmds = [
      clockworkRoot + "Build/Mac/node/node " + tsc + " -p ./Script",
      clockworkRoot + "Build/Mac/node/node " + tsc + " -p ./Script/ClockworkWebViewEditor"
    ];

      // will be copied when editor resources are copied

    var lintTask = jake.Task['build:lint_typescript'];

    lintTask.addListener('complete', function () {
      console.log("\n\nLint: Typescript linting complete.\n\n");
      jake.exec(cmds, function() {

          // copy some external dependencies into the editor modules directory
         var editorModulesDir = "./Artifacts/Build/Resources/EditorData/ClockworkEditor/EditorScripts/ClockworkEditor/modules";
         var webeditorModulesDir = "./Data/ClockworkEditor/CodeEditor/source/editorCore/modules";
         var nodeModulesDir = "./Build/node_modules";
         fs.mkdirsSync(editorModulesDir);
         // TypeScript
         fs.copySync(nodeModulesDir + "/typescript/lib/typescript.js", webeditorModulesDir + "/typescript.js")

         // copy lib.core.d.ts into the tool data directory
         fs.mkdirsSync("./Artifacts/Build/Resources/EditorData/ClockworkEditor/EditorScripts/ClockworkEditor/TypeScriptSupport");
         fs.copySync("./Build/node_modules/typescript/lib/lib.core.d.ts","./Data/ClockworkEditor/TypeScriptSupport/lib.core.d.ts")
         complete();

      }, {
        printStdout: true
      });
    });

    lintTask.invoke("{./Script/ClockworkEditor/**/*.ts,./Script/ClockworkWebViewEditor/**/*.ts}", false);
  });

  task('geneditordata', ["build:compileeditorscripts", "build:ios_deploy", "build:gendocs"], {
    async: true
  }, function() {

    // Mac App

    fs.copySync(clockworkRoot + "Build/CIScripts/Mac/PlayerApp/",
      buildDir + "MacApps/PlayerApp/");

    // Editor Binaries

    fs.copySync(bcommon.artifactsRoot + "Build/Mac/Bin/ClockworkEditor.zip",
      buildDir + "EditorBinaries/Mac/ClockworkEditor.zip");

    fs.copySync(bcommon.artifactsRoot + "Build/Windows/Bin/ClockworkEditor.exe",
      buildDir + "EditorBinaries/Windows/ClockworkEditor.exe");

    fs.copySync(bcommon.artifactsRoot + "Build/Windows/Bin/D3DCompiler_47.dll",
      buildDir + "EditorBinaries/Windows/D3DCompiler_47.dll");

    // Resources

    fs.copySync(clockworkRoot + "Resources/CoreData", buildDir + "Resources/CoreData");
    fs.copySync(clockworkRoot + "Resources/EditorData", buildDir + "Resources/EditorData");
    fs.copySync(clockworkRoot + "Resources/PlayerData", buildDir + "Resources/PlayerData");
    fs.copySync(clockworkRoot + "/Data/ClockworkEditor", buildDir + "Resources/ToolData");

    fs.copySync(clockworkRoot + "Artifacts/Build/Resources/EditorData/ClockworkEditor/EditorScripts",
      buildDir + "Resources/EditorData/ClockworkEditor/EditorScripts");

    // root deployment
    var deployRoot = buildDir + "Resources/ToolData/Deployment/";

    fs.copySync(clockworkRoot + "/Data/ClockworkEditor/Deployment/", deployRoot);

    // Android
    fs.copySync(bcommon.artifactsRoot + "Build/Android/Bin/libClockworkPlayer.so",
      deployRoot + "Android/libs/armeabi-v7a/libClockworkPlayer.so");

    // Mac
    fs.copySync(bcommon.artifactsRoot + "Build/Mac/Bin/ClockworkPlayer",
      deployRoot + "MacOS/ClockworkPlayer.app/Contents/MacOS/ClockworkPlayer");

    // IOS
    fs.copySync(bcommon.artifactsRoot + "Build/IOSDeploy/ios-deploy/ios-deploy",
      deployRoot + "IOS/ios-deploy/ios-deploy");
    fs.copySync(bcommon.artifactsRoot + "Build/IOS/Bin/ClockworkPlayer",
      deployRoot + "IOS/ClockworkPlayer.app/ClockworkPlayer");

    // Web
    fs.copySync(bcommon.artifactsRoot + "Build/Web/Bin/ClockworkPlayer.js",
      deployRoot + "Web/ClockworkPlayer.js");
    fs.copySync(bcommon.artifactsRoot + "Build/Web/Bin/ClockworkPlayer.html.mem",
      deployRoot + "Web/ClockworkPlayer.html.mem");

    // Windows
    fs.copySync(bcommon.artifactsRoot + "Build/Windows/Bin/ClockworkPlayer.exe",
      deployRoot + "Windows/x64/ClockworkPlayer.exe");
    fs.copySync(bcommon.artifactsRoot + "Build/Windows/Bin/D3DCompiler_47.dll",
      deployRoot + "Windows/x64/D3DCompiler_47.dll");


    complete();

    process.chdir(buildDir);

    var cmds = ["zip -r -X ./EditorData.zip ./"];

    jake.exec(cmds,
      function() {
        complete();
      }, {
        printStdout: true
      });

  });

}); // end of build namespace
