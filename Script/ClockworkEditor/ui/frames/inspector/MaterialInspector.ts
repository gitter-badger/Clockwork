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

import ScriptWidget = require("ui/ScriptWidget");
import UIEvents = require("ui/UIEvents");
import EditorUI = require("ui/EditorUI");
import EditorEvents = require("editor/EditorEvents");

import TextureSelector = require("./TextureSelector");

var techniqueSource = new Clockwork.UIMenuItemSource();

var solidSource = new Clockwork.UIMenuItemSource();
solidSource.addItem(new Clockwork.UIMenuItem("Diffuse", "Diffuse"));
solidSource.addItem(new Clockwork.UIMenuItem("Diffuse Emissive", "Diffuse Emissive"));
solidSource.addItem(new Clockwork.UIMenuItem("Diffuse Normal", "Diffuse Normal"));
solidSource.addItem(new Clockwork.UIMenuItem("Diffuse Specular", "Diffuse Specular"));
solidSource.addItem(new Clockwork.UIMenuItem("Diffuse Normal Specular", "Diffuse Normal Specular"));
solidSource.addItem(new Clockwork.UIMenuItem("Diffuse Unlit", "Diffuse Unlit"));
solidSource.addItem(new Clockwork.UIMenuItem("No Texture", "No Texture"));

var tranSource = new Clockwork.UIMenuItemSource();
tranSource.addItem(new Clockwork.UIMenuItem("Alpha", "Alpha"));
tranSource.addItem(new Clockwork.UIMenuItem("Alpha Mask", "Alpha Mask"));
tranSource.addItem(new Clockwork.UIMenuItem("Additive", "Additive"));
tranSource.addItem(new Clockwork.UIMenuItem("Additive Alpha", "Additive Alpha"));
tranSource.addItem(new Clockwork.UIMenuItem("Emissive Alpha", "Emissive Alpha"));
tranSource.addItem(new Clockwork.UIMenuItem("Alpha AO", "Alpha AO"));
tranSource.addItem(new Clockwork.UIMenuItem("Alpha Mask AO", "Alpha Mask AO"));

var lightmapSource = new Clockwork.UIMenuItemSource();
lightmapSource.addItem(new Clockwork.UIMenuItem("Lightmap", "Lightmap"));
lightmapSource.addItem(new Clockwork.UIMenuItem("Lightmap Alpha", "Lightmap Alpha"));

var _ = new Clockwork.UIMenuItem("Solid");
_.subSource = solidSource;
techniqueSource.addItem(_);

_ = new Clockwork.UIMenuItem("Transparency");
_.subSource = tranSource;
techniqueSource.addItem(_);

_ = new Clockwork.UIMenuItem("Lightmap");
_.subSource = lightmapSource;
techniqueSource.addItem(_);

var techniqueLookup = {
    "Techniques/Diff.xml": "Diffuse",
    "Techniques/DiffEmissive.xml": "Diffuse Emissive",
    "Techniques/DiffNormal.xml": "Diffuse Normal",
    "Techniques/DiffSpec.xml": "Diffuse Specular",
    "Techniques/DiffNormalSpec.xml": "Diffuse Normal Specular",
    "Techniques/DiffUnlit.xml": "Diffuse Unlit",
    "Techniques/DiffAlpha.xml": "Alpha",
    "Techniques/DiffAlphaMask.xml": "Alpha Mask",
    "Techniques/DiffAdd.xml": "Additive",
    "Techniques/NoTexture.xml": "No Texture",
    "Techniques/DiffLightMap.xml": "Lightmap",
    "Techniques/DiffLightMapAlpha.xml": "Lightmap Alpha"
};

var techniqueReverseLookup = {};

for (var key in techniqueLookup) {

    techniqueReverseLookup[techniqueLookup[key]] = key;

}


class MaterialInspector extends ScriptWidget {

    currentTexture: Clockwork.UITextureWidget = null;
    tunit: number;
    textureWidget: Clockwork.UITextureWidget

    constructor() {

        super();

        this.fd.id = "Vera";
        this.fd.size = 11;

        this.subscribeToEvent(EditorEvents.RemoveCurrentAssetAssigned, (ev: EditorEvents.RemoveCurrentAssetAssignedEvent) => this.createTextureRemoveButtonCallback(this.tunit, this.textureWidget));
    }

    createShaderParametersSection(): Clockwork.UISection {

        var section = new Clockwork.UISection();
        section.text = "Shader Paramaters";
        section.value = 1;
        section.fontDescription = this.fd;

        var attrsVerticalLayout = new Clockwork.UILayout(Clockwork.UI_AXIS_Y);
        attrsVerticalLayout.spacing = 3;
        attrsVerticalLayout.layoutPosition = Clockwork.UI_LAYOUT_POSITION_LEFT_TOP;
        attrsVerticalLayout.layoutSize = Clockwork.UI_LAYOUT_SIZE_AVAILABLE;

        section.contentRoot.addChild(attrsVerticalLayout);

        var params = this.material.getShaderParameters();

        for (var i in params) {

            var attrLayout = new Clockwork.UILayout();
            attrLayout.layoutDistribution = Clockwork.UI_LAYOUT_DISTRIBUTION_GRAVITY;

            var name = new Clockwork.UITextField();
            name.textAlign = Clockwork.UI_TEXT_ALIGN_LEFT;
            name.skinBg = "InspectorTextAttrName";

            name.text = params[i].name;
            name.fontDescription = this.fd;

            attrLayout.addChild(name);

            var field = new Clockwork.UIEditField();
            field.textAlign = Clockwork.UI_TEXT_ALIGN_LEFT;
            field.skinBg = "TBAttrEditorField";
            field.fontDescription = this.fd;
            var lp = new Clockwork.UILayoutParams();
            lp.width = 140;
            field.layoutParams = lp;

            field.id = params[i].name;
            field.text = params[i].valueString;

            field.subscribeToEvent(field, "WidgetEvent", function(ev: Clockwork.UIWidgetEvent) {

                if (ev.type == Clockwork.UI_EVENT_TYPE_CHANGED) {

                    var field = <Clockwork.UIEditField> ev.target;
                    this.material.setShaderParameter(field.id, field.text);

                }

            }.bind(this));

            attrLayout.addChild(field);

            attrsVerticalLayout.addChild(attrLayout);

            // print(params[i].name, " : ", params[i].value, " : ", params[i].type);

        }


        return section;

    }

    getTextureThumbnail(texture: Clockwork.Texture): Clockwork.Texture {

        if (!texture) return null;

        var db = ToolCore.getAssetDatabase();
        var asset = db.getAssetByPath(texture.name);

        if (!asset)
            return texture;

        var thumbnail = asset.cachePath + "_thumbnail.png";
        var cache = Clockwork.getResourceCache();

        var thumb = <Clockwork.Texture2D> cache.getTempResource("Texture2D", thumbnail);

        if (thumb)
            return thumb;

        return texture;

    }

    onTechniqueSet(techniqueName: string) {

        this.techniqueButton.text = techniqueName;

        var cache = Clockwork.getResourceCache();
        var technique = <Clockwork.Technique> cache.getResource("Technique", techniqueReverseLookup[techniqueName]);
        this.material.setTechnique(0, technique);

    }

    createTechniquePopup(): Clockwork.UIWidget {

        var button = this.techniqueButton = new Clockwork.UIButton();
        var technique = this.material.getTechnique(0);

        button.text = techniqueLookup[technique.name];

        button.fontDescription = this.fd;

        var lp = new Clockwork.UILayoutParams();
        lp.width = 140;
        button.layoutParams = lp;

        button.onClick = function() {

            var menu = new Clockwork.UIMenuWindow(button, "technique popup");

            menu.fontDescription = this.fd;
            menu.show(techniqueSource);

            button.subscribeToEvent(button, "WidgetEvent", function(ev: Clockwork.UIWidgetEvent) {

                if (ev.type != Clockwork.UI_EVENT_TYPE_CLICK)
                    return;

                if (ev.target && ev.target.id == "technique popup") {

                    this.onTechniqueSet(ev.refid);

                }

            }.bind(this));

        }.bind(this);

        return button;

    }

    acceptAssetDrag(importerTypeName: string, ev: Clockwork.DragEndedEvent): ToolCore.AssetImporter {

        var dragObject = ev.dragObject;

        if (dragObject.object && dragObject.object.typeName == "Asset") {

            var asset = <ToolCore.Asset> dragObject.object;

            if (asset.importerTypeName == importerTypeName) {
                return asset.importer;
            }

        }

        return null;

    }


    openTextureSelectionBox(textureUnit: number, textureWidget: Clockwork.UITextureWidget) {

        var inspector = this;

        EditorUI.getModelOps().showResourceSelection("Select Texture", "TextureImporter", "Texture2D", function (asset: ToolCore.Asset, args: any) {

            var texture = <Clockwork.Texture2D>Clockwork.cache.getResource("Texture2D", asset.path);

            if (texture) {
                inspector.material.setTexture(textureUnit, texture);
                textureWidget.texture = inspector.getTextureThumbnail(texture);

                this.sendEvent(EditorEvents.InspectorProjectReference, { "path": texture.getName() });
            }

        });

    }

     // Big Texture Button(referenced texture file path in project frame)
    createTextureButtonCallback(textureUnit:number, textureWidget:Clockwork.UITextureWidget) {

        return () => {

            var texture = this.material.getTexture(textureUnit);

            if (textureWidget.getTexture() != null) {
                this.sendEvent(EditorEvents.InspectorProjectReference, { "path": texture.getName() });
            } else {
                this.openTextureSelectionBox(textureUnit, textureWidget);
            }

        return true;

      };

    }

   // Small Texture Button (Opens texture selection window)
    createTextureReferenceButtonCallback(textureUnit: number, textureWidget: Clockwork.UITextureWidget) {

        return () => {
            this.tunit = textureUnit;
            this.textureWidget = textureWidget;
            this.openTextureSelectionBox(textureUnit, textureWidget);
            return true;
        };
    }

    //Remove Texture Button
    createTextureRemoveButtonCallback(textureUnit: number, textureWidget: Clockwork.UITextureWidget) {

            var texture = this.material.getTexture(textureUnit);

            if (texture != null && textureWidget != null) {
                textureWidget.setTexture(null);
            }

    }

    createTextureSection(): Clockwork.UISection {

        var section = new Clockwork.UISection();
        section.text = "Textures";
        section.value = 1;
        section.fontDescription = this.fd;

        var attrsVerticalLayout = new Clockwork.UILayout(Clockwork.UI_AXIS_Y);
        attrsVerticalLayout.spacing = 3;
        attrsVerticalLayout.layoutPosition = Clockwork.UI_LAYOUT_POSITION_LEFT_TOP;
        attrsVerticalLayout.layoutSize = Clockwork.UI_LAYOUT_SIZE_AVAILABLE;

        section.contentRoot.addChild(attrsVerticalLayout);

        // TODO: Filter on technique
        var textureUnits = [ Clockwork.TU_DIFFUSE, Clockwork.TU_NORMAL, Clockwork.TU_SPECULAR, Clockwork.TU_EMISSIVE ];

        for (var i in textureUnits) {

            var tunit: Clockwork.TextureUnit = textureUnits[i];

            var tunitName = Clockwork.Material.getTextureUnitName(tunit);

            var attrLayout = new Clockwork.UILayout();
            attrLayout.layoutDistribution = Clockwork.UI_LAYOUT_DISTRIBUTION_GRAVITY;

            var name = new Clockwork.UITextField();
            name.textAlign = Clockwork.UI_TEXT_ALIGN_LEFT;
            name.skinBg = "InspectorTextAttrName";

            name.text = tunitName;
            name.fontDescription = this.fd;

            attrLayout.addChild(name);

            var textureWidget = new Clockwork.UITextureWidget();

            var tlp = new Clockwork.UILayoutParams();
            tlp.width = 64;
            tlp.height = 64;
            textureWidget.layoutParams = tlp;
            textureWidget.texture = this.getTextureThumbnail(this.material.getTexture(tunit));

            var textureButton = new Clockwork.UIButton();
            textureButton.skinBg = "TBButton.flatoutline";
            textureButton["tunit"] = tunit;
            textureButton["textureWidget"] = textureWidget;

            //Create drop-down buttons to open Texture Selection Dialog Box
            var textureRefButton = new Clockwork.UIButton();
            textureRefButton.skinBg = "arrow.down";
            textureRefButton["tunit"] = tunit;
            textureRefButton["textureWidget"] = textureWidget;

            textureButton.onClick = this.createTextureButtonCallback(tunit, textureWidget);
            textureRefButton.onClick = this.createTextureReferenceButtonCallback(tunit, textureWidget);

            textureButton.contentRoot.addChild(textureWidget);

            attrLayout.addChild(textureButton);
            attrLayout.addChild(textureRefButton);

            attrsVerticalLayout.addChild(attrLayout);

            // handle dropping of texture on widget
            textureButton.subscribeToEvent(textureButton, "DragEnded", (ev: Clockwork.DragEndedEvent) => {

                var importer = this.acceptAssetDrag("TextureImporter", ev);

                if (importer) {

                    var textureImporter = <ToolCore.TextureImporter> importer;
                    var asset = textureImporter.asset;

                    var texture = <Clockwork.Texture2D> Clockwork.cache.getResource("Texture2D", asset.path);

                    if (texture) {

                        this.material.setTexture(ev.target["tunit"], texture);
                        (<Clockwork.UITextureWidget>ev.target["textureWidget"]).texture = this.getTextureThumbnail(texture);

                        this.sendEvent("InspectorProjectReference", { "path": texture.getName(), "ButtonID": texture.getName() });
                    }
                }
            });

        }

        return section;

    }



    inspect(asset: ToolCore.Asset, material: Clockwork.Material) {

        this.asset = asset;
        this.material = material;

        var mlp = new Clockwork.UILayoutParams();
        mlp.width = 310;

        var materialLayout = new Clockwork.UILayout();
        materialLayout.spacing = 4;

        materialLayout.layoutDistribution = Clockwork.UI_LAYOUT_DISTRIBUTION_GRAVITY;
        materialLayout.layoutPosition = Clockwork.UI_LAYOUT_POSITION_LEFT_TOP;
        materialLayout.layoutParams = mlp;
        materialLayout.axis = Clockwork.UI_AXIS_Y;

        // node attr layout

        var materialSection = new Clockwork.UISection();
        materialSection.text = "Material";
        materialSection.value = 1;
        materialSection.fontDescription = this.fd;
        materialLayout.addChild(materialSection);

        var attrsVerticalLayout = new Clockwork.UILayout(Clockwork.UI_AXIS_Y);
        attrsVerticalLayout.spacing = 3;
        attrsVerticalLayout.layoutPosition = Clockwork.UI_LAYOUT_POSITION_LEFT_TOP;
        attrsVerticalLayout.layoutSize = Clockwork.UI_LAYOUT_SIZE_PREFERRED;

        // NAME
        var nameLayout = new Clockwork.UILayout();
        nameLayout.layoutDistribution = Clockwork.UI_LAYOUT_DISTRIBUTION_GRAVITY;

        var name = new Clockwork.UITextField();
        name.textAlign = Clockwork.UI_TEXT_ALIGN_LEFT;
        name.skinBg = "InspectorTextAttrName";

        name.text = "Name";
        name.fontDescription = this.fd;

        nameLayout.addChild(name);

        var field = new Clockwork.UIEditField();
        field.textAlign = Clockwork.UI_TEXT_ALIGN_LEFT;
        field.skinBg = "TBAttrEditorField";
        field.fontDescription = this.fd;
        var lp = new Clockwork.UILayoutParams();
        lp.width = 160;
        field.layoutParams = lp;

        field.text = Clockwork.splitPath(material.name).fileName;

        nameLayout.addChild(field);

        attrsVerticalLayout.addChild(nameLayout);

        // TECHNIQUE LAYOUT

        var techniqueLayout = new Clockwork.UILayout();
        techniqueLayout.layoutSize = Clockwork.UI_LAYOUT_SIZE_GRAVITY;
        techniqueLayout.layoutDistribution = Clockwork.UI_LAYOUT_DISTRIBUTION_PREFERRED;

        name = new Clockwork.UITextField();
        name.textAlign = Clockwork.UI_TEXT_ALIGN_LEFT;
        name.skinBg = "InspectorTextAttrName";

        name.text = "Technique";
        name.fontDescription = this.fd;

        techniqueLayout.addChild(name);

        var techniquePopup = this.createTechniquePopup();

        techniqueLayout.addChild(techniquePopup);

        attrsVerticalLayout.addChild(techniqueLayout);

        materialSection.contentRoot.addChild(attrsVerticalLayout);

        materialLayout.addChild(this.createTextureSection());
        materialLayout.addChild(this.createShaderParametersSection());

        var button = new Clockwork.UIButton();
        button.fontDescription = this.fd;
        button.gravity = Clockwork.UI_GRAVITY_RIGHT;
        button.text = "Save";

        button.onClick = function() {

            var importer = <ToolCore.MaterialImporter> this.asset.getImporter();
            importer.saveMaterial();

        }.bind(this);

        materialLayout.addChild(button);

        this.addChild(materialLayout);

    }

    techniqueButton: Clockwork.UIButton;
    material: Clockwork.Material;
    asset: ToolCore.Asset;
    nameTextField: Clockwork.UITextField;
    fd: Clockwork.UIFontDescription = new Clockwork.UIFontDescription();


}

export = MaterialInspector;
