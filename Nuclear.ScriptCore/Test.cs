using System.Runtime.CompilerServices;

namespace Nuclear
{
    public class Test
    {
        public void HelloWorld()
        {
            HelloWorld_Native();
        }

       
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void HelloWorld_Native();
    }
}
