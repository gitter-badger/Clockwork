System.register([], function(exports_1, context_1) {
    "use strict";
    var __moduleName = context_1 && context_1.id;
    var editor;
    return {
        setters:[],
        execute: function() {
            editor = ace.edit("editor");
            editor.setOptions({
                enableBasicAutocompletion: true,
                enableSnippets: true,
                enableLiveAutocompletion: true,
                theme: "ace/theme/monokai"
            });
            exports_1("default",editor);
        }
    }
});
//# sourceMappingURL=editor.js.map