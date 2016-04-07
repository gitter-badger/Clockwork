//
// Copyright (c) 2014-2015, CLOCKWORK STUDIOS All rights reserved
// LICENSE: Clockwork Game Engine Editor and Tools EULA
// Please see LICENSE_CLOCKWORK_EDITOR_AND_TOOLS.md in repository root for
// license information: https://github.com/ClockworkGameEngine/ClockworkGameEngine
//

/// <reference path="Editor.d.ts" />

declare module Editor.EditorEvents {

    export interface ModalErrorEvent {

        title: string;
        message: string;

    }

    export interface PlayerLogEvent {

        message: string;
        level: number;

    }

    export interface ActiveSceneEditorChangeEvent {

        sceneEditor: Editor.SceneEditor3D;

    }

    export interface SceneClosedEvent {

        scene: Clockwork.Scene;

    }

    export interface ContentFolderChangedEvent {

        path: string;

    }

    export interface LoadProjectEvent {

        // The full path to the .clockwork file
        path: string;

    }

    /**
     * Called once the resource has been saved
     * @type {String}
     */
    export interface SaveResourceEvent {

        // The full path to the resource to save / empty or undefined for current
        path: string;

    }

    export interface LoadResourceEvent {

        // The full path to the resource to load
        path: string;

    }

    export interface EditorFileEvent {
        filename: string;
        fileExt: string;
        editor: any;
    }

    export interface CodeLoadedEvent extends EditorFileEvent {
        code: string;
    }

    export interface EditorCloseResourceEvent {

        editor: Editor.ResourceEditor;
        navigateToAvailableResource: boolean;

    }

    export interface EditResourceEvent {

        // The full path to the resource to edit
        path: string;

    }

    /**
     * Called once the resource has been deleted
     * @type {String}
     */
    export interface DeleteResourceEvent {

        // The full path to the resource to edit
        path: string;

    }

    /**
     * Called once the resource has been renamed
     * @type {String}
     */
    export interface RenameResourceEvent {

        /**
         * Original path of the resource
         * @type {string}
         */
        path: string;

        /**
         * New path of the resource
         * @type {string}
         */
        newPath: string;

        /**
         * New base name of the resource (no path or extension)
         * @type {string}
         */
        newName?: string;

        // the asset being changed
        asset?: ToolCore.Asset;
    }

    export interface SceneEditStateChangeEvent {

        serializable: Clockwork.Serializable;

    }
}

declare module Editor.Extensions {

    /**
     * Base interface for any editor services.
     */
    export interface EditorService {
        /**
         * Unique name of this service
         * @type {string}
         */
        name: string;

        /**
         * Description of this service
         * @type {string}
         */
        description: string;

    }

    interface EventDispatcher {
        /**
         * Send a custom event.  This can be used by services to publish custom events
         * @param  {string} eventType
         * @param  {any} data
         */
        sendEvent(eventType: string, data: any);

        /**
         * Subscribe to an event and provide a callback.  This can be used by services to subscribe to custom events
         * @param  {string} eventType
         * @param  {any} callback
         */
        subscribeToEvent(eventType, callback);
    }

    /**
     * Generic service locator of editor services that may be injected by either a plugin
     * or by the editor itself.
     */
    export interface ServiceLoader extends EventDispatcher {
        /**
         * Loads a service into a service registry
         * @param  {EditorService} service
         */
        loadService(service: EditorService): void;
    }

    /**
     * Service registry interface for registering services
     */
    export interface ServiceRegistry<T extends EditorService> {
        registeredServices: T[];

        /**
         * Adds a service to the registered services list for this type of service
         * @param  {T}      service the service to register
         */
        register(service: T);
    }
}

declare module Editor.HostExtensions {

    /**
     * Generic service locator of editor services that may be injected by either a plugin
     * or by the editor itself.
     */
    export interface HostServiceLocator extends Editor.Extensions.ServiceLoader {
        resourceServices: Editor.Extensions.ServiceRegistry<ResourceService>;
        projectServices: Editor.Extensions.ServiceRegistry<ProjectService>;
    }

    export interface HostEditorService extends Editor.Extensions.EditorService {
        /**
         * Called by the service locator at load time
         */
        initialize(serviceLocator: Editor.Extensions.ServiceLoader);
    }

    export interface ResourceService extends Editor.Extensions.EditorService {
        save?(ev: EditorEvents.SaveResourceEvent);
        delete?(ev: EditorEvents.DeleteResourceEvent);
        rename?(ev: EditorEvents.RenameResourceEvent);
    }

    export interface ProjectService extends Editor.Extensions.EditorService {
        projectUnloaded?();
        projectLoaded?(ev: EditorEvents.LoadProjectEvent);
        playerStarted?();
    }
}

/**
 * Interfaces for client extensions
 */
declare module Editor.ClientExtensions {

    /**
     * Generic service locator of editor services that may be injected by either a plugin
     * or by the editor itself.
     */
    export interface ClientServiceLocator extends Editor.Extensions.ServiceLoader {
        getHostInterop(): HostInterop;
    }

    export interface ClientEditorService extends Editor.Extensions.EditorService {
        /**
         * Called by the service locator at load time
         */
        initialize(serviceLocator: ClientServiceLocator);
    }

    export interface WebViewService extends Editor.Extensions.EditorService {
        configureEditor?(ev: EditorEvents.EditorFileEvent);
        codeLoaded?(ev: EditorEvents.CodeLoadedEvent);
        save?(ev: EditorEvents.SaveResourceEvent);
        delete?(ev: EditorEvents.DeleteResourceEvent);
        rename?(ev: EditorEvents.RenameResourceEvent);
        projectUnloaded?();
    }

    export interface ClockworkErrorMessage {
        error_code: number;
        error_message: string;
    }

    /**
     * Interface for functions that are available from the client web view to call on the host
     */
    export interface HostInterop {

        /**
         * Called from the host to notify the client what file to load
         * @param  {string} codeUrl
         */
        loadCode(codeUrl: string);

        /**
         * Save the contents of the editor
         * @return {Promise}
         */
        saveCode(): PromiseLike<{}>;

        /**
         * Save the contents of a file as filename
         * @param  {string} filename
         * @param  {string} fileContents
         * @return {Promise}
         */
        saveFile(filename: string, fileContents: string): PromiseLike<{}>;

        /**
         * Queries the host for a particular resource and returns it in a promise
         * @param  {string} codeUrl
         * @return {Promise}
         */
        getResource(codeUrl: string): PromiseLike<{}>;

        /**
         * Returns a file resource from the resources directory
         * @param  {string} filename name and path of file under the project directory or a fully qualified file name
         * @return {Promise}
         */
        getFileResource(filename: string): PromiseLike<{}>;

        /**
         * Notify the host that the contents of the editor has changed
         */
        notifyEditorChange();
    }
}
