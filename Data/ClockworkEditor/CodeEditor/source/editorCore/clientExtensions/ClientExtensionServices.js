System.register(["./ClientExtensionEventNames"], function(exports_1, context_1) {
    "use strict";
    var __moduleName = context_1 && context_1.id;
    var ClientExtensionEventNames_1;
    var EventDispatcher, ServiceRegistry, ExtensionServiceRegistry;
    return {
        setters:[
            function (ClientExtensionEventNames_1_1) {
                ClientExtensionEventNames_1 = ClientExtensionEventNames_1_1;
            }],
        execute: function() {
            class EventDispatcher {
                constructor() {
                    this.subscriptions = [];
                }
                sendEvent(eventType, data) {
                    this.subscriptions.forEach(sub => {
                        if (sub.eventName == eventType) {
                            sub.callback(data);
                        }
                    });
                }
                subscribeToEvent(eventType, callback) {
                    this.subscriptions.push({
                        eventName: eventType,
                        callback: callback
                    });
                }
            }
            exports_1("EventDispatcher", EventDispatcher);
            class ServiceRegistry {
                constructor() {
                    this.registeredServices = [];
                }
                register(service) {
                    this.registeredServices.push(service);
                }
            }
            class ExtensionServiceRegistry extends ServiceRegistry {
                subscribeToEvents(eventDispatcher) {
                    eventDispatcher.subscribeToEvent(ClientExtensionEventNames_1.default.CodeLoadedEvent, (ev) => this.codeLoaded(ev));
                    eventDispatcher.subscribeToEvent(ClientExtensionEventNames_1.default.ConfigureEditorEvent, (ev) => this.configureEditor(ev));
                    eventDispatcher.subscribeToEvent(ClientExtensionEventNames_1.default.ResourceRenamedEvent, (ev) => this.renameResource(ev));
                    eventDispatcher.subscribeToEvent(ClientExtensionEventNames_1.default.ProjectUnloadedEvent, (ev) => this.projectUnloaded());
                    eventDispatcher.subscribeToEvent(ClientExtensionEventNames_1.default.ResourceDeletedEvent, (ev) => this.deleteResource(ev));
                }
                codeLoaded(ev) {
                    this.registeredServices.forEach((service) => {
                        try {
                            if (service.codeLoaded) {
                                service.codeLoaded(ev);
                            }
                        }
                        catch (e) {
                            alert(`Extension Error:\n Error detected in extension ${service.name}\n \n ${e.stack}`);
                        }
                    });
                }
                saveResource(ev) {
                    this.registeredServices.forEach((service) => {
                        try {
                            if (service.save) {
                                service.save(ev);
                            }
                        }
                        catch (e) {
                            alert(`Error detected in extension ${service.name}\n \n ${e.stack}`);
                        }
                    });
                }
                deleteResource(ev) {
                    this.registeredServices.forEach((service) => {
                        try {
                            if (service.delete) {
                                service.delete(ev);
                            }
                        }
                        catch (e) {
                            alert(`Error detected in extension ${service.name}\n \n ${e.stack}`);
                        }
                    });
                }
                renameResource(ev) {
                    this.registeredServices.forEach((service) => {
                        try {
                            if (service.rename) {
                                service.rename(ev);
                            }
                        }
                        catch (e) {
                            alert(`Error detected in extension ${service.name}\n \n ${e.stack}`);
                        }
                    });
                }
                configureEditor(ev) {
                    this.registeredServices.forEach((service) => {
                        try {
                            if (service.configureEditor) {
                                service.configureEditor(ev);
                            }
                        }
                        catch (e) {
                            alert(`Extension Error:\n Error detected in extension ${service.name}\n \n ${e.stack}`);
                        }
                    });
                }
                projectUnloaded() {
                    this.registeredServices.forEach((service) => {
                        try {
                            if (service.projectUnloaded) {
                                service.projectUnloaded();
                            }
                        }
                        catch (e) {
                            alert(`Extension Error:\n Error detected in extension ${service.name}\n \n ${e.stack}`);
                        }
                    });
                }
            }
            exports_1("ExtensionServiceRegistry", ExtensionServiceRegistry);
        }
    }
});
//# sourceMappingURL=ClientExtensionServices.js.map