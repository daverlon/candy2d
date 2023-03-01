#pragma once

class GameTime {
private:
    float dt;
    Uint64 prev;
public:
    GameTime() : dt(0.0f), prev(0) {}

    float DeltaTime() const { return dt; }
    float FPS() const { return dt * 60.0f * 60.0f; }
    void UpdateFirst(Uint64 t) { prev = t; } 
    void UpdateLast(Uint64 t)  { dt = float(t-prev) / 1000.0f; }
};