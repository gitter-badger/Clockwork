//////////////////////////////////////////////////////////
// IMPORTANT: THIS FILE IS GENERATED, CHANGES WILL BE LOST
//////////////////////////////////////////////////////////

// Clockwork TypeScript Definitions

/// <reference path="Clockwork.d.ts" />

declare module ClockworkNET {


   // enum CSComponentMethod
   export type CSComponentMethod = number;
   export var CSComponentMethod_Start: CSComponentMethod;
   export var CSComponentMethod_DelayedStart: CSComponentMethod;
   export var CSComponentMethod_Update: CSComponentMethod;
   export var CSComponentMethod_PostUpdate: CSComponentMethod;
   export var CSComponentMethod_FixedUpdate: CSComponentMethod;
   export var CSComponentMethod_PostFixedUpdate: CSComponentMethod;


//----------------------------------------------------
// MODULE: NETCore
//----------------------------------------------------


   export class NETCore extends Clockwork.AObject {

      context: Clockwork.Context;

      // Construct.
      constructor();

      shutdown(): void;
      addAssemblyLoadPath(assemblyPath: string): void;
      execAssembly(assemblyName: string, args: string[]): number;
      waitForDebuggerConnect(): void;
      // to get a reference from
      static getContext(): Clockwork.Context;

   }

   export class NETVariantMap extends Clockwork.RefCounted {

      constructor();

      getBool(key: string): boolean;
      getInt(key: string): number;
      getFloat(key: string): number;
      getVector3(key: string): Clockwork.Vector3;
      getQuaternion(key: string): Clockwork.Quaternion;
      getPtr(key: string): Clockwork.RefCounted;
      getString(key: string): string;
      getVariantType(key: string): Clockwork.VariantType;
      getResourceFromRef(key: string): Clockwork.Resource;
      contains(key: string): boolean;

   }



//----------------------------------------------------
// MODULE: NETScript
//----------------------------------------------------


   export class CSComponent extends Clockwork.ScriptComponent {

      componentClassName: string;
      componentFile: Clockwork.ScriptComponentFile;
      assemblyFile: CSComponentAssembly;

      // Construct.
      constructor();

      applyAttributes(): void;
      // Handle enabled/disabled state change. Changes update event subscription.
      onSetEnabled(): void;
      applyFieldValues(): void;
      setComponentClassName(name: string): void;
      getComponentClassName(): string;
      getComponentFile(): Clockwork.ScriptComponentFile;
      getAssemblyFile(): CSComponentAssembly;
      setAssemblyFile(assemblyFile: CSComponentAssembly): void;

   }

   export class CSComponentAssembly extends Clockwork.ScriptComponentFile {

      classNames: string[];

      // Construct.
      constructor();

      createCSComponent(classname: string): CSComponent;
      // Only valid in editor, as we don't inspect assembly at runtime
      getClassNames(): string[];

   }

   export class CSManaged extends Clockwork.AObject {

      // Construct.
      constructor();

      initialize(): boolean;
      nETUpdate(timeStep: number): void;
      cSComponentCreate(assemblyName: string, componentName: string): CSComponent;
      cSComponentApplyFields(component: CSComponent, fieldMapPtr: NETVariantMap): void;
      cSComponentCallMethod(id: number, methodID: CSComponentMethod, value?: number): void;

   }

   export class CSScriptObject extends Clockwork.AObject {

      // Construct.
      constructor();


   }



}
