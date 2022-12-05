using System.Runtime.CompilerServices;

namespace Nuclear
{
    public class ScriptCore
    {
       

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void HelloWorld_Native();
    }
}
