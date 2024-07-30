using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace WinFormsSandbox.Source
{
    internal class RectangleRenderer
    {
        #region DLL Imports
        [DllImport("EngineFinalDLL.dll")]
        static private extern int GetWidth(IntPtr rendererPtr);
        [DllImport("EngineFinalDLL.dll")]
        static private extern int GetHeight(IntPtr rendererPtr);
        [DllImport("EngineFinalDLL.dll")]
        static private extern int GetRed(IntPtr rendererPtr);
        [DllImport("EngineFinalDLL.dll")]
        static private extern int GetGreen(IntPtr rendererPtr);
        [DllImport("EngineFinalDLL.dll")]
        static private extern int GetBlue(IntPtr rendererPtr);

        [DllImport("EngineFinalDLL.dll")]
        static private extern void SetWidth(IntPtr rendererPtr, int newWidth);
        [DllImport("EngineFinalDLL.dll")]
        static private extern void SetHeight(IntPtr rendererPtr, int newHeight);
        [DllImport("EngineFinalDLL.dll")]
        static private extern void SetRed(IntPtr rendererPtr, int newRed);
        [DllImport("EngineFinalDLL.dll")]
        static private extern void SetGreen(IntPtr rendererPtr, int newGreen);
        [DllImport("EngineFinalDLL.dll")]
        static private extern void SetBlue(IntPtr rendererPtr, int newBlue);

        #endregion
        public RectangleRenderer(IntPtr ptr)
        {
            RendererPtr = ptr;
        }

        public int GetWidth()
        {
            return GetWidth(RendererPtr);
        }

        public int GetHeight()
        {
            return GetHeight(RendererPtr);
        }

        public Color GetColor()
        {
            int r = GetRed(RendererPtr);
            int g = GetGreen(RendererPtr);
            int b = GetBlue(RendererPtr);

            return Color.FromArgb(255, r, g, b);
        }
        
        public void SetWidth(int newWidth)
        {
            SetWidth(RendererPtr, newWidth);
        }

        public void SetHeight(int newHeight)
        {
            SetHeight(RendererPtr, newHeight);
        }

        public void SetColor(Color newColor)
        {
            SetRed(RendererPtr, newColor.R);
            SetGreen(RendererPtr, newColor.G);
            SetBlue(RendererPtr, newColor.B);
        }

        private IntPtr RendererPtr = IntPtr.Zero;

        #region Property Grid Display Info
        [Category("Renderer")]
        [DisplayName("Width")]
        [Description("How wide the displayed rectangle will be.")]
        public int DisplayWidth
        {
            get { return GetWidth(); }
            set { SetWidth(value); }
        }

        [Category("Renderer")]
        [DisplayName("Height")]
        [Description("How tall the displayed rectangle will be.")]
        public int DisplayHeight
        {
            get { return GetHeight(); }
            set { SetHeight(value); }
        }

        [Category("Renderer")]
        [DisplayName("Color")]
        [Description("The color of the rectangle renderer.")]
        public Color DisplayColor
        {
            get { return GetColor(); }
            set { SetColor(value); }
        }
        #endregion
    }
}
