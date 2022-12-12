using Nuclear.ECS;
using System;
using System.Runtime.CompilerServices;


namespace Nuclear.Components
{
    public class LightComponent : Component
    {
        public Graphics.Color Color
        {
            get
            {
                GetColor_Native(Entity.ID, out Graphics.Color result);
                return result;
            }

            set
            {
                SetColor_Native(Entity.ID, ref value);
            }
        }

        public float Intensity
        {
            get
            {                
                return GetIntensity_Native(Entity.ID);
            }

            set
            {
                SetIntensity_Native(Entity.ID, ref value);
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void GetColor_Native(uint id, out Graphics.Color outcolor);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetColor_Native(uint id, ref Graphics.Color incolor);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float GetIntensity_Native(uint id);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetIntensity_Native(uint id, ref float infloat);
    }
}
