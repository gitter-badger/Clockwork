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

class InspectorUtils {

  private static Ctor = (() => {

    var fd = InspectorUtils.attrFontDesc = new Clockwork.UIFontDescription();
    fd.id = "Vera";
    fd.size = 11;

  })();

  static createSeparator(parent:Clockwork.UIWidget):Clockwork.UISeparator {

    var sep = new Clockwork.UISeparator();

    sep.gravity = Clockwork.UI_GRAVITY_LEFT_RIGHT;
    sep.skinBg = "AESeparator";

    parent.addChild(sep);
    return sep;

  }

  static createContainer():Clockwork.UIContainer {

    var container = new Clockwork.UIContainer();

    container.skinBg = "AEContainer";

    return container;

  }

  static createAttrName(name:string):Clockwork.UITextField {

    var nameField = new Clockwork.UITextField();
    nameField.textAlign = Clockwork.UI_TEXT_ALIGN_LEFT;
    nameField.skinBg = "InspectorTextAttrName";
    nameField.text = name;
    nameField.fontDescription = InspectorUtils.attrFontDesc;

    // atttribute name layout param
    var atlp = new Clockwork.UILayoutParams();
    atlp.width = 120;
    nameField.layoutParams = atlp;

    return nameField;
  }

  static createEditField():Clockwork.UIEditField {

    var edit = new Clockwork.UIEditField();
    edit.id = "editfield";
    edit.textAlign = Clockwork.UI_TEXT_ALIGN_LEFT;
    edit.skinBg = "TBAttrEditorField";
    edit.fontDescription = InspectorUtils.attrFontDesc;
    var lp = new Clockwork.UILayoutParams();
    lp.width = 160;
    lp.height = 24;
    edit.layoutParams = lp;

    return edit;

  }

  static createAttrEditField(name:string, parent:Clockwork.UIWidget):Clockwork.UIEditField {

    var attrLayout = new Clockwork.UILayout();

    attrLayout.layoutSize = Clockwork.UI_LAYOUT_SIZE_AVAILABLE;
    attrLayout.gravity = Clockwork.UI_GRAVITY_LEFT_RIGHT;
    attrLayout.layoutDistribution = Clockwork.UI_LAYOUT_DISTRIBUTION_GRAVITY;

    var _name = InspectorUtils.createAttrName(name);
    attrLayout.addChild(_name);

    var edit = InspectorUtils.createEditField();

    attrLayout.addChild(edit);
    parent.addChild(attrLayout);

    return edit;

  }

  static createAttrCheckBox(name:string, parent:Clockwork.UIWidget):{ textField:Clockwork.UITextField, checkBox: Clockwork.UICheckBox} {

    var attrLayout = new Clockwork.UILayout();

    attrLayout.layoutSize = Clockwork.UI_LAYOUT_SIZE_AVAILABLE;
    attrLayout.gravity = Clockwork.UI_GRAVITY_LEFT_RIGHT;
    attrLayout.layoutDistribution = Clockwork.UI_LAYOUT_DISTRIBUTION_GRAVITY;

    var _name = InspectorUtils.createAttrName(name);
    attrLayout.addChild(_name);

    var checkBox = new Clockwork.UICheckBox();

    attrLayout.addChild(checkBox);
    parent.addChild(attrLayout);

    return {textField: _name, checkBox : checkBox};

  }


  static createAttrEditFieldWithSelectButton(name:string, parent:Clockwork.UIWidget):{editField:Clockwork.UIEditField, selectButton:Clockwork.UIButton} {

    var attrLayout = new Clockwork.UILayout();
    attrLayout.layoutDistribution = Clockwork.UI_LAYOUT_DISTRIBUTION_POSITION_LEFT_TOP;

    if (name) {
      var _name = InspectorUtils.createAttrName(name);
      attrLayout.addChild(_name);
    }

    var fieldLayout = new Clockwork.UILayout();
    fieldLayout.layoutDistribution = Clockwork.UI_LAYOUT_DISTRIBUTION_POSITION_LEFT_TOP;

    var edit = InspectorUtils.createEditField();

    var selectButton = new Clockwork.UIButton();
    selectButton.text = "...";
    selectButton.fontDescription = InspectorUtils.attrFontDesc;

    fieldLayout.addChild(edit);
    fieldLayout.addChild(selectButton);

    attrLayout.addChild(fieldLayout);
    parent.addChild(attrLayout);

    return {editField:edit, selectButton:selectButton};

  }


  // "static constructor"
  static attrFontDesc:Clockwork.UIFontDescription;

}

export = InspectorUtils;
