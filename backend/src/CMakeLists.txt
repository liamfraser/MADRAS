cmake_minimum_required(VERSION 3.2)
project(MADRAS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic -O3")

set(SOURCE_FILES main.cpp AudioEngine.cpp AudioEngine.h PluginChain.cpp PluginChain.h Plugin.h StreamParameters.h Oscillator.h Oscillators/SineOscillator.h Plugins/OscillatorInsert.cpp Plugins/OscillatorInsert.h Plugins/RingModulator.cpp Plugins/RingModulator.h PluginParameter.h ParameterTypes/FloatPluginParameter.h ParameterTypes/WaveshapePluginParameter.h ParameterTypes/FloatSetterPluginParameter.h REST/RESTApi.cpp REST/RESTApi.h REST/RootResource.cpp REST/RootResource.h Plugins/BiQuad/BiQuad.h Plugins/BiQuad/LowPass.h ParameterTypes/FloatSetterBase.h Plugins/PitchShifters/PitchShift.cpp Plugins/PitchShifters/PitchShift.h Plugins/SoftDistortion.cpp Plugins/SoftDistortion.h Plugins/Delay.cpp Plugins/Delay.h Plugins/Flanger.h PluginComponents/DelayLine.h Plugins/PitchShifters/Vocoder.cpp Plugins/PitchShifters/Vocoder.h PluginComponents/Note.h PluginComponents/MidiTable.h Plugins/PitchShifters/PitchShiftWorker.h PluginComponents/ModulatedDelayBase.cpp PluginComponents/ModulatedDelayBase.h Plugins/Chorus.h PluginComponents/CombFilter.h PluginComponents/DelayLineFilter.h PluginComponents/AllPassFilter.h Plugins/Reverb.h PluginComponents/DelayLine.cpp)
add_executable(MADRAS ${SOURCE_FILES})
target_link_libraries(MADRAS pthread jack httpserver jsoncpp sndfile fftw3f)
