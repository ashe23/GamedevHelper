import subprocess
import unreal

# we will receive command that user picks from unreal editor and check if its valid command, and report back to editor, so user can check realtime if command valid or not
cmd = "ffmpeg.exe -i"
output = subprocess.call(cmd, shell=True)
unreal.log(output)
# if subprocess.call(cmd, shell=True) != 0:
#     unreal.log_error("Cmd failed {}".format(cmd))
