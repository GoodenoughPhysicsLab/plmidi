import math
import os.path as path

try:
    import mido
except ImportError:
    print("can not use physicsLab.music, type `pip install mido`")

from .plmidi_cpp import sound as _sound, OpenMidiFileError, plmidiInitError

def sound(midifile: str):
    if not isinstance(midifile, str) and not path.exists(midifile):
        raise TypeError

    midi_duration = 0
    for msg in mido.MidiFile(midifile, clip=True):
        midi_duration += msg.time

    _sound(midifile, math.ceil(midi_duration))
