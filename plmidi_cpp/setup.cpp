#include "sound.hpp"

PYBIND11_MODULE(plmidi_cpp, m) {
    m.def("sound", plmidi::sound);

    py::register_exception<plmidi::plmidiInitError>(m, "plmidiInitError");
    py::register_exception<plmidi::OpenMidiFileError>(m, "OpenMidiFileError");
    py::register_exception<plmidi::plmidi_KeyBordInterrupt>(m, "plmidi_KeyBordInterrupt");
}
