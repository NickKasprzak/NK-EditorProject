using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace WinFormsSandbox.Source
{
    internal class GameObject
    {
        #region DLL Imports
        [DllImport("EngineFinalDLL.dll")]
        private static extern IntPtr CreateGameObject(IntPtr engine);
        [DllImport("EngineFinalDLL.dll")]
        private static extern IntPtr DestroyGameObject(IntPtr engine, IntPtr obj);

        [DllImport("EngineFinalDLL.dll")]
        private static extern IntPtr CreateTransform(IntPtr obj);
        [DllImport("EngineFinalDLL.dll")]
        private static extern IntPtr GetTransform(IntPtr obj);
        [DllImport("EngineFinalDLL.dll")]
        private static extern void DeleteTransform(IntPtr obj);

        [DllImport("EngineFinalDLL.dll")]
        private static extern IntPtr CreateRenderer(IntPtr obj);
        [DllImport("EngineFinalDLL.dll")]
        private static extern IntPtr GetRenderer(IntPtr obj);
        [DllImport("EngineFinalDLL.dll")]
        private static extern void DeleteRenderer(IntPtr obj);

        #endregion

        // Creates a new game object entirely (ie. Doesn't exist in the C++ side)
        // Doesn't load any components to the class because a new GameObject is empty
        public GameObject()
        {
            if (Engine.GetEnginePtr() != IntPtr.Zero)
            {
                GameObjectPtr = CreateGameObject(Engine.GetEnginePtr());
            }
        }

        // Creates a new game object from an existing object reference (ie. Something already loaded in the C++ engine)
        public GameObject(IntPtr objPtr)
        {
            if (objPtr != IntPtr.Zero)
            {
                GameObjectPtr = objPtr;
            }
        }

        // Deletes the GameObject instance stored in the C++ engine
        //
        // Exists outside of the destructor due to the C# wrapper
        // of these classes being temporary ways for the editor
        // to interact with the the C++ versions of these classes.
        //
        // Destroying the C++ classes from the C# wrapper's destructor
        // would delete all game objects immediately upon creation.
        public void DestroyGameObject()
        {
            DestroyGameObject(Engine.GetEnginePtr(), GameObjectPtr);
        }

        #region Transform Functions
        // Adds a transform on the C++ level if one doesn't exist already
        public void AddTransform()
        {
            CreateTransform(GameObjectPtr);
        }

        // Returns the attached transform in its readable C# format
        // Returns null if there isn't a transform
        public Transform GetTransform()
        {
            IntPtr transformPtr = GetTransform(GameObjectPtr);

            if (transformPtr != IntPtr.Zero)
            {
                return new Transform(transformPtr);
            }

            return null;
        }

        // Calls the C++ game object's remove transform function
        // to delete the transform attached to the object
        public void RemoveTransform()
        {
            DeleteTransform(GameObjectPtr);
        }
        #endregion

        #region Rectangle Renderer Functions

        public void AddRenderer()
        {
            CreateRenderer(GameObjectPtr);
        }

        public RectangleRenderer GetRenderer()
        {
            IntPtr rendererPtr = GetRenderer(GameObjectPtr);

            if (rendererPtr != IntPtr.Zero)
            {
                return new RectangleRenderer(rendererPtr);
            }

            return null;
        }

        public void RemoveRenderer()
        {
            DeleteRenderer(GameObjectPtr);
        }

        #endregion

        private IntPtr GameObjectPtr = IntPtr.Zero;
    }
}
