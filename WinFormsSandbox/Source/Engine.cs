using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WinFormsSandbox.Source
{
    internal class Engine
    {
        #region DLL Imports
        // Main Engine Imports
        [DllImport("EngineFinalDLL.dll")]
        private static extern IntPtr CreateEngine();
        [DllImport("EngineFinalDLL.dll")]
        private static extern void EngineGameLoop(IntPtr engine);
        [DllImport("EngineFinalDLL.dll")]
        private static extern IntPtr GetWindowHandle(IntPtr engine);
        [DllImport("EngineFinalDll.dll")]
        private static extern IntPtr SaveLevel(IntPtr engine, string path);
        [DllImport("EngineFinalDLL.dll")]
        private static extern IntPtr LoadLevel(IntPtr engine, string path);
        [DllImport("EngineFinalDLL.dll")]
        private static extern IntPtr GetGameObjectByIndex(IntPtr engine, int index);

        // Win32 entry point function imports
        // Credit to https://gist.github.com/flibitijibibo/cf282bfccc1eaeb47550 for the introduction to them
        [DllImport("user32.dll")]
        private static extern IntPtr SetWindowPos(IntPtr handle, IntPtr handleAfter, int x, int y, int cx, int cy, uint flags);
        [DllImport("user32.dll")]
        private static extern IntPtr SetParent(IntPtr child, IntPtr newParent);
        [DllImport("user32.dll")]
        private static extern IntPtr ShowWindow(IntPtr handle, int command);
        #endregion

        public Engine()
        {
            EnginePtr = CreateEngine();
        }

        public void RunLoop()
        {
            EngineGameLoop(EnginePtr);
        }

        public void SaveLevel(string levelPath)
        {
            SaveLevel(EnginePtr, levelPath);
        }

        public void LoadLevel(string levelPath)
        {
            // Create all relevant level objects
            LoadLevel(EnginePtr, levelPath);
        }

        // Returns a list of active game objects in a format
        // that the C# code can utilize
        public List<GameObject> GetGameObjectList()
        {
            List<GameObject> objList = new List<GameObject>();

            // Loop through all game objects to create C# references to each
            // using the pointer to the C++ object
            for (int i = 0; i < 50; i++)
            {
                IntPtr obj = GetGameObjectByIndex(EnginePtr, i);

                if (obj != IntPtr.Zero)
                {
                    objList.Add(new GameObject(obj));
                }
            }

            return objList;
        }

        public void AttachToPanel(Panel panel)
        {
            IntPtr winHandle = GetWindowHandle(EnginePtr);
            SetWindowPos(winHandle, panel.Parent.Handle, 0, 0, 0, 0, 0x0401);
            SetParent(winHandle, panel.Handle);
            ShowWindow(winHandle, 1);
        }

        public static IntPtr GetEnginePtr()
        {
            return EnginePtr;
        }

        private static IntPtr EnginePtr;
    }
}
