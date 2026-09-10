from pedalboard import Pedalboard, PitchShift, LowShelfFilter, Distortion, Compressor, Reverb
from pedalboard.io import AudioFile

INPUT_FILE = "garmadon_1.mp3"
OUTPUT_FILE = "garmadon_1_out.mp3"

with AudioFile(INPUT_FILE) as f:
    audio = f.read(f.frames)
    sample_rate = f.samplerate

board = Pedalboard([
    PitchShift(semitones=-0.2),
    LowShelfFilter(cutoff_frequency_hz=300, gain_db=4.0),
    Distortion(drive_db=10),
    Compressor(threshold_db=14, ratio=6.0),
    Reverb(room_size=0.1, damping=0.1, wet_level=0.1, dry_level=0.85)
])

effected = board(audio, sample_rate)

with AudioFile(OUTPUT_FILE, 'w', sample_rate, effected.shape[0]) as f:
    f.write(effected)

print(f"Обработка завершена! Файл сохранен: {OUTPUT_FILE}")