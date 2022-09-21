#pragma once

class Time {
    private:
        float dt;
        int prev;
    public:
        float DeltaTime()       { return dt; }
        float FPS()             { return dt*60.0f*60.0f; }
        void UpdateFirst(int t) { prev = t; } 
        void UpdateLast(int t)  { dt = float(t-prev) / 1000.0f; }
};

namespace g {
    extern Time time;
}