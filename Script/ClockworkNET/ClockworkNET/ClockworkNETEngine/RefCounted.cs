
using System;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Collections.Generic;

namespace ClockworkEngine
{

	public partial class RefCounted
	{

		public RefCounted ()
		{
		}

		protected RefCounted (IntPtr native)
		{
			nativeInstance = native;
		}

		public IntPtr nativeInstance = IntPtr.Zero;

		[DllImport (Constants.LIBNAME, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
		public static extern IntPtr csb_Clockwork_RefCounted_GetClassID (IntPtr self);

	}


}