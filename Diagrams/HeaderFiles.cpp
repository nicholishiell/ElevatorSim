class FloorPanel
{
    Public:
        FloorPanel();

        void SetUpLight(LightState s);

        void SendFloorRequest(int level, Direction dir);

    private:

        DirectionButton up_;
        DirectionButton down_;

};

class Floor
{
    public:
        Floor(int level, string label)
    private:

        FloorPanel panel_;
        std::list<Passengers> passengers_;
        int level_;
        std::string label_;
};

class Elevator
{
    public:
        Floor(int level, string label)
    private:

        FloorPanel panel_;
        std::list<Passengers> passengers_;
        int level_;
        std::string label_;
};

class ECS
{
    public:
        ECS();

    private:

        std::list<Elevator> elevators_;
        std::list<Floor> floors_;
        
};