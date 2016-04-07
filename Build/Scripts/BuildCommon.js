var fs = require('fs-extra');
var os = require('os');
var path = require("path");
var host = require("./Host");
var clockworkRoot = host.clockworkRoot;
var glob = require('glob');

var Tslint = require("tslint");

namespace('build', function() {

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

  task('genscripts', {
    async: true
  }, function(platform) {

        process.chdir(clockworkRoot);

        var modules = host.getScriptModules(platform);
        var bindCmd = host.clockworkTool + " bind \"" + clockworkRoot + "\" ";
        var node;
        var tsc = "./Build/node_modules/typescript/lib/tsc";
        var tslint = "./Build/node_modules/tslint/lib/tslint-cli";

        switch(os.platform()) {
            case "win32":
                node = "Build\\Windows\\node\\node.exe";
                break;
            case "darwin":
                node = "Build/Mac/node/node";
                break;
            case "linux":
                node = "Build/Linux/node/node";
                break;
        }

        var cmds = [];
        for (var pkgName in modules) {
            cmds.push(bindCmd + "Script/Packages/" + pkgName + "/ " + platform);
        }

        if (node) {
          // compile
          cmds.push(node + " " + tsc + " -p ./Script");
          cmds.push(node + " " + tsc + " -p ./Script/ClockworkWebViewEditor");

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

        } else {
            throw new Error("Node not configured for this platform: " + os.platform());
        }


  });


}); // end of build namespace
