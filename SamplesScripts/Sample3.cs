using System;
using Nuclear;
using Nuclear.Core;

namespace Samples
{
    public class Sample3 : ScriptCore
    {
        public void OnStart()
        {
            Logger.Info("Script Start");
        }

        public void OnUpdate(float td)
        {
            Logger.Info("Update Time Delta "+ td);
        }
    }
}
