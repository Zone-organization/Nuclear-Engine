using System;
using System.Runtime.InteropServices;

namespace Nuclear.Math
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector4
    {
        public float x;
        public float y;
        public float z;
        public float w;

        public Vector4(float scalar)
        {
            x = y = z = w = scalar;
        }

        public Vector4(float _x, float _y, float _z, float _w)
        {
            x = _x;
            y = _y;
            z = _z;
            w = _w;
        }

        public Vector4(float _x, Vector3 yz)
        {
            x = _x;
            y = yz.x;
            z = yz.y;
            w = yz.z;
        }

        public Vector4(Vector3 vector)
        {
            x = vector.x;
            y = vector.y;
            z = vector.z;
            w = 1.0f;
        }

        public static Vector4 operator +(Vector4 left, Vector4 right)
        {
            return new Vector4(left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w);
        }

        public static Vector4 operator -(Vector4 left, Vector4 right)
        {
            return new Vector4(left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w);
        }

        public static Vector4 operator *(Vector4 left, Vector4 right)
        {
            return new Vector4(left.x * right.x, left.y * right.y, left.z * right.z, left.w * right.w);
        }

        public static Vector4 operator *(Vector4 left, float scalar)
        {
            return new Vector4(left.x * scalar, left.y * scalar, left.z * scalar, left.w * scalar);
        }

        public static Vector4 operator *(float scalar, Vector4 right)
        {
            return new Vector4(scalar * right.x, scalar * right.y, scalar * right.z, scalar * right.w);
        }

        public static Vector4 operator /(Vector4 left, Vector4 right)
        {
            return new Vector4(left.x / right.x, left.y / right.y, left.z / right.z, left.w / right.w);
        }

        public static Vector4 operator /(Vector4 left, float scalar)
        {
            return new Vector4(left.x / scalar, left.y / scalar, left.z / scalar, left.w / scalar);
        }
    }
}
