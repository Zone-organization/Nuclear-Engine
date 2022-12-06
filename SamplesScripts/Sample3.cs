using System;
using Nuclear;
using Nuclear.Core;

namespace Samples
{
    public class Sample3 : Entity
    {
        public void OnStart()
        {
            Logger.Info("Script Start id " + mID);
        }

        public void OnUpdate(float td)
        {
            Logger.Info("Update Time Delta "+ td);
        }
    }
}
