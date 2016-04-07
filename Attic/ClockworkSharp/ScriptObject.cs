﻿using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace ClockworkEngine
{
	public class ScriptObject : AObject
	{
		public ScriptObject ()
		{
			nativeInstance = NativeCore.RegisterNative (csb_Clockwork_CSScriptObject_Constructor(), this);	
		}

		public void SendEvent(string eventType, Dictionary<string, object> eventData = null)
		{
			EventCore.SendEvent (this, eventType);
		}

		public void SubscribeToEvent(AObject sender, string eventType, ClockworkEventDelegate function)
		{
			EventCore.SubscribeToEvent (this, sender, eventType, function);
		}

		public void SubscribeToEvent(string eventType, ClockworkEventDelegate function)
		{
			EventCore.SubscribeToEvent (this, null, eventType, function);
		}

		[DllImport (Constants.LIBNAME, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
		private static extern IntPtr csb_Clockwork_CSScriptObject_Constructor();
	}
}

