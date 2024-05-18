#include "define.hpp"

#if !PLMIDI_IS_WINDOWS
#   error "Do not use windows"
#endif // PLMIDI_IS_WINDOWS

#include "sound.hpp"
PYBIND11_MODULE(plmidi_cpp, m) {
    m.def("sound_by_mciSendCommand", plmidi::sound_by_mciSendCommand);
    m.def("sound_by_mciSendCommand_with_duration", plmidi::sound_by_mciSendCommand_with_duration);

    py::register_exception<plmidi::plmidiInitError>(m, "plmidiInitError");
    py::register_exception<plmidi::OpenMidiFileError>(m, "OpenMidiFileError");
}
