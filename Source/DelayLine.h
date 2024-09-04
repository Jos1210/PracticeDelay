#pragma once
#include <memory>

class DelayLine
{
public:
    void setMaximumDelayInSamples(int maxLengthInSamples);
    void reset() noexcept;
    
    void write(float input) noexcept;
    float read (float delayInSamples) const noexcept;
    
    int getBufferLength() const noexcept{
        return bufferLength;
    }
private:
    std::unique_ptr<float[]> buffer; //Unique pointers take care of handling memory automatically instead of having to delete them manually
    int bufferLength = 0;
    int writeIndex = 0; //Donde se escribe el valor mas reciente
};

