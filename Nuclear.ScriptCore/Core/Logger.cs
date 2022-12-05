using System.Runtime.CompilerServices;

namespace Nuclear.Core
{
    public class Logger
    {
        public static void Trace(object message)
        {
            LoggerTrace_Native(message);
        }

        public static void Info(object message)
        {
            LoggerInfo_Native(message);
        }

        public static void Warn(object message)
        {
            LoggerWarn_Native(message);
        }     

        public static void Error(object message)
        {
            LoggerError_Native(message);
        }

        public static void Fatal(object message)
        {
            LoggerFatal_Native(message);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void LoggerInfo_Native(object message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void LoggerWarn_Native(object message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void LoggerTrace_Native(object message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void LoggerError_Native(object message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void LoggerFatal_Native(object message);
    }
}
