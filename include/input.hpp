#pragma once

#include <vector>

namespace rwpm {

typedef void (*InputCallbackFunction)(void* context);

struct InputEvent {
    int virtualKey;
    bool currentState;
    bool lastState;
    
    InputCallbackFunction callback;
    void* context;

    InputEvent(const int key, const InputCallbackFunction cb, void* ctx = nullptr)
        : virtualKey(key), currentState(false), lastState(false), callback(cb), context(ctx) {}
    void updateState();
};

class InputHandler {
public:
    InputHandler() = default;

    InputEvent* addEvent(const int virtualKey, InputCallbackFunction callback, void* context = nullptr);
    void reset();
    
    void updateEvents();
private:
    std::vector<InputEvent> mEvents;
};

}