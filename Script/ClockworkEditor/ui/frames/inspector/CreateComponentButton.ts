//
// Copyright (c) 2014-2016 THUNDERBEAST GAMES LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

var audioCreateSource = new Clockwork.UIMenuItemSource();

audioCreateSource.addItem(new Clockwork.UIMenuItem("SoundListener", "SoundListener"));
audioCreateSource.addItem(new Clockwork.UIMenuItem("SoundSource", "SoundSource"));
audioCreateSource.addItem(new Clockwork.UIMenuItem("SoundSource3D", "SoundSource3D"));

var _2DCreateSource = new Clockwork.UIMenuItemSource();
_2DCreateSource.addItem(new Clockwork.UIMenuItem("PhysicsWorld2D", "PhysicsWorld2D"));
_2DCreateSource.addItem(new Clockwork.UIMenuItem("StaticSprite2D", "StaticSprite2D"));
_2DCreateSource.addItem(new Clockwork.UIMenuItem("AnimatedSprite2D", "AnimatedSprite2D"));
_2DCreateSource.addItem(new Clockwork.UIMenuItem("ParticleEmitter2D", "ParticleEmitter2D"));
_2DCreateSource.addItem(new Clockwork.UIMenuItem("PointLight2D", "PointLight2D"));
_2DCreateSource.addItem(new Clockwork.UIMenuItem("DirectionalLight2D", "DirectionalLight2D"));
_2DCreateSource.addItem(new Clockwork.UIMenuItem("RigidBody2D", "RigidBody2D"));
_2DCreateSource.addItem(new Clockwork.UIMenuItem("CollisionBox2D", "CollisionBox2D"));
_2DCreateSource.addItem(new Clockwork.UIMenuItem("CollisionCircle2D", "CollisionCircle2D"));
_2DCreateSource.addItem(new Clockwork.UIMenuItem("TileMap2D", "TileMap2D"));

var geometryCreateSource = new Clockwork.UIMenuItemSource();

geometryCreateSource.addItem(new Clockwork.UIMenuItem("StaticModel", "StaticModel"));
geometryCreateSource.addItem(new Clockwork.UIMenuItem("AnimatedModel", "AnimatedModel"));
geometryCreateSource.addItem(new Clockwork.UIMenuItem("AnimationController", "AnimationController"));
geometryCreateSource.addItem(new Clockwork.UIMenuItem("BillboardSet", "BillboardSet"));
geometryCreateSource.addItem(new Clockwork.UIMenuItem("CustomGeometry", "CustomGeometry"));
geometryCreateSource.addItem(new Clockwork.UIMenuItem("ParticleEmitter", "ParticleEmitter"));
geometryCreateSource.addItem(new Clockwork.UIMenuItem("Skybox", "SkyBox"));
geometryCreateSource.addItem(new Clockwork.UIMenuItem("StaticModelGroup", "StaticModelGroup"));
geometryCreateSource.addItem(new Clockwork.UIMenuItem("Terrain", "Terrain"));
geometryCreateSource.addItem(new Clockwork.UIMenuItem("Text3D", "create component"));
geometryCreateSource.addItem(new Clockwork.UIMenuItem("Water", "Water"));

var logicCreateSource = new Clockwork.UIMenuItemSource();

logicCreateSource.addItem(new Clockwork.UIMenuItem("JSComponent", "JSComponent"));
logicCreateSource.addItem(new Clockwork.UIMenuItem("CSComponent", "CSComponent"));
logicCreateSource.addItem(new Clockwork.UIMenuItem("AnimationController", "AnimationController"));
logicCreateSource.addItem(new Clockwork.UIMenuItem("SplinePath", "SplinePath"));

var navigationCreateSource = new Clockwork.UIMenuItemSource();

navigationCreateSource.addItem(new Clockwork.UIMenuItem("CrowdAgent", "CrowdAgent"));
navigationCreateSource.addItem(new Clockwork.UIMenuItem("CrowdManager", "CrowdManager"));
navigationCreateSource.addItem(new Clockwork.UIMenuItem("NavArea", "NavArea"));
navigationCreateSource.addItem(new Clockwork.UIMenuItem("Navigable", "Navigable"));
navigationCreateSource.addItem(new Clockwork.UIMenuItem("NavigationMesh", "NavigationMesh"));
navigationCreateSource.addItem(new Clockwork.UIMenuItem("DynamicNavigationMesh", "DynamicNavigationMesh"));
navigationCreateSource.addItem(new Clockwork.UIMenuItem("Obstacle", "Obstacle"));
navigationCreateSource.addItem(new Clockwork.UIMenuItem("OffMeshConnection", "OffMeshConnection"));

var networkCreateSource = new Clockwork.UIMenuItemSource();

networkCreateSource.addItem(new Clockwork.UIMenuItem("Network Priority", "create component"));

var physicsCreateSource = new Clockwork.UIMenuItemSource();

physicsCreateSource.addItem(new Clockwork.UIMenuItem("CollisionShape", "CollisionShape"));
physicsCreateSource.addItem(new Clockwork.UIMenuItem("Constraint", "Constraint"));
physicsCreateSource.addItem(new Clockwork.UIMenuItem("RigidBody", "RigidBody"));

var sceneCreateSource = new Clockwork.UIMenuItemSource();

sceneCreateSource.addItem(new Clockwork.UIMenuItem("Camera", "Camera"));
sceneCreateSource.addItem(new Clockwork.UIMenuItem("Light", "Light"));
sceneCreateSource.addItem(new Clockwork.UIMenuItem("Zone", "Zone"));

var subsystemCreateSource = new Clockwork.UIMenuItemSource();

subsystemCreateSource.addItem(new Clockwork.UIMenuItem("DebugRenderer", "create component"));
subsystemCreateSource.addItem(new Clockwork.UIMenuItem("Octree", "create component"));
subsystemCreateSource.addItem(new Clockwork.UIMenuItem("PhysicsWorld", "create component"));

var editorCreateSource = new Clockwork.UIMenuItemSource();

editorCreateSource.addItem(new Clockwork.UIMenuItem("CubemapGenerator", "CubemapGenerator"));


var componentCreateSource = new Clockwork.UIMenuItemSource();

var sources = {
    Audio: audioCreateSource,
    "2D": _2DCreateSource,
    Geometry: geometryCreateSource,
    Logic: logicCreateSource,
    Navigation: navigationCreateSource,
    Network: networkCreateSource,
    Physics: physicsCreateSource,
    Scene: sceneCreateSource,
    SubSystem: subsystemCreateSource,
    Editor : editorCreateSource
};

for (var sub in sources) {

    var item = new Clockwork.UIMenuItem(sub);
    item.subSource = sources[sub];
    componentCreateSource.addItem(item);

}


class CreateComponentButton extends Clockwork.UIButton {

    constructor() {

        super();

        this.fd.id = "Vera";
        this.fd.size = 11;

        this.text = "Create Component";

        this.subscribeToEvent("WidgetEvent", (data) => this.handleWidgetEvent(data));

    }

    // note instance method
    onClick = () => {

        var menu = new Clockwork.UIMenuWindow(this, "create component popup");
        menu.fontDescription = this.fd;
        menu.show(componentCreateSource);
    };

    handleWidgetEvent(ev: Clockwork.UIWidgetEvent) {

        if (ev.type != Clockwork.UI_EVENT_TYPE_CLICK)
            return;

        if (ev.target && ev.target.id == "create component popup") {

            this.sendEvent("SelectionCreateComponent", { componentTypeName : ev.refid});

            return true;

        }

    }

    fd: Clockwork.UIFontDescription = new Clockwork.UIFontDescription();

}

export = CreateComponentButton;
