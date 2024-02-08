#include <signal.h>
#include "sound.hpp"

PYBIND11_MODULE(plmidi_cpp, m) {
    signal(SIGINT, detail::plmidi_exit);

    m.def("sound", plmidi::sound);

    py::register_exception<plmidi::plmidiInitError>(m, "plmidiInitError");
    py::register_exception<plmidi::OpenMidiFileError>(m, "OpenMidiFileError");
    py::register_exception<plmidi::plmidi_KeyBordInterrupt>(m, "plmidi_KeyBordInterrupt");
}
