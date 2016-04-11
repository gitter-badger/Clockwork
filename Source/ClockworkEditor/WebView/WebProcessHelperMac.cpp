// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that can
// be found in the LICENSE file.

#ifdef CLOCKWORK_PLATFORM_OSX

#include <ClockworkWebView/ClockworkWebView.h>

// Entry point function for sub-processes.
int main(int argc, char* argv[])
{
    return Clockwork::WebMain(argc, argv);
}

#endif
