import os.path as path

try:
    import mido
except ImportError as e:
    e("can not use physicsLab.music, type `pip install mido`")

import plmidi_cpp

def sound(midifile: str):
    if not isinstance(midifile, str) and not path.exists(midifile):
        raise TypeError

    midi_duration = 0
    for msg in mido.MidiFile(midifile):
        midi_duration += msg.time
    
    plmidi_cpp.sound_by_mciSendCommand(midifile, midi_duration)