from pwn import *

context.terminal = ['tmux', 'new-window']
context.aslr=False
context.arch = 'amd64'
#context.log_level='debug'
e = ELF('/sharing/mediocrity')
libc = e.libc

if args.REMOTE:
    HOST, PORT = 'gradebook.2023.ctfcompetition.com 1337'.split(' ')
    p = remote(HOST, PORT)
else:
    p = process(e.path)

if args.GDB:
    gdb.attach(p, gdbscript="""
        decompiler connect ida --host host.docker.internal --port 3662
		heap-analysis
        #nb "before_call_wrapper" 0x0000555555554000+0x0000000000001F23
        #pie breakpoint 0x0000000000001FD3
        #memory watch 0x0000555555554000+0x0000000000003718 0x40 byte
        #pie breakpoint 0x000000000000150E
        """)

#libc.address=
#libc.address = libc_leaks - 0x14ed70
#env_addr = libc.sym['environ']
#e.address=0x0000555555554000
#ret = next(e.search(asm('ret')))
#ret = next(e.search(asm(' pop rdi ; out 0xff, eax ; call qword ptr [rax + 0x489001eb]')))
#log.critical(f"ret is {hex(ret)}")
p.interactive()
