using System.Runtime.CompilerServices;

namespace Nuclear
{
    public class Entity
    {
        public uint mID { get; private set; }

        protected Entity() { mID = 0; }

        internal Entity(uint id)
        {
            mID = id;
        }

        ~Entity() { }
    }
}
