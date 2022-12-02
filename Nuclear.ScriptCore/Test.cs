using System.Runtime.CompilerServices;

namespace Nuclear
{
    public class Test
    {
        public static void HelloWorld(object message)
        {
            HelloWorld_Native(message);
        }

       
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void HelloWorld_Native(object message);
    }
}
