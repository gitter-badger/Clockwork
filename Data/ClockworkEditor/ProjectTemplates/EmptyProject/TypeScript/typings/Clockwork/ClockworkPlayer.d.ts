//Clockwork TypeScript Definitions


/// <reference path="Clockwork.d.ts" />

declare module ClockworkPlayer {


//----------------------------------------------------
// MODULE: Player
//----------------------------------------------------


   export class Player extends Clockwork.AObject {

      currentScene: Clockwork.Scene;

      // Construct.
      constructor();

      loadScene(filename: string, camera?: Clockwork.Camera): Clockwork.Scene;
      getCurrentScene(): Clockwork.Scene;

   }



}
