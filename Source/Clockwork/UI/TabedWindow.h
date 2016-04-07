//
// Copyright (c) 2008-2015 the Clockwork project.
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

#pragma once

#include "Window.h"

namespace Clockwork
{
	EVENT(E_ACTIVETABCHANGED, ActiveTabChanged)
	{
		PARAM(P_TABINDEX, TabIndex);              // unsigned
	}

	class Window;
	class Button;
	class UIElement;

	/// %TabWindow %UI element. Allows for a window with multiple tabs
	class CLOCKWORK_API TabedWindow : public Window
	{
		OBJECT(TabedWindow)
	public:
		/// Construct.
		TabedWindow(Context* context);
		/// Destruct.
		~TabedWindow();
		/// Register object factory.
		static void RegisterObject(Context* context);
		/// Create a new tab
		unsigned AddTab(const String& name, UIElement* content);
		/// Remove a tab after its creation
		void RemoveTab(unsigned index);
		/// Remove a tab after its creation
		void RemoveTab(const String& name);
		/// Sets the current tab in use
		bool SetActiveTab(unsigned index);
		/// Sets the current tab in use
		bool SetActiveTab(const String& name);
	protected:

		void HandleButton(StringHash eventType, VariantMap& eventData);

		virtual void OnResize();

		SharedPtr<UIElement> buttonContainer_;
		SharedPtr<UIElement> contentContainer_;
		SharedPtr<UIElement> activeContent_;

		Vector<SharedPtr<UIElement> >	tabsContent_;
		Vector<SharedPtr<Button> >		tabsButtons_;
		HashMap<StringHash, unsigned>	nameTabMapping_;
	};

}