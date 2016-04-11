
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

var clockworkeditor = function (flags, opts) {
  opts = opts || {};
  opts.detached = true;
  opts.stdio = ["ignore", "ignore", "ignore"];
  var child = spawn(platform_cli.CLOCKWORKEDITOR_APP, flags, opts);
  child.unref();
};

module.exports = clockworkeditor;
