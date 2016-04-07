System.register(["../../../../modules/typescript"], function(exports_1, context_1) {
    "use strict";
    var __moduleName = context_1 && context_1.id;
    var ts;
    var TypescriptLanguageService;
    return {
        setters:[
            function (ts_1) {
                ts = ts_1;
            }],
        execute: function() {
            class TypescriptLanguageService {
                constructor(fs) {
                    this.fs = null;
                    this.languageService = null;
                    this.documentRegistry = null;
                    this.compilerOptions = {
                        noEmitOnError: true,
                        noImplicitAny: false,
                        target: 1,
                        module: 1,
                        noLib: true
                    };
                    this.name = "TypescriptLanguageService";
                    this.fullCompile = true;
                    this.projectFiles = [];
                    this.versionMap = {};
                    this.fs = fs;
                    this.documentRegistry = ts.createDocumentRegistry();
                    this.createLanguageService(this.documentRegistry);
                }
                createLanguageService(documentRegistry) {
                    const servicesHost = {
                        getScriptFileNames: () => this.projectFiles,
                        getScriptVersion: (fileName) => this.versionMap[fileName] && this.versionMap[fileName].version.toString(),
                        getScriptSnapshot: (filename) => {
                            const scriptVersion = this.versionMap[filename];
                            if (scriptVersion) {
                                if (scriptVersion.snapshot) {
                                    return scriptVersion.snapshot;
                                }
                                else {
                                    console.log(`!!! creating snapshot for ${filename}`);
                                    let sourceFile = this.documentRegistry.acquireDocument(filename, this.compilerOptions, ts.ScriptSnapshot.fromString(""), scriptVersion.version.toString());
                                    return ts.ScriptSnapshot.fromString(sourceFile.text);
                                }
                            }
                        },
                        getCurrentDirectory: () => this.fs.getCurrentDirectory(),
                        getCompilationSettings: () => this.compilerOptions,
                        getDefaultLibFileName: (options) => undefined
                    };
                    this.languageService = ts.createLanguageService(servicesHost, documentRegistry);
                }
                addProjectFile(filename, fileContents) {
                    if (this.projectFiles.indexOf(filename) == -1) {
                        console.log("Added project file: " + filename);
                        this.versionMap[filename] = {
                            version: 0,
                            snapshot: ts.ScriptSnapshot.fromString(fileContents || this.fs.getFile(filename))
                        };
                        this.projectFiles.push(filename);
                        this.documentRegistry.acquireDocument(filename, this.compilerOptions, this.versionMap[filename].snapshot, "0");
                    }
                }
                updateProjectFile(filename, fileContents) {
                    console.log("Updated project file: " + filename);
                    this.versionMap[filename].version++;
                    this.versionMap[filename].snapshot = ts.ScriptSnapshot.fromString(fileContents || this.fs.getFile(filename));
                    return this.documentRegistry.updateDocument(filename, this.compilerOptions, this.versionMap[filename].snapshot, this.versionMap[filename].version.toString());
                }
                getProjectFiles() {
                    return this.projectFiles;
                }
                transpile(fileNames, options) {
                    options = options || this.compilerOptions;
                    this.compilerOptions = options;
                    fileNames.forEach((fileName) => {
                        console.log(`${this.name}:  Transpiling ${fileName}`);
                        let script = this.fs.getFile(fileName);
                        let diagnostics = [];
                        let result = ts.transpile(script, options, fileName, diagnostics);
                        if (diagnostics.length) {
                            this.logErrors(diagnostics);
                        }
                        if (diagnostics.length == 0) {
                            this.fs.writeFile(fileName.replace(".ts", ".js"), result);
                        }
                    });
                }
                getPositionOfLineAndCharacter(file, line, character) {
                    return ts.getPositionOfLineAndCharacter(file, line, character);
                }
                getCompletions(filename, pos) {
                    return this.languageService.getCompletionsAtPosition(filename, pos);
                }
                getCompletionEntryDetails(filename, pos, entryname) {
                    return this.languageService.getCompletionEntryDetails(filename, pos, entryname);
                }
                compile(files, options) {
                    let start = new Date().getTime();
                    options = options || this.compilerOptions;
                    files.forEach((file) => {
                        this.addProjectFile(file);
                    });
                    let errors = [];
                    if (files.length == 0) {
                        this.projectFiles.forEach(filename => {
                            errors = errors.concat(this.compileFile(filename));
                        });
                    }
                    else {
                        files.forEach(filename => {
                            this.versionMap[filename].version++;
                            this.versionMap[filename].snapshot = null;
                            errors = errors.concat(this.compileFile(filename));
                        });
                    }
                    if (errors.length) {
                        this.logErrors(errors);
                    }
                    console.log(`${this.name}: Compiling complete after ${new Date().getTime() - start} ms`);
                }
                deleteProjectFile(filepath) {
                    if (this.versionMap[filepath]) {
                        delete this.versionMap[filepath];
                    }
                    let idx = this.projectFiles.indexOf(filepath);
                    if (idx > -1) {
                        console.log(`delete project file from ${filepath}`);
                        this.projectFiles.splice(idx, 1);
                    }
                }
                renameProjectFile(filepath, newpath) {
                    let oldFile = this.versionMap[filepath];
                    if (oldFile) {
                        console.log(`Rename project file from ${filepath} to ${newpath}`);
                        delete this.versionMap[filepath];
                        this.versionMap[newpath] = oldFile;
                    }
                    let idx = this.projectFiles.indexOf(filepath);
                    if (idx > -1) {
                        console.log(`Update project files array from ${filepath} to ${newpath}`);
                        this.projectFiles[idx] = newpath;
                    }
                }
                reset() {
                    this.projectFiles = [];
                    this.versionMap = {};
                }
                compileFile(filename) {
                    console.log(`${this.name}: Compiling version ${this.versionMap[filename].version} of ${filename}`);
                    try {
                        return this.emitFile(filename);
                    }
                    catch (err) {
                        console.log(`${this.name}: problem encountered compiling ${filename}: ${err}`);
                        return [];
                    }
                }
                emitFile(filename) {
                    let output = this.languageService.getEmitOutput(filename);
                    let allDiagnostics = [];
                    if (output.emitSkipped) {
                        console.log(`${this.name}: Failure Emitting ${filename}`);
                        allDiagnostics = this.languageService.getCompilerOptionsDiagnostics()
                            .concat(this.languageService.getSyntacticDiagnostics(filename))
                            .concat(this.languageService.getSemanticDiagnostics(filename));
                    }
                    output.outputFiles.forEach(o => {
                        this.fs.writeFile(o.name, o.text);
                    });
                    return allDiagnostics;
                }
                logErrors(diagnostics) {
                    let msg = [];
                    diagnostics.forEach(diagnostic => {
                        let message = ts.flattenDiagnosticMessageText(diagnostic.messageText, "\n");
                        if (diagnostic.file) {
                            let d = diagnostic.file.getLineAndCharacterOfPosition(diagnostic.start);
                            let line = d.line;
                            let character = d.character;
                            msg.push(`${this.name}:  Error ${diagnostic.file.fileName} (${line + 1},${character + 1}): ${message}`);
                        }
                        else {
                            msg.push(`${this.name}  Error: ${message}`);
                        }
                    });
                    console.log(`TypeScript Errors:\n${msg.join("\n")}`);
                    throw new Error(`TypeScript Errors:\n${msg.join("\n")}`);
                }
            }
            exports_1("TypescriptLanguageService", TypescriptLanguageService);
        }
    }
});
//# sourceMappingURL=TypescriptLanguageService.js.map