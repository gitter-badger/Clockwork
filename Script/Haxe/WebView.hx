package clockwork;

//Clockwork Haxe Definitions

extern class WebView {


}

//----------------------------------------------------
// MODULE: WebView
//----------------------------------------------------


@:native("Clockwork.UIWebView")
extern class UIWebView extends UIWidget {

    var webClient: WebClient;
    var webTexture2D: WebTexture2D;

    function new(?initialURL: String);

      // Get the widget's WebClient
    function getWebClient(): WebClient;
      // Get the WebTexture in use by the WebView
    function getWebTexture2D(): WebTexture2D;

}

@:native("Clockwork.WebBrowserHost")
extern class WebBrowserHost extends AObject {

      // Construct.
    function new();

      // Set global property object values, available as read only on page
    function setGlobalBoolProperty(globalVar: String, property: String, value: Bool): Void;
    function setGlobalStringProperty(globalVar: String, property: String, value: String): Void;
    function setGlobalNumberProperty(globalVar: String, property: String, value: Float): Void;

}

@:native("Clockwork.WebClient")
extern class WebClient extends AObject {

    var webRenderHandler: WebRenderHandler;

      // Construct.
    function new();

      // Create the browser, call only once initialized with handlers
    function createBrowser(initialURL: String, width: Int, height: Int): Bool;
      // Set the browser's width and height
    function setSize(width: Int, height: Int): Void;
      // Send a mouse click event to the browser
    function sendMouseClickEvent(x: Int, y: Int, button: UInt, mouseUp: Bool, modifier: UInt, ?clickCount: Int): Void;
      // Send a mouse press event to the browser
    function sendMousePressEvent(x: Int, y: Int, ?button: UInt, ?modifier: UInt, ?clickCount: Int): Void;
      // Send a mouse move event to the browser
    function sendMouseMoveEvent(x: Int, y: Int, modifier: UInt, ?mouseLeave: Bool): Void;
      // Send a mouse wheel event to the browser
    function sendMouseWheelEvent(x: Int, y: Int, modifier: UInt, deltaX: Int, deltaY: Int): Void;
      // Send a focus event to the browser
    function sendFocusEvent(?focus: Bool): Void;
      // Invoke the Cut shortcut on the browser's main frame
    function shortcutCut(): Void;
      // Invoke the Copy shortcut on the browser's main frame
    function shortcutCopy(): Void;
      // Invoke the Paste shortcut on the browser's main frame
    function shortcutPaste(): Void;
      // Invoke the SelectAll shortcut on the browser's main frame
    function shortcutSelectAll(): Void;
      // Invoke the Undo shortcut on the browser's main frame
    function shortcutUndo(): Void;
      // Invoke the Redo shortcut on the browser's main frame
    function shortcutRedo(): Void;
      // Invoke the Delete shortcut on the browser's main frame
    function shortcutDelete(): Void;
      // Execute some JavaScript in the browser
    function executeJavaScript(script: String): Void;
      // Eval some JavaScript in the browser (async return value referenced by evalID)
    function evalJavaScript(evalID: UInt, script: String): Void;
      // Returns true if the page is currently loading
    function isLoading(): Bool;
      // Load the specified url into the main frame of the browser
    function loadURL(url: String): Void;
      // Load html source into main frame of browser
    function loadString(source: String, ?url: String): Void;
      // Go back in page history
    function goBack(): Void;
      // Go forward in page history
    function goForward(): Void;
      // Reload the current page
    function reload(): Void;
      // Set the render handler for this client
    function setWebRenderHandler(handler: WebRenderHandler): Void;

}

@:native("Clockwork.WebRenderHandler")
extern class WebRenderHandler extends AObject {

    var width: Int;
    var height: Int;
    var webClient: WebClient;

      // Construct.
    function new();

      // Get the current renderer width
    function getWidth(): Int;
      // Get the current renderer height
    function getHeight(): Int;
      // Get the WebClient associated with the render handler
    function getWebClient(): WebClient;
      // Set the dimensions of the render handler
    function setSize(width: Int, height: Int): Void;
      // Set the render handlers WebClient
    function setWebClient(webClient: WebClient): Void;

}

@:native("Clockwork.WebTexture2D")
extern class WebTexture2D extends WebRenderHandler {

    var texture2D: Clockwork.Texture2D;
    var clearColor: Clockwork.Color;

      // Construct.
    function new();

      // Get the current width of the texture
    @:overload(function(): Int{})
    override function getWidth(): Int;
      // Get the current height of the texture
    @:overload(function(): Int{})
    override function getHeight(): Int;
      // Get the Texture2D associated with the WebTexture2D
    function getTexture2D(): Clockwork.Texture2D;
      // get the clear color for the WebTexture
    function getClearColor(): Clockwork.Color;
      // Set the dimensions of the texture
    @:overload(function(width: Int, height: Int): Void{})
    override function setSize(width: Int, height: Int): Void;
      // Set the clear color for the WebTexture
    function setClearColor(color: Clockwork.Color): Void;

}


