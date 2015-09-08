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

#include <Clockwork/Engine/Application.h>
#include <Clockwork/UI/UI.h>
#include <Clockwork/UI/UIElement.h>
#include <Clockwork/UI/Window.h>

using namespace Clockwork;

class ClockworkLauncher : public Application
{
	OBJECT(ClockworkLauncher);
public:
	ClockworkLauncher(Context* context);

	/// Launcher Setup, runs before the engines initilization
	void Setup();

	/// Starts the launcher, runs after initilization
	void Start();

	/// Stops the launcher
	void Stop();
private:
	void SetIconAndTitle();
	/// Create and initialize a Window control.
	void InitWindow();
	/// Create and add various common controls for demonstration purposes.
	void InitControls();
	/// Create a list of projects.
	void InitProjectList();

	/// The Window.
	SharedPtr<Window> window_;
	/// The UI's root UIElement.
	SharedPtr<UIElement> uiRoot_;
	/// Remembered drag begin position.
	IntVector2 dragBeginPosition_;

	/// Handles initial input for the launcher
	void HandleKeyDown(StringHash eventType, VariantMap& eventData);
};