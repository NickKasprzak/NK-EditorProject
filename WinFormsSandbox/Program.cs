using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace WinFormsSandbox
{
    internal static class Program
    {
        /*
         * Whats tripping me up a little is how each of the
         * extern'd functions are static. My first thought is
         * to think "oh fuck, that means that this function can
         * be called at anytime from anywhere. that means i cant
         * have these functions apply to a specific object instance!"
         * 
         * But that isn't true is it? Because functions that need
         * to step into a given instance of an object require a
         * reference to said class instance in order to do so,
         * it still allows for multiple instances of a given
         * object to exist at once. The extern'd functions would
         * still exist out in the open, but classes wrapping
         * IntPtr objects can still be applied on an instance to
         * instance basis by wrapping these global functions into
         * class specific functions.
         * 
         * For example, if we had a container class that holds
         * an int and it has an add function exported to C# from
         * C++, a C# class could exist that contains functions
         * for generating a new instance of the class, containing
         * a reference to said class through an IntPtr, and
         * a wrapped version of the add function, taking in an
         * argument for an integer to add and then adding that
         * value to the class instance stored in its IntPtr by
         * passing in both as arguments to the static extern
         * function.
         * 
         * Its sorta like how SDL's render functions take in
         * the renderer instance as an argument rather than
         * these functions being native to the renderer itself.
         * 
         * What would be the best approach to organizing all
         * this though? Should I have a seperate header file
         * that just imports all the neccessary functions from
         * the DLL and then give that header to the wrapped
         * object class' header?
         */

        [DllImport("WrapperDLL.dll")]
        public static extern IntPtr exp_CreateTestClass(int x);
        [DllImport("WrapperDLL.dll")]
        public static extern void exp_Add(IntPtr classRef, int x);
        [DllImport("WrapperDLL.dll")]
        public static extern int exp_GetValue(IntPtr classRef);


        [DllImport("WrapperDLL.dll")]
        public static extern IntPtr exp_CreateTestClassWrapper(int x);
        [DllImport("WrapperDLL.dll")]
        public static extern void exp_AddTen(IntPtr classRef);

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }
    }
}
