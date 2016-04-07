﻿using System;
using System.Runtime.InteropServices;

namespace ClockworkEngine
{
	internal static class ClockworkInterop
	{
		[UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
		delegate IntPtr CSComponentCreateDelegate (string name);

		static IntPtr CSComponentCreate(string name)
		{
			return ComponentCore.CreateCSComponent (name);
		}

		[DllImport (Constants.LIBNAME, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
		private static extern void csb_ClockworkEngine_ClockworkInterop_Set_CSComponentCreate(CSComponentCreateDelegate method);


		// ----

		[UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
		delegate void CSComponentCallMethodDelegate (uint componentID, CSComponentMethod method, float value);

		[DllImport (Constants.LIBNAME, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
		private static extern void csb_ClockworkEngine_ClockworkInterop_Set_CSComponentCallMethod(CSComponentCallMethodDelegate method);	

		static void CSComponentCallMethod(uint componentID, CSComponentMethod method, float value)
		{
			ComponentCore.CallComponentMethod (componentID, method, value);
		}

		// Events

		[UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
		delegate void CSBeginSendEventDelegate (uint senderRefId, uint eventType, IntPtr eventData);

		[DllImport (Constants.LIBNAME, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
		private static extern void csb_ClockworkEngine_ClockworkInterop_Set_CSBeginSendEvent(CSBeginSendEventDelegate method);	

		static void CSBeginSendEvent(uint senderRefId, uint eventType, IntPtr eventData)
		{
			EventCore.BeginSendEvent (senderRefId, eventType, eventData);
		}

		public static void Initialize()
		{
			csb_ClockworkEngine_ClockworkInterop_Set_CSComponentCreate (CSComponentCreate);	
			csb_ClockworkEngine_ClockworkInterop_Set_CSComponentCallMethod (CSComponentCallMethod);
			csb_ClockworkEngine_ClockworkInterop_Set_CSBeginSendEvent (CSBeginSendEvent);
		}

	}
}

