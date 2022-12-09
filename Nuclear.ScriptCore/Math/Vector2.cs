using System;
using System.Runtime.InteropServices;

namespace Nuclear.Math
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector2
    {
        public float x;
        public float y;

        public Vector2(float scalar)
        {
            x = y = scalar;
        }

        public Vector2(float _x, float _y)
        {
            x = _x;
            y = _y;
        }

        public Vector2(Vector3 vector)
        {
            x = vector.x;
            y = vector.y;
        }

        public static Vector2 operator -(Vector2 left, Vector2 right)
        {
            return new Vector2(left.x - right.x, left.y - right.y);
        }

        public static Vector2 operator +(Vector2 left, Vector2 right)
        {
            return new Vector2(left.x + right.x, left.y + right.y);
        }

        public static Vector2 operator /(Vector2 left, double value)
        {
            return new Vector2(left.x / (float)value, left.y / (float)value);
        }

        public static Vector2 operator -(Vector2 vector)
        {
            return new Vector2(-vector.x, -vector.y);
        }
    }
}
