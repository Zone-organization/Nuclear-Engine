using System;
using System.Runtime.CompilerServices;

namespace Nuclear.Components
{

    public class EntityInfoComponent : Component
    {
        //TODO
        //Entity ParentEntity;
        //ECS.Transform Transform;


        public string Name
        {
            get => GetName_Native(Entity.ID);
            set => SetName_Native(Entity.ID, value);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern string GetName_Native(uint entity);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void SetName_Native(uint entity, string name);

    }

}
