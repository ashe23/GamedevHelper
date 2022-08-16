# import unreal

import argparse
import subprocess
import unreal
import json
import os
import time

# def hms(seconds):
#     ''' Converts seconds to 0000h 00m 00s format'''

#     d = seconds / 86400
#     h = seconds // 3600
#     m = seconds % 3600 // 60
#     s = seconds % 3600 % 60
#     return "{:03}d {:02d}h {:02d}m {:02d}s".format(int(d), int(h), int(m), int(s))

intervals = (
    ('weeks', 604800),  # 60 * 60 * 24 * 7
    ('days', 86400),    # 60 * 60 * 24
    ('hours', 3600),    # 60 * 60
    ('minutes', 60),
    ('seconds', 1),
)

def display_time(seconds, granularity=2):
    result = []

    for name, count in intervals:
        value = seconds // count
        if value:
            seconds -= value * count
            if value == 1:
                name = name.rstrip('s')
            result.append("{} {}".format(value, name))
    return ' '.join(result[:granularity])

def parse_queue(json_file):
    json_file = os.path.normpath(json_file)
    if not os.path.exists(json_file):
        unreal.log_error("Cant find {} file".format(json_file))
        exit(0)

    # load json file
    with open(json_file, encoding="utf8") as f:
        data = json.load(f)

    return data

gdh_subsystem = unreal.get_editor_subsystem(unreal.GamedevHelperSubsystem)

parser = argparse.ArgumentParser(description="GamedevHelper Rendering pipeline ffmpeg runner")
parser.add_argument("-queue", type=parse_queue, dest="queue", help = "FFmpeg render commands")

args = vars(parser.parse_args())
cmds = args['queue']

steps = len(cmds)
start_time = time.time()
rendered = 0

with unreal.ScopedSlowTask(steps, "Encoding...") as slow_task:
    slow_task.make_dialog(True)

    for cmd in cmds:

        cmd_str = str(cmd).split(":")
        command_name = cmd_str[0]
        pipeline_name = cmd_str[1]
        shot_name = cmd_str[2]
        audio_track = cmd_str[3]
        ffmpeg_cmd = cmds[cmd]

        slow_task.enter_progress_frame(1, "{} {}:{}:{} ({} of {})".format(command_name, pipeline_name, shot_name, audio_track, rendered, steps))

        if slow_task.should_cancel():
            break

        if subprocess.call(ffmpeg_cmd, shell=True) != 0:
            unreal.log_error("{} failed: {}:{}".format(command_name,pipeline_name, shot_name))
            break

        rendered+=1

elapsed_time = time.time() - start_time
msg = "Encoded {} of {} videos in {}".format(rendered, steps, display_time(elapsed_time))
unreal.log_warning(msg)
gdh_subsystem.show_modal(msg, unreal.GamedevHelperModalStatus.SUCCESS, 10.0)