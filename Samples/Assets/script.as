
class Script1
{
    Script1()
    {
        print("Constructor\n");
    }

    void Load()
    {
        print("Start\n");
    }

    void Update(float dt)
    {
        print("Update\n" + dt);
    }
};