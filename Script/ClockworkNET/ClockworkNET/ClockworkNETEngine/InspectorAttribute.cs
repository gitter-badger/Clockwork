using System;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Collections.Generic;

namespace ClockworkEngine
{


	public class InspectorAttribute : Attribute
	{
		public InspectorAttribute(string DefaultValue = "")
		{
		}

		public string DefaultValue;
	}


}