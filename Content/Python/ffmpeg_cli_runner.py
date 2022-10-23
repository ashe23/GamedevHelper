import argparse
import subprocess
import unreal
import json
import os
import time

    
def parse_queue(json_file):
    json_file = os.path.normpath(json_file)
    if not os.path.exists(json_file):
        unreal.log_error("Cant find {} file".format(json_file))
        exit(0)

    # load json file
    with open(json_file, encoding="utf8") as f:
        data = json.load(f)

    return data

parser = argparse.ArgumentParser(description="GamedevHelper Rendering Manager ffmpeg runner")
parser.add_argument("-queue", type=parse_queue, dest="queue", help = "FFmpeg render commands")

args = vars(parser.parse_args())
cmds = args['queue']

jobs_total = len(cmds)
start_time = time.time()
jobs_finished = 0

with unreal.ScopedSlowTask(jobs_total, "Encoding...") as slow_task:
    slow_task.make_dialog(False, False)

    for cmd in cmds:

        cmd_str = str(cmd).split(":")
        sequence_name = cmd_str[0]
        ffmpeg_cmd = cmds[cmd]

        slow_task.enter_progress_frame(1, "Encoding {} ({} of {})".format(sequence_name, jobs_finished, jobs_total))

        result = subprocess.run(ffmpeg_cmd, shell=True)
        if result.returncode != 0:
            unreal.log_error("Failed cmd ({}) : {}".format(sequence_name, ffmpeg_cmd))
            break

        jobs_finished+=1

elapsed_time = time.time() - start_time
human_time = unreal.GdhTimeLibrary.get_human_readable_time(elapsed_time)

if jobs_finished == jobs_total:
    msg = "Encoding done. Finished {} of {} jobs in {}".format(jobs_finished, jobs_total, human_time)
    unreal.GdhNotificationLibrary.show_modal("FFmpeg Encoder", msg, unreal.GdhModalStatus.OK, 10.0)
else:
    msg = "Failed to encode some commands. Check OutputLog in UnrealEngine"
    unreal.GdhNotificationLibrary.show_modal_output_log("FFmpeg Encoder", msg, unreal.GdhModalStatus.ERROR, 10.0)
