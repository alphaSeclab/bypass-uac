using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;

namespace CLSID
{
    public class Class1
    {
        static Class1()
        {
            RegistryKey Key = Registry.CurrentUser.OpenSubKey("Software\\MyExe", false);
            string CustomParam = Key.GetValue("").ToString();
            Key.Close();
            Process.Start(CustomParam);
            Environment.Exit(0);
        }
    }
}
