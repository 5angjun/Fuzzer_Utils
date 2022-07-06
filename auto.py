import sys
import subprocess
import os


def exec_cmd(cmd, file_path):
    cmd = list(cmd)
    idx = cmd.index("@@") if "@@" in cmd else -1

    if idx>0:
        cmd[idx] = file_path
        r = subprocess.run(cmd, capture_output=True)
    #else:
    #    with ope
    return file_path, r.stdout, r.stderr

if __name__=='__main__':
    cmd = sys.argv[2:]

    with os.scandir(sys.argv[1]) as item:
        for f_name in item:
            if f_name.is_file() and f_name.name !="README.txt":
                sample, _, err =exec_cmd(cmd, f_name.path)
                log=str(_)
                index = log.find('total values ')
                slice_front = log[index + len('total values '):]
                index = slice_front.find(')')
                values = slice_front[:index]

                #print(f_name.path)
                #print(values)
                os.rename(f_name.path,values)
                #sys.exit(0)

