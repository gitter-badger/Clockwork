

var mode = process.env.CLOCKWORK_GAME_ENGINE_ENV || 'production';

var clockworktool = require ("./clockworktool.js");
var clockworkeditor = require ("./clockworkeditor.js");

module.exports = {

  "mode" : mode,
  "newProject" : clockworktool.newProject,
  "addPlatform" : clockworktool.addPlatform,
  "build" : clockworktool.build,
  "run" : clockworktool.run,
  "activate" : clockworktool.activate,
  "deactivate" : clockworktool.deactivate,
  "clockworkeditor" : clockworkeditor

}
