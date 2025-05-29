# SimpleOS

This example demonstrates a very small runtime that loads applications dynamically.
It is not meant to replace the full firmware but showcases a possible direction
for a modular OS that can download and execute apps on demand.

Apps are compiled as shared libraries (`.so` files) and placed in the `apps/`
directory. The loader searches this directory at runtime, loads the libraries and
instantiates the contained `App` objects.

This code builds for a desktop host only. It relies on standard POSIX functions
(`dlopen`, `dlsym`) and uses stubs instead of the real hardware drivers.
