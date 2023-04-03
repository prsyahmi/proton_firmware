#!/usr/bin/env python3
#
# Cross Platform and Multi Architecture Advanced Binary Emulation Framework
#

import sys
sys.path.append("..")

from qiling import Qiling
from qiling.const import QL_VERBOSE, QL_INTERCEPT
from qiling.os.const import INT, STRING
from qiling.os.disk import QlDisk
import os
fd = os.open("system/bin/ls", os.O_RDONLY, 0o666)
s = os.fstat(fd)
print(f"{s}")

def ql_syscall_sigaltstack(ql: Qiling, addr, args):
    return 0

def ql_syscall_sigaction(ql: Qiling, addr, args):
    return 0

def ql_syscall_sigsuspend(ql: Qiling, addr, args):
    return 0

def my_open(ql, address, size):
    # Get the file descriptor associated with the opened file
    fd = ql.reg.read(ql.loader.registers['arg'][0])
    # Get the filename associated with the file descriptor
    filename = ql.os.fd2name(fd)
    # Print the filename to the console
    print("Opened file:", filename)

def my_androidprint2(ql: Qiling, *args):
    # Get the log message parameters
    params = ql.os.resolve_fcall_params({
        'level1': INT,
        'level': INT,
        'tag': STRING,
        'msg': STRING,
        'v1': STRING
    })

    # Print the log message to the console
    ql.log.info(f"!!!!!!!!!!!!!!!! - [{params['level']} {params['tag']}] {params['msg']}")
    #print(params['msg'])

if __name__ == "__main__":
    # ql = Qiling(
    #     ["install"],
    #     ".",
    #     multithread=True,
    #     verbose=QL_VERBOSE.OFF
    #     #verbose=QL_VERBOSE.DEBUG
    # )
    ql = Qiling(
        ["system/bin/minirecovery", "5install", "/attack-finish/update/update.zip"],
        #["system/bin/minirecovery", "80"],
        ".",
        multithread=True,
        #verbose=QL_VERBOSE.DEBUG
        verbose=QL_VERBOSE.OFF
    )
    
    #emu_disk = QlDisk('dev/system.img', '/dev/block/mmcblk0')
    #ql.add_fs_mapper('/dev/block/mmcblk0', emu_disk)

    #ql.os.set_api('access', my_access, QL_INTERCEPT.CALL)
    #ql.os.set_api('fstat', my_fstat, QL_INTERCEPT.CALL)
    #ql.os.chmod("/system/bin/ls", 1)

    ql.add_fs_mapper('/storage', 'storage')
    ql.add_fs_mapper('/storage/emulated/legacy', 'sdcard')
    ql.add_fs_mapper('/mnt/shell/emulated/0', 'sdcard')

    ql.add_fs_mapper('/proc/cmdline', 'dev/cmdline.txt')
    ql.add_fs_mapper('/dev/block/mmcblk0', 'dev/system.img')
    ql.add_fs_mapper('/dev/log/main', 'dev/log_main.txt')
    ql.add_fs_mapper('/dev/log/radio', 'dev/log_radio.txt')
    ql.add_fs_mapper('/dev/log/events', 'dev/log_events.txt')
    ql.add_fs_mapper('/dev/log/system', 'dev/log_system.txt')
    ql.add_fs_mapper('/sys/devices/platform/debug_control/percent_write', 'dev/percent_write.txt')
    #ql.os.set_syscall(0xba, ql_syscall_sigaltstack)
    #ql.os.set_syscall(0x43, ql_syscall_sigaction)
    #ql.os.set_syscall(72, ql_syscall_sigsuspend)

    #ql.os.set_api("open", my_open)
    #ql.os.set_api('__android_log_buf_print', my_androidprint2, QL_INTERCEPT.ENTER)

    ql.mem.map(0x0, 0x1000, 7)
    ql.mem.map(0x4a000, 0x40000, 7)
    #ql.mem.map(0x565a6000, 0x1000, 7)

    #ql.debugger = True
    #ql.debugger = "qdb:0x565a6000"
    ql.run()
