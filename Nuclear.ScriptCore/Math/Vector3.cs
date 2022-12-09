using System;
using System.Runtime.InteropServices;

namespace Nuclear.Math
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector3
    {
        public float x;
        public float y;
        public float z;

        public Vector3(float scalar)
        {
            x = y = z = scalar;
        }

        public Vector3(float _x, Vector2 yz)
        {
            x = _x;
            y = yz.x;
            z = yz.y;
        }

        public Vector3(float _x, float _y, float _z)
        {
            x = _x;
            y = _y;
            z = _z;
        }

        public Vector3(Vector2 vector)
        {
            x = vector.x;
            y = vector.y;
            z = 0.0f;
        }

        public Vector3(Vector4 vector)
        {
            x = vector.x;
            y = vector.y;
            z = vector.z;
        }

        public Vector2 xy
        {
            get { return new Vector2(x, y); }
            set { x = value.x; y = value.y; }
        }
        public Vector2 xz
        {
            get { return new Vector2(x, z); }
            set { x = value.x; z = value.y; }
        }
        public Vector2 yz
        {
            get { return new Vector2(y, z); }
            set { y = value.x; z = value.y; }
        }


        public static Vector3 operator *(Vector3 left, float scalar)
        {
            return new Vector3(left.x * scalar, left.y * scalar, left.z * scalar);
        }

        public static Vector3 operator *(float scalar, Vector3 right)
        {
            return new Vector3(scalar * right.x, scalar * right.y, scalar * right.z);
        }

        public static Vector3 operator +(Vector3 left, Vector3 right)
        {
            return new Vector3(left.x + right.x, left.y + right.y, left.z + right.z);
        }

        public static Vector3 operator +(Vector3 left, float right)
        {
            return new Vector3(left.x + right, left.y + right, left.z + right);
        }

        public static Vector3 operator -(Vector3 left, Vector3 right)
        {
            return new Vector3(left.x - right.x, left.y - right.y, left.z - right.z);
        }

        public static Vector3 operator /(Vector3 left, Vector3 right)
        {
            return new Vector3(left.x / right.x, left.y / right.y, left.z / right.z);
        }

        public static Vector3 operator /(Vector3 left, float scalar)
        {
            return new Vector3(left.x / scalar, left.y / scalar, left.z / scalar);
        }

        public static Vector3 operator -(Vector3 vector)
        {
            return new Vector3(-vector.x, -vector.y, -vector.z);
        }


        public static Vector3 Zero = new Vector3(0, 0, 0);
        public static Vector3 Forward = new Vector3(0, 0, -1);
        public static Vector3 Right = new Vector3(1, 0, 0);
        public static Vector3 Up = new Vector3(0, 1, 0);
    }
}
