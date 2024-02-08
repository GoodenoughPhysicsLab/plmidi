#pragma once

#ifdef _WIN32
#pragma comment(lib, "winmm.lib")
#endif // _WIN32

#include <string>
#ifdef _WIN32
#include <Windows.h>
#include <mmsystem.h>
#endif // _WIN32

#include "process_bar.hpp"
#include "pybind11/pybind11.h"

namespace py = pybind11;

namespace detail {

volatile bool is_signal = false; // should I use atomic<bool> ?

static void plmidi_exit(int signal)
{
    if (signal == SIGINT) {
        is_signal = true;
    }
}

} // namespace detil

namespace plmidi {

struct plmidiInitError : public ::std::exception
{
    const char *err_msg = "module plmidi init fail";

    plmidiInitError() = default;
    plmidiInitError(const char *text) {
        err_msg = text;
    }
    ~plmidiInitError() = default;

    const char* what() const noexcept override {
        return err_msg;
    }
};

struct OpenMidiFileError : public ::std::exception
{
    const char *err_msg = "Failed to open MIDI file";

    OpenMidiFileError() = default;
    OpenMidiFileError(const char *text) {
        err_msg = text;
    }
    ~OpenMidiFileError() = default;

    const char* what() const noexcept override {
        return err_msg;
    }
};

struct plmidi_KeyBordInterrupt : public ::std::exception
{
    const char *err_msg = "KeyBord interrupt";

    plmidi_KeyBordInterrupt() = default;
    plmidi_KeyBordInterrupt(const char *text) {
        err_msg = text;
    }
    ~plmidi_KeyBordInterrupt() = default;

    const char* what() const noexcept override {
        return err_msg;
    }
};

#if 0
static int8_t plmidi_initflag = 0; // -1: fail, 0: ready to init, 1: success initialized

// input -> mido.MidiTrack, tempo: int
// class mido.MidiTrack(list)
void sound_by_midiOutShortMsg(py::list piece, int tempo)
{
    // init midi player
    HMIDIOUT handle;
    if (plmidi_initflag == 0) {
        if (midiOutOpen(&handle, MIDI_MAPPER, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR) {
            plmidi_initflag = -1;
        } else {
            plmidi_initflag = 1;
        }
    }
    if (plmidi_initflag == -1) {
        throw plmidiExc_InitErr();
    }

    // init exit
    signal(SIGINT, details::plmidi_exit);

    // main loop
#ifdef PLMIDI_DEBUG
        py::print("main loop of plmidi player start");
#endif // PLMIDI_DEBUG
    for (auto& msg : piece) // msg: mido.Message
    {
        ::std::string msg_type = msg.attr("type").cast<::std::string>();
#ifdef PLMIDI_DEBUG
        py::print("msg_type: ", msg_type);
#endif // PLMIDI_DEBUG
        // sound midi
        if ( msg_type.compare("program_change") == 0 ) {
            midiOutShortMsg(handle, msg.attr("program").cast<int>() << 8 | 0xC0 + msg.attr("channel").cast<int>());
        } else if ( msg_type.compare("note_on") == 0 ) {
            midiOutShortMsg(handle, msg.attr("velocity").cast<int>() << 16 | msg.attr("note").cast<int>() << 8 | 0x90 + msg.attr("channel").cast<int>());
        } else if ( msg_type.compare("note_off") == 0 ) {
            midiOutShortMsg(handle, msg.attr("velocity").cast<int>() << 16 | msg.attr("note").cast<int>() << 8 | 0x80 + msg.attr("channel").cast<int>());
        }
#ifdef PLMIDI_DEBUG
            py::print(msg);
#endif // PLMIDI_DEBUG
        Sleep(msg.attr("time").cast<int>() * tempo / 500'000); 
    }
#ifdef PLMIDI_DEBUG
    py::print("main loop of plmidi player end");
#endif // PLMIDI_DEBUG
    midiOutClose(handle);
}
#endif // 0

#ifdef _WIN32
//TODO: rename path to English file
void sound_by_mciSendCommand(py::str path, float midi_duration)
{
    // I don't know why this will fail to play
    // const char *cpath = path.cast<::std::string>().c_str();
    ::std::string spath = path.cast<::std::string>();
    const char *cpath = spath.c_str();

    process_bar::MidiProcessBar pb{midi_duration};

    // Open MIDI file
    MCI_OPEN_PARMS mciOpenParms;
    mciOpenParms.lpstrDeviceType = "sequencer";
    mciOpenParms.lpstrElementName = cpath;
    if (mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD_PTR)&mciOpenParms) != 0) {
        throw OpenMidiFileError();
    }

   // Play MIDI file
    MCI_PLAY_PARMS mciPlayParms;
    if (mciSendCommand(mciOpenParms.wDeviceID, MCI_PLAY, 0, (DWORD_PTR)&mciPlayParms) != 0) {
        mciSendCommand(mciOpenParms.wDeviceID, MCI_CLOSE, 0, 0); // Close the device
        throw plmidiInitError("Failed to play MIDI file");
    }

    // Continuously check the status of MIDI playback
    while (true) {
        MCI_STATUS_PARMS mciStatusParms;
        mciStatusParms.dwItem = MCI_STATUS_MODE;
        if (mciSendCommand(mciOpenParms.wDeviceID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD_PTR)&mciStatusParms) != 0) {
            py::print("Failed to check MIDI playback status");
            break;
        }

        if (mciStatusParms.dwReturn == MCI_MODE_STOP) {
            // MIDI playback has reached the end
            break;
        }

        if (detail::is_signal) {
            puts("\n");

            if (mciSendCommand(mciOpenParms.wDeviceID, MCI_CLOSE, 0, 0) != 0) {
                throw plmidiInitError("Failed to close MIDI device");
            }

            throw plmidi_KeyBordInterrupt();
        }

        pb.print();
        Sleep(100); // Sleep for a short duration before checking again
        pb.update();
    }

    // Close MIDI device
    if (mciSendCommand(mciOpenParms.wDeviceID, MCI_CLOSE, 0, 0) != 0) {
        throw plmidiInitError("Failed to close MIDI device");
    }
}
#endif // _WIN32

void sound(py::str path, float midi_duration)
{
#ifdef _WIN32
    sound_by_mciSendCommand(path, midi_duration);
#else
    py::print("can not support sound plmidi on os except windows");
#endif
}

} // namespace plmidi
