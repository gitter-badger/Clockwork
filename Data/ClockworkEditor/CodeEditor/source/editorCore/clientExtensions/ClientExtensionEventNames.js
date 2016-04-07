System.register([], function(exports_1, context_1) {
    "use strict";
    var __moduleName = context_1 && context_1.id;
    var ClientExtensionEventNames;
    return {
        setters:[],
        execute: function() {
            class ClientExtensionEventNames {
            }
            ClientExtensionEventNames.CodeLoadedEvent = "CodeLoadedEvent";
            ClientExtensionEventNames.ConfigureEditorEvent = "ConfigureEditorEvent";
            ClientExtensionEventNames.ResourceSavedEvent = "ResourceSavedEvent";
            ClientExtensionEventNames.ResourceRenamedEvent = "ResourceRenamedEvent";
            ClientExtensionEventNames.ResourceDeletedEvent = "ResourceDeletedEvent";
            ClientExtensionEventNames.ProjectUnloadedEvent = "ProjectUnloadedEvent";
            exports_1("default", ClientExtensionEventNames);
        }
    }
});
//# sourceMappingURL=ClientExtensionEventNames.js.map