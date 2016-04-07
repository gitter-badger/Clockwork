System.register(["./TypescriptLanguageService", "./workerProcessTypes", "../../../ClientExtensionEventNames"], function(exports_1, context_1) {
    "use strict";
    var __moduleName = context_1 && context_1.id;
    var TypescriptLanguageService_1, WorkerProcessTypes, ClientExtensionEventNames_1;
    var WebFileSystem, TypescriptLanguageServiceWebWorker;
    function getResource(codeUrl) {
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
    function getFileResource(filename) {
        return getResource(`clockwork://${filename}`);
    }
    return {
        setters:[
            function (TypescriptLanguageService_1_1) {
                TypescriptLanguageService_1 = TypescriptLanguageService_1_1;
            },
            function (WorkerProcessTypes_1) {
                WorkerProcessTypes = WorkerProcessTypes_1;
            },
            function (ClientExtensionEventNames_1_1) {
                ClientExtensionEventNames_1 = ClientExtensionEventNames_1_1;
            }],
        execute: function() {
            class WebFileSystem {
                constructor() {
                    this.fileCache = {};
                }
                fileExists(filename) {
                    return this.fileCache[filename] != null;
                }
                cacheFile(filename, file) {
                    this.fileCache[filename] = file;
                }
                getFile(filename) {
                    console.log("FS.GETFILE!!");
                    return this.fileCache[filename];
                }
                writeFile(filename, contents) {
                }
                getCurrentDirectory() {
                    return "";
                }
            }
            class TypescriptLanguageServiceWebWorker {
                constructor() {
                    this.connections = 0;
                    this.languageService = null;
                    this.projectLoaded = false;
                    this.fs = new WebFileSystem();
                    this.languageService = new TypescriptLanguageService_1.TypescriptLanguageService(this.fs);
                }
                connect(e) {
                    let port = e.ports[0];
                    this.connections++;
                    port.addEventListener("message", (e) => {
                        switch (e.data.command) {
                            case WorkerProcessTypes.Connect:
                                this.handleHELO(port, e.data);
                                break;
                            case WorkerProcessTypes.Disconnect:
                                this.handleCLOSE(port, e.data);
                                break;
                            case WorkerProcessTypes.GetCompletions:
                                this.handleGetCompletions(port, e.data);
                                break;
                            case WorkerProcessTypes.GetDocTooltip:
                                this.handleGetDocTooltip(port, e.data);
                                break;
                            case ClientExtensionEventNames_1.default.ResourceSavedEvent:
                                this.handleSave(port, e.data);
                                break;
                            case ClientExtensionEventNames_1.default.ResourceRenamedEvent:
                                this.handleRename(port, e.data);
                                break;
                            case ClientExtensionEventNames_1.default.ResourceDeletedEvent:
                                this.handleDelete(port, e.data);
                                break;
                            case ClientExtensionEventNames_1.default.ProjectUnloadedEvent:
                                this.handleProjectUnloaded(port);
                                break;
                        }
                    }, false);
                    port.start();
                }
                reset() {
                    this.languageService.reset();
                    this.projectLoaded = false;
                }
                loadProjectFiles() {
                    return getFileResource("resources/tsconfig.clockwork").then((jsonTsConfig) => {
                        let promises = [];
                        let tsConfig = JSON.parse(jsonTsConfig);
                        if (tsConfig.compilerOptions) {
                            this.languageService.compilerOptions = tsConfig.compilerOptions;
                        }
                        ;
                        let existingFiles = this.languageService.getProjectFiles();
                        existingFiles.forEach((f) => {
                            if (tsConfig.files.indexOf(f) == -1) {
                                this.languageService.deleteProjectFile(f);
                            }
                        });
                        tsConfig.files.forEach((f) => {
                            if (existingFiles.indexOf(f) == -1) {
                                promises.push(getFileResource(f).then((code) => {
                                    this.languageService.addProjectFile(f, code);
                                }));
                            }
                        });
                        return Promise.all(promises);
                    }).then(() => {
                        this.projectLoaded = true;
                    });
                }
                handleHELO(port, eventData) {
                    port.postMessage({ command: WorkerProcessTypes.Message, message: "Hello " + eventData.sender + " (port #" + this.connections + ")" });
                    this.loadProjectFiles().then(() => {
                        this.languageService.compile([eventData.filename]);
                    });
                }
                handleCLOSE(port, eventData) {
                    this.connections--;
                    if (this.connections <= 0) {
                        this.reset();
                    }
                    console.log("Got a close");
                }
                handleGetCompletions(port, eventData) {
                    let sourceFile = this.languageService.updateProjectFile(eventData.filename, eventData.sourceText);
                    let newpos = this.languageService.getPositionOfLineAndCharacter(sourceFile, eventData.pos.row, eventData.pos.column);
                    let completions = this.languageService.getCompletions(eventData.filename, newpos);
                    let message = {
                        command: WorkerProcessTypes.CompletionResponse,
                        completions: []
                    };
                    let langService = this.languageService;
                    if (completions) {
                        message.completions = completions.entries.map((completion) => {
                            let value = completion.name;
                            let completionItem = {
                                caption: completion.name,
                                value: value,
                                score: 100 - parseInt(completion.sortText, 0),
                                meta: completion.kind,
                                pos: newpos
                            };
                            return completionItem;
                        });
                    }
                    port.postMessage(message);
                }
                handleGetDocTooltip(port, eventData) {
                    let message = {
                        command: WorkerProcessTypes.DocTooltipResponse
                    };
                    const details = this.languageService.getCompletionEntryDetails(eventData.filename, eventData.pos, eventData.completionItem.caption);
                    if (details) {
                        let docs = details.displayParts.map(part => part.text).join("");
                        if (details.documentation) {
                            docs += "<br/" + details.documentation.map(part => part.text).join("");
                        }
                        message.docHTML = docs;
                    }
                    port.postMessage(message);
                }
                handleSave(port, eventData) {
                    getFileResource(eventData.path).then((code) => {
                        this.languageService.updateProjectFile(eventData.path, code);
                    });
                }
                handleDelete(port, eventData) {
                    this.languageService.deleteProjectFile(eventData.path);
                }
                handleRename(port, eventData) {
                    this.languageService.renameProjectFile(eventData.path, eventData.newPath);
                }
                handleProjectUnloaded(port) {
                    this.reset();
                }
            }
            exports_1("default", TypescriptLanguageServiceWebWorker);
        }
    }
});
//# sourceMappingURL=TypescriptLanguageServiceWebWorker.js.map