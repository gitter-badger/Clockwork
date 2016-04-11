var os = require('os');
var path = require('path');

// get the root folder
var clockworkRoot = path.resolve(__dirname, "../..") + "/";

// patch in our local node_modules
process.env.NODE_PATH = clockworkRoot + "Build/node_modules/";
require('module').Module._initPaths();
var fs = require('fs-extra');

/// Returns a list of all script packages, regardless of platform
function getScriptPackages() {

  var srcpath = clockworkRoot + "Script/Packages/";

  return fs.readdirSync(srcpath).filter(function(file) {
    return fs.statSync(path.join(srcpath, file)).isDirectory() && fs.existsSync(srcpath + "/" + file + "/Package.json");
  });

}

// return an object with package name keys and module name lists as values
function getScriptModules(platform) {

  modules = {};

  var packages = getScriptPackages();

  for (var i in packages) {

    var pkg = JSON.parse(fs.readFileSync(clockworkRoot + "Script/Packages/" + packages[i] + "/Package.json"));

    if (pkg.platforms && pkg.platforms.indexOf(platform) == -1)
      continue;

    for (var j in pkg.modules) {

      var moduleName = pkg.modules[j];

      if (pkg.moduleExclude && pkg.moduleExclude[platform])
        if (pkg.moduleExclude[platform].indexOf(moduleName) != -1)
          continue;

      if (!modules[pkg.name])
        modules[pkg.name] = [];

      modules[pkg.name].push(moduleName);
    }

  }

  return modules;

}

function getGenScriptRootDir(platform) {

  return clockworkRoot + "Artifacts/Build/Source/Generated/" + platform + "/";

}

// Get a list of script source filenames for a given platform
function getGenScriptFilenames(platform) {

  var filenames = [];

  var scriptGenRoot = getGenScriptRootDir(platform);

  var modules = getScriptModules(platform);

  for (var pkgName in modules) {

    var jsPackageFolder = scriptGenRoot + "Javascript/Packages/" + pkgName + "/";

    // the JS package sources
    filenames.push(jsPackageFolder + "JSPackage" + pkgName + ".cpp");

    for (var i in modules[pkgName]) {
      filenames.push(jsPackageFolder + "JSModule" + modules[pkgName][i] + ".cpp");
    }

  }

  return filenames;

}

function createGenScriptFiles(platform) {

  var scriptFiles = getGenScriptFilenames(platform);

  for (var i in scriptFiles) {

    fs.ensureFileSync(scriptFiles[i]);

  }

}

function cleanCreateDir(directory) {

  testRemoveDir(directory);
  testCreateDir(directory);

}

function testCreateDir(directory) {

  if (fs.existsSync(directory)) {
    fail("Path already exists: " + directory);
  }

  jake.mkdirP(directory);

  if (!fs.existsSync(directory)) {
    fail("Unable to create path: " + directory);
  }
}


function testRemoveDir(path) {

  if (fs.existsSync(path)) {
    jake.rmRf(path);
  }

  if (fs.existsSync(path)) {
    fail("Unable to remove path: " + path);
  }
}

exports.clockworkRoot = clockworkRoot;
exports.artifactsRoot = clockworkRoot + "Artifacts/";
exports.cleanCreateDir = cleanCreateDir;
exports.testCreateDir = testCreateDir;
exports.testRemoveDir = testRemoveDir;
exports.getScriptPackages = getScriptPackages;
exports.getScriptModules = getScriptModules;
exports.getGenScriptFilenames = getGenScriptFilenames;
exports.createGenScriptFiles = createGenScriptFiles;
exports.getGenScriptRootDir = getGenScriptRootDir;
