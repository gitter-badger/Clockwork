System.register(["./workerprocess/workerProcessTypes", "../../ClientExtensionEventNames"], function(exports_1, context_1) {
    "use strict";
    var __moduleName = context_1 && context_1.id;
    var WorkerProcessTypes, ClientExtensionEventNames_1;
    var TypescriptLanguageExtension;
    return {
        setters:[
            function (WorkerProcessTypes_1) {
                WorkerProcessTypes = WorkerProcessTypes_1;
            },
            function (ClientExtensionEventNames_1_1) {
                ClientExtensionEventNames_1 = ClientExtensionEventNames_1_1;
            }],
        execute: function() {
            class TypescriptLanguageExtension {
                constructor() {
                    this.name = "ClientTypescriptLanguageExtension";
                    this.description = "This extension handles typescript language features such as completion, compilation, etc.";
                    this.fullCompile = true;
                }
                initialize(serviceLocator) {
                    this.serviceLocator = serviceLocator;
                }
                isValidFiletype(path) {
                    let ext = path.split(".").pop();
                    return ext == "ts";
                }
                workerRequest(responseChannel, message) {
                    let worker = this.worker;
                    return new Promise((resolve, reject) => {
                        const responseCallback = function (e) {
                            if (e.data.command == responseChannel) {
                                worker.port.removeEventListener("message", responseCallback);
                                resolve(e.data);
                            }
                        };
                        this.worker.port.addEventListener("message", responseCallback);
                        this.worker.port.postMessage(message);
                    });
                }
                configureEditor(ev) {
                    if (this.isValidFiletype(ev.filename)) {
                        let editor = ev.editor;
                        editor.session.setMode("ace/mode/typescript");
                        editor.setOptions({
                            enableBasicAutocompletion: true,
                            enableLiveAutocompletion: true
                        });
                        this.editor = editor;
                    }
                }
                codeLoaded(ev) {
                    if (this.isValidFiletype(ev.filename)) {
                        this.filename = ev.filename;
                        let editor = ev.editor;
                        editor.completers = [this.buildWordCompleter(ev.filename)];
                        this.buildWorker();
                        this.worker.port.postMessage({ command: WorkerProcessTypes.Connect, sender: "Typescript Language Extension", filename: ev.filename });
                    }
                }
                handleWorkerMessage(e) {
                    switch (e.data.command) {
                        case WorkerProcessTypes.Message:
                            console.log(e.data.message);
                            break;
                        case WorkerProcessTypes.Alert:
                            alert(e.data.message);
                            break;
                    }
                }
                buildWorker() {
                    this.worker = new SharedWorker("./source/editorCore/clientExtensions/languageExtensions/typescript/workerprocess/workerLoader.js");
                    this.worker.port.addEventListener("message", this.handleWorkerMessage.bind(this), false);
                    addEventListener("beforeunload", () => {
                        this.worker.port.postMessage({ command: WorkerProcessTypes.Disconnect });
                    });
                    this.worker.port.start();
                }
                buildWordCompleter(filename) {
                    let extension = this;
                    let wordCompleter = {
                        getDocTooltip: function (selected) {
                            const message = {
                                command: WorkerProcessTypes.GetDocTooltip,
                                filename: extension.filename,
                                completionItem: selected,
                                pos: selected.pos
                            };
                            extension.workerRequest(WorkerProcessTypes.DocTooltipResponse, message)
                                .then((e) => {
                                extension.editor.completer.showDocTooltip(e);
                            });
                        },
                        getCompletions: function (editor, session, pos, prefix, callback) {
                            const message = {
                                command: WorkerProcessTypes.GetCompletions,
                                filename: extension.filename,
                                pos: pos,
                                sourceText: editor.session.getValue(),
                                prefix: prefix
                            };
                            extension.workerRequest(WorkerProcessTypes.CompletionResponse, message)
                                .then((e) => {
                                callback(null, e.completions);
                            });
                        }
                    };
                    return wordCompleter;
                }
                save(ev) {
                    if (this.isValidFiletype(ev.path)) {
                        console.log(`${this.name}: received a save resource event for ${ev.path}`);
                        const message = {
                            command: ClientExtensionEventNames_1.default.ResourceSavedEvent,
                            path: ev.path
                        };
                        this.worker.port.postMessage(message);
                    }
                }
                delete(ev) {
                    if (this.isValidFiletype(ev.path)) {
                        console.log(`${this.name}: received a delete resource event for ${ev.path}`);
                        const message = {
                            command: ClientExtensionEventNames_1.default.ResourceDeletedEvent,
                            path: ev.path
                        };
                        this.worker.port.postMessage(message);
                    }
                }
                rename(ev) {
                    if (this.isValidFiletype(ev.path)) {
                        console.log(`${this.name}: received a rename resource event for ${ev.path} -> ${ev.newPath}`);
                        const message = {
                            command: ClientExtensionEventNames_1.default.ResourceRenamedEvent,
                            path: ev.path,
                            newPath: ev.newPath
                        };
                        this.worker.port.postMessage(message);
                    }
                }
                projectUnloaded() {
                    if (this.worker) {
                        console.log(`${this.name}: received a project unloaded event`);
                        const message = {
                            command: ClientExtensionEventNames_1.default.ProjectUnloadedEvent
                        };
                        this.worker.port.postMessage(message);
                    }
                }
            }
            exports_1("default", TypescriptLanguageExtension);
        }
    }
});
//# sourceMappingURL=TypescriptLanguageExtension.js.map