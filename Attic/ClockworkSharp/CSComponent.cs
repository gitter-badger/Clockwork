using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace ClockworkEngine
{
	public delegate void ClockworkEventDelegate (VariantMap eventData);

	public partial class CSComponent : Component
	{
		public uint ManagedID;

		//public CSComponent ()
		//{
		//	nativeInstance = NativeCore.RegisterNative (csb_Clockwork_CSComponent_Constructor(), this);
//			ComponentCore.RegisterCSComponent (this);
	//	}

		virtual public void Start()
		{

		}

		virtual public void Update(float timeStep)
		{

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

		//[DllImport (Constants.LIBNAME, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
		//private static extern IntPtr csb_Clockwork_CSComponent_Constructor();

	}
}
