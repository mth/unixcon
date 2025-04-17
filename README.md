# Connect telnet into local UNIX socket

This provides `LD_PRELOAD` wrapper to allow connecting into UNIX sockets with telnet client.

You must first install usual telnet client (for example inetutils-telnet package in Debian).

The unixcon wrapper can be installed into ~/bin using `make`.

    make install

Then run unixcon:

    unixcon /path/to/serial.socket

This is mostly useful for accessing qemu virtual machine console.

Add the following arguments to the `qemu-system-x86_64` command:

    -chardev socket,telnet=on,server=on,wait=off,path=/path/to/serial.socket,id=char0,signal=off
    -serial chardev:char0

Ensure after qemu startup that the socket user/group/mode permissions are suitable and you can use the `unixcon` to access the virtual machines serial console.
