

// ln -sf /Users/josh/Dev/clockwork/ClockworkGameEngine/Artifacts/MacOSX_Package/ClockworkEditor.app ./ClockworkEditor.app

clockwork_data = require('clockwork-cli-mac-data');

module.exports = {
  "CLOCKWORKTOOL_BIN" : clockwork_data.CLOCKWORKTOOL_BIN,
  "CLOCKWORKEDITOR_APP" : clockwork_data.CLOCKWORKEDITOR_APP,
  "CLOCKWORKEDITOR_DATADIR" : clockwork_data.CLOCKWORKEDITOR_DATADIR

}
