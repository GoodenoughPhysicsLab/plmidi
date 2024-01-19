#define PYBIND11_NO_ASSERT_GIL_HELD_INCREF_DECREF
#include "sound.hpp"

PYBIND11_MODULE(plmidi_cpp, m) {
    m.def("sound", plmidi::sound);
    // plmidiInitError
    py::register_exception<plmidi::plmidiExc_InitErr>(m, "plmidiInitError");
    // OpenMidiFileError
    py::register_exception<plmidi::OpenMidiFileError>(m, "OpenMidiFileError");
}
