System.register(["./editor", "../clientExtensions/ServiceLocator", "../interop", "../clientExtensions/ClientExtensionEventNames"], function(exports_1, context_1) {
    "use strict";
    var __moduleName = context_1 && context_1.id;
    var editor_1, ServiceLocator_1, interop_1, ClientExtensionEventNames_1;
    function configure(fileExt, filename) {
        editor_1.default.setTheme("ace/theme/monokai");
        editor_1.default.session.setMode("ace/mode/javascript");
        ServiceLocator_1.default.sendEvent(ClientExtensionEventNames_1.default.ConfigureEditorEvent, {
            fileExt: fileExt,
            filename: filename,
            editor: editor_1.default
        });
    }
    exports_1("configure", configure);
    function getSourceText() {
        return editor_1.default.session.getValue();
    }
    exports_1("getSourceText", getSourceText);
    function loadCodeIntoEditor(code, filename, fileExt) {
        editor_1.default.session.setValue(code);
        editor_1.default.gotoLine(0);
        editor_1.default.getSession().on("change", function (e) {
            interop_1.default.getInstance().notifyEditorChange();
        });
        ServiceLocator_1.default.sendEvent(ClientExtensionEventNames_1.default.CodeLoadedEvent, {
            code: code,
            filename: filename,
            fileExt: fileExt,
            editor: editor_1.default
        });
    }
    exports_1("loadCodeIntoEditor", loadCodeIntoEditor);
    function projectUnloaded() {
        ServiceLocator_1.default.sendEvent(ClientExtensionEventNames_1.default.ProjectUnloadedEvent, null);
    }
    exports_1("projectUnloaded", projectUnloaded);
    function resourceRenamed(path, newPath) {
        let data = {
            path: path,
            newPath: newPath
        };
        ServiceLocator_1.default.sendEvent(ClientExtensionEventNames_1.default.ResourceRenamedEvent, data);
    }
    exports_1("resourceRenamed", resourceRenamed);
    function resourceDeleted(path) {
        let data = {
            path: path
        };
        ServiceLocator_1.default.sendEvent(ClientExtensionEventNames_1.default.ResourceDeletedEvent, data);
    }
    exports_1("resourceDeleted", resourceDeleted);
    return {
        setters:[
            function (editor_1_1) {
                editor_1 = editor_1_1;
            },
            function (ServiceLocator_1_1) {
                ServiceLocator_1 = ServiceLocator_1_1;
            },
            function (interop_1_1) {
                interop_1 = interop_1_1;
            },
            function (ClientExtensionEventNames_1_1) {
                ClientExtensionEventNames_1 = ClientExtensionEventNames_1_1;
            }],
        execute: function() {
        }
    }
});
//# sourceMappingURL=editorCommands.js.map