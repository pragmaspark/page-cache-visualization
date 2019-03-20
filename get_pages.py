#!/usr/bin/python3

import datetime
from subprocess import call

def add(x, y):
    return list(map(lambda a, b: a + b, x, y))

def main():
    file_name = ["po_full", "po", "page_owner_sort"]
    time = datetime.datetime.now()
    file_time = [f"-{time.year}-{time.month}-{time.day}-{time.hour}:{time.minute}"]*len(file_name)
    file_path = ["./data/"]*len(file_name)

    files = add(file_path, file_name)
    files = add(files,     file_time)
    
    call(f"cat /sys/kernel/debug/page_ownwer > {files[0]}", shell = True)
    call(f"grep -v ^PFT {files[0]} > {files[1]}", shell = True)
    call(f"touch {files[2]}", shell = True)
    call(f"page_owner_sort {files[1]} {files[2]}", shell = True)

if __name__ == "__main__":
    main()

