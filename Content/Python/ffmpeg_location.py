# this is small script that tries to find ffmpeg.exe location

import shutil
import sys

path = shutil.which("ffmpeg")
if path is not None:
    print(path)
    sys.exit(0)