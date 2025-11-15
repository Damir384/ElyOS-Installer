#pragma once

enum class StepResult {
    Next,
    Prev,
    Exit
};

class Step {
public:
    virtual ~Step() = default;
    virtual StepResult run() = 0;
};
