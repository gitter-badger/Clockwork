System.register([], function(exports_1, context_1) {
    "use strict";
    var __moduleName = context_1 && context_1.id;
    var JavascriptLanguageExtension;
    return {
        setters:[],
        execute: function() {
            class JavascriptLanguageExtension {
                constructor() {
                    this.name = "ClientJavascriptLanguageExtension";
                    this.description = "Javascript language services for the editor.";
                }
                initialize(serviceLocator) {
                    this.serviceLocator = serviceLocator;
                }
                isValidFiletype(path) {
                    let ext = path.split(".").pop();
                    return ext == "js";
                }
                configureEditor(ev) {
                    if (this.isValidFiletype(ev.filename)) {
                        let editor = ev.editor;
                        editor.session.setMode("ace/mode/javascript");
                        editor.setOptions({
                            enableBasicAutocompletion: true,
                            enableLiveAutocompletion: true
                        });
                    }
                }
            }
            exports_1("default", JavascriptLanguageExtension);
        }
    }
});
//# sourceMappingURL=JavascriptLanguageExtension.js.map