System.register(["../interop", "./ClientExtensionServices", "./languageExtensions/typescript/TypescriptLanguageExtension", "./languageExtensions/javascript/JavascriptLanguageExtension"], function(exports_1, context_1) {
    "use strict";
    var __moduleName = context_1 && context_1.id;
    var interop_1, ClientExtensionServices, TypescriptLanguageExtension_1, JavascriptLanguageExtension_1;
    var ClientServiceLocatorType, serviceLocator;
    return {
        setters:[
            function (interop_1_1) {
                interop_1 = interop_1_1;
            },
            function (ClientExtensionServices_1) {
                ClientExtensionServices = ClientExtensionServices_1;
            },
            function (TypescriptLanguageExtension_1_1) {
                TypescriptLanguageExtension_1 = TypescriptLanguageExtension_1_1;
            },
            function (JavascriptLanguageExtension_1_1) {
                JavascriptLanguageExtension_1 = JavascriptLanguageExtension_1_1;
            }],
        execute: function() {
            class ClientServiceLocatorType {
                constructor() {
                    this.eventDispatcher = new ClientExtensionServices.EventDispatcher();
                    this.services = new ClientExtensionServices.ExtensionServiceRegistry();
                    this.services.subscribeToEvents(this);
                }
                getHostInterop() {
                    return interop_1.default.getInstance();
                }
                loadService(service) {
                    try {
                        this.services.register(service);
                        service.initialize(this);
                    }
                    catch (e) {
                        alert(`Extension Error:\n Error detected in extension ${service.name}\n \n ${e.stack}`);
                    }
                }
                sendEvent(eventType, data) {
                    if (this.eventDispatcher) {
                        this.eventDispatcher.sendEvent(eventType, data);
                    }
                }
                subscribeToEvent(eventType, callback) {
                    if (this.eventDispatcher) {
                        this.eventDispatcher.subscribeToEvent(eventType, callback);
                    }
                }
            }
            exports_1("ClientServiceLocatorType", ClientServiceLocatorType);
            serviceLocator = new ClientServiceLocatorType();
            exports_1("default",serviceLocator);
            serviceLocator.loadService(new TypescriptLanguageExtension_1.default());
            serviceLocator.loadService(new JavascriptLanguageExtension_1.default());
        }
    }
});
//# sourceMappingURL=ServiceLocator.js.map