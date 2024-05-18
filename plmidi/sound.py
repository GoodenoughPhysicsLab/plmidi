import os
import shutil
import string
import tempfile
import multiprocessing
import os.path as path

import mido

from plmidi_cpp import sound_by_mciSendCommand, sound_by_mciSendCommand_with_duration

def _sound(midifile: str, is_print: bool):
    if not isinstance(midifile, str) and not path.exists(midifile):
        raise TypeError
    
    ascii_chars = tuple(string.ascii_letters + string.digits + string.punctuation + " ")
    use_tmpfile: bool = False
    for char in midifile:
        if char not in ascii_chars:
            use_tmpfile = True
            with tempfile.NamedTemporaryFile('w+t', delete=False) as _tmpfile:
                _tmpfile.close()
            shutil.copy(midifile, _tmpfile.name)
            midifile = _tmpfile.name
            break

    try:
        if is_print:
            midi_duration = 0
            for msg in mido.MidiFile(midifile, clip=True):
                midi_duration += msg.time
            sound_by_mciSendCommand_with_duration(midifile, int(midi_duration))
        else:
            sound_by_mciSendCommand(midifile)
    except KeyboardInterrupt:
        pass

    if use_tmpfile and path.exists(_tmpfile.name):
        os.remove(_tmpfile.name)

def sound(midifile: str, is_print: bool = True):
    try:
        p = multiprocessing.Process(target=_sound, args=(midifile, is_print))
        p.start()
        # p.join()
    except KeyboardInterrupt:
        pass