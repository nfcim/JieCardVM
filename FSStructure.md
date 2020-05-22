# File System Structure

All persistent data is saved in file system. It includes code and heap.

## Context

Each CAP file corresponds to one context. And each CAP has its own AID, so all data of this context lives under `/AID`.

### Constant pool

`Constant.cap` is copied to a file called `/AID/c`.

### Methods

`Method.cap` is copied to a file called `/AID/m`.


### Classes

`Class.cap` is copied to a file called `/AID/C`.


### Applets

`Applet.cap` is copied to a file called `/AID/a`.
