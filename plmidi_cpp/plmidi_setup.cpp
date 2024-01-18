#include "plmidi_sound.hpp"

PYBIND11_MODULE(plmidi_cpp, m) {
#ifdef _WIN32
    m.def("sound_by_midiOutShortMsg", plmidi::sound_by_midiOutShortMsg);
    m.def("sound_by_mciSendCommand", plmidi::sound_by_mciSendCommand);
    // plmidiInitError
    py::register_exception<plmidi::plmidiExc_InitErr>(m, "plmidiInitError");
    // OpenMidiFileError
    py::register_exception<plmidi::OpenMidiFileError>(m, "OpenMidiFileError");
#endif
}