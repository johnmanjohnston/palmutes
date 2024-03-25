from pydub import AudioSegment

notes = [
    "E",
    "F",
    "F#",
    "G",
    "G#",
    "A",
    "A#",
    "B",
    "C",
    "C#"
]

bpm = 70
quarterNoteDuration = 60000 / bpm # gives duration in ms. 60,000ms / bpm = duration
variantsPerNote = 4
totalDivisions = len(notes) * variantsPerNote

for i in range(totalDivisions):
    print(f"Trimming for note number {52+i}")
    for j in range(variantsPerNote):
        print(f"    trimming for variant {j}")
        start = (i * variantsPerNote + j) * quarterNoteDuration
        end = (i * variantsPerNote + j + 1) * quarterNoteDuration

        audio = AudioSegment.from_wav("splitting/batch2.wav")
        
        audio = audio[start:end]
        audio.export(f'splitting/{i+52}_{j}.wav', format="wav")
