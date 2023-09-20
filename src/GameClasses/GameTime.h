#pragma once

class GameTime {
private:
    float dt;
    Uint64 prev;
    Uint64 frameCount;
    float fps;
    Uint64 fpsUpdateTime;

public:
    GameTime() : dt(0.0f), prev(0), frameCount(0), fps(0.0f), fpsUpdateTime(0) {}

    float DeltaTime() const { return dt; }
    float FPS() const { return fps; }

    void UpdateFirst(Uint64 t) { 
        prev = t; 
    }

    void UpdateLast(Uint64 t) {
        dt = float(t - prev) / 1000.0f;
        frameCount++;
        
        // Update FPS every second
        if (t - fpsUpdateTime >= 1000) {
            fps = float(frameCount) / ((t - fpsUpdateTime) / 1000.0f);
            frameCount = 0;
            fpsUpdateTime = t;
        }
    }
};