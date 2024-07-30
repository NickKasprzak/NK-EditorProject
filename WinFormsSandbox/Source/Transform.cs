using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace WinFormsSandbox.Source
{
    // Literally all this is is just a way of interpreting
    // a given pointer as if it were a transform. So sorta
    // like translating a C++ component into something that
    // the C# code can interact with

    // This will explode if you give it a pointer that doesn't
    // actually correspond to a transform
    internal class Transform
    {
        #region DLL Imports
        [DllImport("EngineFinalDLL.dll")]
        private static extern int GetX(IntPtr transformPtr);
        [DllImport("EngineFinalDLL.dll")]
        private static extern int GetY(IntPtr transformPtr);
        [DllImport("EngineFinalDLL.dll")]
        private static extern int GetZ(IntPtr transformPtr);

        [DllImport("EngineFinalDLL.dll")]
        private static extern void SetX (IntPtr transformPtr, int newX);
        [DllImport("EngineFinalDLL.dll")]
        private static extern void SetY(IntPtr transformPtr, int newY);
        [DllImport("EngineFinalDLL.dll")]
        private static extern void SetZ(IntPtr transformPtr, int newZ);
        #endregion

        // Does this really need access to the parent game object?
        // We can just handle all operations on it from the C#
        // level through the GameObject right?

        // Having parent/child communication would be nice at
        // a higher level at some point, but the way things are
        // structured on the lower level now makes that a bit of
        // a headache to manage.

        // I think I'm overthinking this all though. I don't
        // need to store a reference to the parent game object
        // in here because I can just grab a pointer to the
        // transform pointer's parent object and then just
        // turn that into a GameObject C# class. Idiot!!!!

        // That makes me wonder though. Do I really need a
        // direct reference to any game object's component
        // if I can just grab a pointer to it from the C++
        // class then convert it into a readable C# class
        // whenever I want to?

        // What would I do if I wanted to have a Component
        // attached to a Component though? Would I have to
        // have a name or object id passed between the C++
        // and C# side of things so the C# code can know how
        // to properly cast the pointer it recieves to its
        // corresponding type?
        public Transform(IntPtr ptr)
        {
            TransformPtr = ptr;
        }

        public int GetX()
        {
            return GetX(TransformPtr);
        }

        public int GetY()
        {
            return GetY(TransformPtr);
        }

        public int GetZ()
        {
            return GetZ(TransformPtr);
        }

        public void SetX(int x)
        {
            SetX(TransformPtr, x);
        }

        public void SetY(int y)
        {
            SetY(TransformPtr, y);
        }

        public void SetZ(int z)
        {
            SetZ(TransformPtr, z);
        }

        private IntPtr TransformPtr = IntPtr.Zero;

        #region Property Grid Display Info
        [Category("Transform")]
        [DisplayName("X Position")]
        [Description("The position on the X axis.")]
        public int DisplayX
        {
            get { return GetX(); }
            set { SetX(value); }
        }

        [Category("Transform")]
        [DisplayName("Y Position")]
        [Description("The position on the Y axis.")]
        public int DisplayY
        {
            get { return GetY(); }
            set { SetY(value); }
        }

        [Category("Transform")]
        [DisplayName("Z Position")]
        [Description("The position on the Z axis.")]
        public int DisplayZ
        {
            get { return GetZ(); }
            set { SetZ(value); }
        }
        #endregion
    }
}
