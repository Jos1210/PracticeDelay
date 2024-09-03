#include "Tempo.h"

static std::array <double, 16> noteLengthMultipliers = {
    0.125,       // 1/32
    0.5/3.0,     // 1/16 trip
    0.1875,      // 1/32 dot
    0.25,        // 1/16
    1.0/3.0,     // 1/8 trip
    0.375,       // 1/16 dot
    0.5,         // 1/8
    2.0/3.0,     // 1/4 trip
    0.75,        // 1/8 ot
    1.0,         // 1/4
    4.0/3.0,     // 1/2 trip
    1.5,         // 1/4 dot
    2.0,         // 1/2
    8.0/3.0,     // 1/1 trip
    3.0,         // 1/2 dot
    4.0          // 1/1
};

void Tempo::reset() noexcept{
    bpm = 120.0;
}


void Tempo::update(const juce::AudioPlayHead *playhead) noexcept{
    
    reset(); //Reseteo por seguridad (default 120 bpm)
    
    if (playhead == nullptr) // Si playHead no ha sido asignado, salgase
        return;
    
    const auto opt = playhead->getPosition(); //Obtenga información del audio transport
    
    if (!opt.hasValue()) //Chequear que tenga un valor valido
        return;
    
    const auto &pos = *opt; //Obtener valor del puntero opt
    
    if (pos.getBpm().hasValue()) // Chequear valor valido
        bpm = *pos.getBpm(); //Obtenga el BPM en double.
}


double Tempo::getMillisecondsForNoteLength(int index) const noexcept{
    return 60000.0 * noteLengthMultipliers[size_t(index)] / bpm; //size_t?
}
