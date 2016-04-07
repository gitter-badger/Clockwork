System.register(["./editor/editorCommands"], function(exports_1, context_1) {
    "use strict";
    var __moduleName = context_1 && context_1.id;
    var editorCommands;
    var DEBUG_PORT, DEBUG_ALERT, HostInteropType;
    function clockworkQueryPromise(message) {
        return new Promise(function (resolve, reject) {
            let queryMessage = message;
            if (typeof (message) != "string") {
                queryMessage = JSON.stringify(message);
            }
            window.clockworkQuery({
                request: queryMessage,
                persistent: false,
                onSuccess: resolve,
                onFailure: (error_code, error_message) => reject({ error_code: error_code, error_message: error_message })
            });
        });
    }
    return {
        setters:[
            function (editorCommands_1) {
                editorCommands = editorCommands_1;
            }],
        execute: function() {
            DEBUG_PORT = 3335;
            DEBUG_ALERT = false;
            class HostInteropType {
                constructor() {
                    window.HOST_loadCode = this.loadCode.bind(this);
                    window.HOST_saveCode = this.saveCode.bind(this);
                    window.HOST_projectUnloaded = this.projectUnloaded.bind(this);
                    window.HOST_resourceRenamed = this.resourceRenamed.bind(this);
                    window.HOST_resourceDeleted = this.resourceDeleted.bind(this);
                }
                static getInstance() {
                    if (HostInteropType._inst == null) {
                        HostInteropType._inst = new HostInteropType();
                    }
                    return HostInteropType._inst;
                }
                loadCode(codeUrl) {
                    console.log("Load Code called for :" + codeUrl);
                    const fileExt = codeUrl.split(".").pop();
                    const filename = codeUrl.replace("clockwork://", "");
                    editorCommands.configure(fileExt, filename);
                    this.getResource(codeUrl).then((src) => {
                        editorCommands.loadCodeIntoEditor(src, filename, fileExt);
                    }).catch((e) => {
                        console.log("Error loading code: " + e.error_message);
                    });
                }
                saveCode() {
                    return clockworkQueryPromise({
                        message: HostInteropType.EDITOR_SAVE_CODE,
                        payload: editorCommands.getSourceText()
                    });
                }
                saveFile(filename, fileContents) {
                    return clockworkQueryPromise({
                        message: HostInteropType.EDITOR_SAVE_FILE,
                        filename: filename,
                        payload: fileContents
                    });
                }
                editorLoaded() {
                    if (DEBUG_ALERT) {
                        alert(`Attach chrome dev tools to this instance by navigating to http://localhost:${DEBUG_PORT}`);
                    }
                    clockworkQueryPromise(HostInteropType.EDITOR_LOAD_COMPLETE);
                }
                getResource(codeUrl) {
                    return new Promise(function (resolve, reject) {
                        const xmlHttp = new XMLHttpRequest();
                        xmlHttp.onreadystatechange = () => {
                            if (xmlHttp.readyState == 4 && xmlHttp.status == 200) {
                                resolve(xmlHttp.responseText);
                            }
                        };
                        xmlHttp.open("GET", codeUrl, true);
                        xmlHttp.send(null);
                    });
                }
                getFileResource(filename) {
                    return this.getResource(`clockwork://${filename}`);
                }
                notifyEditorChange() {
                    clockworkQueryPromise(HostInteropType.EDITOR_CHANGE).catch((e) => {
                        console.log("Error on change: " + e.error_message);
                    });
                }
                projectUnloaded() {
                    editorCommands.projectUnloaded();
                }
                resourceRenamed(path, newPath) {
                    editorCommands.resourceRenamed(path, newPath);
                }
                resourceDeleted(path) {
                    editorCommands.resourceDeleted(path);
                }
            }
            HostInteropType._inst = null;
            HostInteropType.EDITOR_SAVE_CODE = "editorSaveCode";
            HostInteropType.EDITOR_SAVE_FILE = "editorSaveFile";
            HostInteropType.EDITOR_LOAD_COMPLETE = "editorLoadComplete";
            HostInteropType.EDITOR_CHANGE = "editorChange";
            exports_1("default", HostInteropType);
            HostInteropType.getInstance().editorLoaded();
        }
    }
});
//# sourceMappingURL=interop.js.map