#include "Project.h"

namespace Nuclear::Editor
{
    Project::Project()
    {
    }
    ECS::Scene* Project::GetActiveScene()
    {
        return &MainScene;
    }
    //ECS::Scene* Project::AddNewScene()
    //{
    //  //  mScenes.push_back(ECS::Scene());
    //   // return &mScenes.back();
    //    return nullptr;

    //}
}