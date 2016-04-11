
var Q = require("q");
var fs = require("fs");
var os = require("os");
var path = require("path");
var spawn = require("child_process").spawn;

try {
  var platform_cli = require('clockwork-cli-mac');
}
catch (e) {
}
try {
  var platform_cli = require('clockwork-cli-windows');
}
catch (e) {
}

var mode = process.env.CLOCKWORK_GAME_ENGINE_ENV || 'production';
if (mode == 'dev') {
  if (process.platform == 'darwin')
    platform_cli.CLOCKWORKTOOL_BIN = "/Users/josh/Dev/clockwork/ClockworkGameEngine-build/Source/ClockworkTool/ClockworkTool";
  else
    platform_cli.CLOCKWORKTOOL_BIN = "C:/Dev/clockwork/ClockworkGameEngine-build/Source/ClockworkTool/ClockworkTool.exe";
}
// Commands ----------------------

var newProject = function(folder) {
  return clockworktool(["new", folder], {output:true});
}

var addPlatform = function (platform) {
  return clockworktool(["platform-add", platform], {output:true});
};

var build = function (platform) {

  console.log("building");
  return clockworktool(["build", platform], {output:true});
};

var activate = function (productkey) {
  return clockworktool(["--activate", productkey.toUpperCase().trim()], {output:true});
};

var deactivate = function () {
  return clockworktool(["--deactivate"], {output:true});
};

var run = function (platform, opts) {

    opts = opts || {};
    var debug = opts.debug;

    var run = function () {

        switch (platform) {

        case "web":
            var url = "http://localhost:" + HTTP_PORT + "/ClockworkPlayer.html";
            console.log("Launching: " + url);

            var open = require("open");
            open(url);
            break;

          case "mac":
              var open = require("open");
              open(path.resolve("Build/Mac-Build/ClockworkPlayer.app"));
              break;

          case "windows":
              var open = require("open");
              open(path.resolve("Build/Windows-Build/ClockworkPlayer.exe"));
              break;
        }
    };

    return opts.noBuild ? run(platform) : build([platform], opts).then(function () {
        return run();
    });
};


// Utils --------------

var exec = function (command, flags, opts) {
    opts = opts || {};
    if (opts.verbose !== false) {
        console.log([command].concat(flags).join(" "));
    }

    var deferred = Q.defer();
    var child = spawn(command, flags, {stdio: (opts.output === false) ? "ignore" : "inherit"});
    child.on("close", function (code) {
        if (code && opts.check !== false) {
            deferred.reject();
        }
        deferred.resolve(code);
    });
    child.on("error", function (error) {
        deferred.reject(error);
    });
    return deferred.promise;
};


var clockworktool = function (flags, opts) {
    opts = opts || {};
    flags.unshift(platform_cli.CLOCKWORKEDITOR_DATADIR);
    flags.unshift("--cli-data-path");
    return exec(platform_cli.CLOCKWORKTOOL_BIN, flags, opts);
};

module.exports = {
  "clockworktool" : clockworktool,
  "newProject" : newProject,
  "addPlatform" : addPlatform,
  "build" : build,
  "run" : run,
  "activate" : activate,
  "deactivate" : deactivate
}
