from pydub import AudioSegment

bpm = 70
quarterNoteDuration = 60000 / bpm # gives duration in ms. 60,000ms / bpm = duration
divisions = 8

notes = [
    "E",
    "F",
    "F#",
    "G",
    "G#",
    "A",
    "A#",
    "B",
]

for i in range(divisions):
    start = i * quarterNoteDuration
    end = (i + 1) * quarterNoteDuration

    audio = AudioSegment.from_wav("splitting/batch1.wav")
    
    audio = audio[start:end]
    audio.export(f'splitting/{i+52}.wav', format="wav")
