using Nuclear.Core;
using System;
using System.Runtime.CompilerServices;
using System.Security.Cryptography;

namespace Nuclear
{
    public class Entity
    {
        public uint ID { get; private set; }

        protected Entity() { ID = 0; }

        internal Entity(uint id)
        {
            ID = id;
        }

        ~Entity() { }

        public T AddComponent<T>() where T : Components.Component, new()
        {
            AddComponent_Native(ID, typeof(T));
            T component = new T();
            component.Entity = this;
            return component;
        }

        public bool HasComponent<T>() where T : Components.Component, new()
        {
            return HasComponent_Native(ID, typeof(T));
        }

        public T GetComponent<T>() where T : Components.Component, new()
        {
            if (HasComponent<T>())
            {
                T component = new T();
                component.Entity = this;
                return component;
            }
            return null;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void AddComponent_Native(uint id, Type type);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool HasComponent_Native(uint id, Type type);
    }
}
