package clockwork;

//Clockwork Haxe Definitions

extern class ClockworkPlayer {


}

//----------------------------------------------------
// MODULE: Player
//----------------------------------------------------


@:native("Clockwork.Player")
extern class Player extends AObject {

    var currentScene: Clockwork.Scene;

      // Construct.
    function new();

    function loadScene(filename: String, ?camera: Clockwork.Camera): Clockwork.Scene;
    function getCurrentScene(): Clockwork.Scene;

}


