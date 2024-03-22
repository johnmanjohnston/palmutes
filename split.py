from pydub import AudioSegment

bpm = 70
quarterNoteDuration = 60000 / bpm # gives duration in ms. 60,000ms / bpm = duration
divisions = 4

for i in range(divisions):
    start = i * quarterNoteDuration
    end = (i + 1) * quarterNoteDuration

    audio = AudioSegment.from_wav("splitting/splittest.wav")
    
    audio = audio[start:end]
    audio.export(f'splitting/splitoutput_{i}.wav', format="wav")