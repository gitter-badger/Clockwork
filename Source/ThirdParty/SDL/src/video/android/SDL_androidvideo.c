/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2014 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

// Modified by Yao Wei Tjong for Clockwork

#include "../../SDL_internal.h"

#if SDL_VIDEO_DRIVER_ANDROID

/* Android SDL video driver implementation
*/

#include "SDL_video.h"
#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "../../events/SDL_events_c.h"
#include "../../events/SDL_windowevents_c.h"

#include "SDL_androidvideo.h"
#include "SDL_androidclipboard.h"
#include "SDL_androidevents.h"
#include "SDL_androidkeyboard.h"
#include "SDL_androidtouch.h"
#include "SDL_androidwindow.h"

#define ANDROID_VID_DRIVER_NAME "Android"

/* Initialization/Query functions */
static int Android_VideoInit(_THIS);
static void Android_VideoQuit(_THIS);

#include "../SDL_egl_c.h"
/* GL functions (SDL_androidgl.c) */
extern SDL_GLContext Android_GLES_CreateContext(_THIS, SDL_Window * window);
extern int Android_GLES_MakeCurrent(_THIS, SDL_Window * window, SDL_GLContext context);
extern void Android_GLES_SwapWindow(_THIS, SDL_Window * window);
extern int Android_GLES_LoadLibrary(_THIS, const char *path);
#define Android_GLES_GetProcAddress SDL_EGL_GetProcAddress
#define Android_GLES_UnloadLibrary SDL_EGL_UnloadLibrary
#define Android_GLES_SetSwapInterval SDL_EGL_SetSwapInterval
#define Android_GLES_GetSwapInterval SDL_EGL_GetSwapInterval
#define Android_GLES_DeleteContext SDL_EGL_DeleteContext

/* Android driver bootstrap functions */


/* These are filled in with real values in Android_SetScreenResolution on init (before SDL_main()) */
int Android_ScreenWidth = 0;
int Android_ScreenHeight = 0;
Uint32 Android_ScreenFormat = SDL_PIXELFORMAT_UNKNOWN;
SDL_sem *Android_PauseSem = NULL, *Android_ResumeSem = NULL;

/* Currently only one window */
SDL_Window *Android_Window = NULL;

static int
Android_Available(void)
{
    return 1;
}

static void
Android_DeleteDevice(SDL_VideoDevice * device)
{
	// Clockwork: bug fix
	SDL_free(device->driverdata);
    SDL_free(device);
}

static SDL_VideoDevice *
Android_CreateDevice(int devindex)
{
    SDL_VideoDevice *device;
    SDL_VideoData *data;

    /* Initialize all variables that we clean on shutdown */
    device = (SDL_VideoDevice *) SDL_calloc(1, sizeof(SDL_VideoDevice));
    if (!device) {
        SDL_OutOfMemory();
        return NULL;
    }

    data = (SDL_VideoData*) SDL_calloc(1, sizeof(SDL_VideoData));
    if (!data) {
        SDL_OutOfMemory();
        SDL_free(device);
        return NULL;
    }

    device->driverdata = data;

    /* Set the function pointers */
    device->VideoInit = Android_VideoInit;
    device->VideoQuit = Android_VideoQuit;
    device->PumpEvents = Android_PumpEvents;

    device->CreateWindow = Android_CreateWindow;
    device->SetWindowTitle = Android_SetWindowTitle;
    device->DestroyWindow = Android_DestroyWindow;

    device->free = Android_DeleteDevice;

    /* GL pointers */
    device->GL_LoadLibrary = Android_GLES_LoadLibrary;
    device->GL_GetProcAddress = Android_GLES_GetProcAddress;
    device->GL_UnloadLibrary = Android_GLES_UnloadLibrary;
    device->GL_CreateContext = Android_GLES_CreateContext;
    device->GL_MakeCurrent = Android_GLES_MakeCurrent;
    device->GL_SetSwapInterval = Android_GLES_SetSwapInterval;
    device->GL_GetSwapInterval = Android_GLES_GetSwapInterval;
    device->GL_SwapWindow = Android_GLES_SwapWindow;
    device->GL_DeleteContext = Android_GLES_DeleteContext;

    /* Text input */
    device->StartTextInput = Android_StartTextInput;
    device->StopTextInput = Android_StopTextInput;
    device->SetTextInputRect = Android_SetTextInputRect;

    /* Screen keyboard */
    device->HasScreenKeyboardSupport = Android_HasScreenKeyboardSupport;
    device->IsScreenKeyboardShown = Android_IsScreenKeyboardShown;

    /* Clipboard */
    device->SetClipboardText = Android_SetClipboardText;
    device->GetClipboardText = Android_GetClipboardText;
    device->HasClipboardText = Android_HasClipboardText;

    return device;
}

VideoBootStrap Android_bootstrap = {
    ANDROID_VID_DRIVER_NAME, "SDL Android video driver",
    Android_Available, Android_CreateDevice
};


int
Android_VideoInit(_THIS)
{
    SDL_DisplayMode mode;

    mode.format = Android_ScreenFormat;
    mode.w = Android_ScreenWidth;
    mode.h = Android_ScreenHeight;
    mode.refresh_rate = 0;
    mode.driverdata = NULL;

    // Clockwork: merge patch found in https://bugzilla.libsdl.org/show_bug.cgi?id=2291 submitted by Thomas Faller
    SDL_PixelFormat pixelFormat;
    Uint32 mask;
    int bitCount;

    /* We need to set color sizes */
    if(!SDL_InitFormat(&pixelFormat, mode.format)){
        for(mask = pixelFormat.Rmask >> pixelFormat.Rshift,
            bitCount = 0; mask > 0; mask >>= 1)
            bitCount += 1;
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, bitCount);
        for(mask = pixelFormat.Gmask >> pixelFormat.Gshift,
            bitCount = 0; mask > 0; mask >>= 1)
            bitCount += 1;
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, bitCount);
        for(mask = pixelFormat.Bmask >> pixelFormat.Bshift,
            bitCount = 0; mask > 0; mask >>= 1)
            bitCount += 1;
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, bitCount);
        for(mask = pixelFormat.Amask >> pixelFormat.Ashift,
            bitCount = 0; mask > 0; mask >>= 1)
            bitCount += 1;
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, bitCount);
    }

    if (SDL_AddBasicVideoDisplay(&mode) < 0) {
        return -1;
    }

    SDL_AddDisplayMode(&_this->displays[0], &mode);

    Android_InitKeyboard();

    Android_InitTouch();

    /* We're done! */
    return 0;
}

void
Android_VideoQuit(_THIS)
{
}

/* This function gets called before VideoInit() */
void
Android_SetScreenResolution(int width, int height, Uint32 format)
{
    Android_ScreenWidth = width;
    Android_ScreenHeight = height;
    Android_ScreenFormat = format;

    if (Android_Window) {
        SDL_SendWindowEvent(Android_Window, SDL_WINDOWEVENT_RESIZED, width, height);
    }
}

#endif /* SDL_VIDEO_DRIVER_ANDROID */

/* vi: set ts=4 sw=4 expandtab: */