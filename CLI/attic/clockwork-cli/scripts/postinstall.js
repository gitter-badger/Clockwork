"use strict";

var fs = require("fs");
var path = require('path');
var osenv = require("osenv");
var semver = require("semver")
var mkdirp = require("mkdirp")
var clockworkeditor = require("./clockworkeditor");
var exists = fs.exists || path.exists;

var mkdirSync = function (path) {

  if (exists(path))
    return;

  mkdirp.sync(path);

}

var updateEditor = function() {

}

var editorUpdateRequired = function(path) {

  if (!exists(path))
    return true;

  // JSON.parse(fs.readFileSync(path))
  // console.log ( semver.major(semver.valid("2.0.0")));
}

// CDN
var cdnroot = JSON.parse(fs.readFileSync(__dirname + "/../data/cdn.json"))
var branch;

// TODO: otherwise we need to use the package data
if (process.env.CLOCKWORK_DEV_LOCAL) {
  branch = "local";
}

var cdn = cdnroot[branch]
var cdnServer = cdn["server"];
var cdnEditorVersion = cdn["editor"]["version"];
var cdnClockworkToolVersion = cdn["clockworktool"]["version"];

var home = osenv.home();
var clockworkRootDir = home + "/.clockworkgameengine/" + branch;

// CDN url for editor
var cdnEditorURL;

if (process.platform == "darwin") {
  cdnEditorURL = cdnServer + cdn["editor"] + "-" + cdnEditorVersion + ".zip";
}

// ensure clockwork's home exists
mkdirSync(clockworkRootDir);

var editorUpdate = false;

if (editorUpdateRequired(clockworkRootDir)) {
  editorUpdate = true;
  console.log("Editor Update Required");
}
