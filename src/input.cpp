#include "input.hpp"

#include <windows.h>

namespace rwpm {

void InputEvent::updateState() {
    lastState = currentState;
    currentState = (GetAsyncKeyState(virtualKey) & 0x8000) > 0;

    if (currentState && !lastState && callback) {
        callback(context);
    }
}

InputEvent* InputHandler::addEvent(const int virtualKey, InputCallbackFunction callback, void* context) {
    mEvents.emplace_back(virtualKey, callback, context);
    return &mEvents.back();
}

void InputHandler::reset() {
    mEvents.clear();
}

void InputHandler::updateEvents() {
    for (auto& event : mEvents) {
        event.updateState();
    }
}

}