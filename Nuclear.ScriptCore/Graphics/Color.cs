using Nuclear.Math;
using System;
using System.Runtime.InteropServices;

namespace Nuclear.Graphics
{


    [StructLayout(LayoutKind.Sequential)]
    public struct Color
    {
        public float r;
        public float g;
        public float b;
        public float a;


        public Color(float value)
        {
            r = g = b = a = value;
        }
        public Color(int Red, int Green, int Blue, int Alpha)
        {
            r = Red / 255.0f;
            g = Green / 255.0f;
            b = Blue / 255.0f;
            a = Alpha / 255.0f;
        }
        public Color(float r_, float g_, float b_, float a_ = 1.0f)
        {
            r = r_;
            g = g_;
            b = b_;
            a = a_;
        }

        Math.Vector4 Get4f()
        {
            return new Math.Vector4(r, g, b, a);
        }

        public static Color operator +(Color left, Color right)
        {
            return new Color(left.r + right.r, left.g + right.g, left.b + right.b, left.a + right.a);
        }
        public static Color operator -(Color left, Color right)
        {
            return new Color(left.r - right.r, left.g - right.g, left.b - right.b, left.a - right.a);
        }
        public static Color operator *(Color left, Color right)
        {
            return new Color(left.r * right.r, left.g * right.g, left.b * right.b, left.a * right.a);
        }
        public static Color operator /(Color left, Color right)
        {
            return new Color(left.r / right.r, left.g / right.g, left.b / right.b, left.a / right.a);
        }

        //public static bool operator ==(Color left, Color right)
        //{
        //    return left.r == right.r && left.g == right.g && left.b == right.b && left.a == right.a;
        //}

        //public static bool operator !=(Color left, Color right)
        //{
        //    return left.r != right.r || left.g != right.g || left.b != right.b || left.a != right.a ? true : false;
        //}

    };
}
