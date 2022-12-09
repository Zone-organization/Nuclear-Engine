using System;

namespace Nuclear.Components
{
    public abstract class Component
    {
        public ECS.Entity Entity { get; set; }
    }
}
