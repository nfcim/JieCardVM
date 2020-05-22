# File System Structure

All persistent data is saved in file system. It includes code and heap.

## Context

Each CAP file corresponds to one context. And each CAP has its own AID, so all data of this context lives under `/AID`.

### Constant pool

CAP file has only one constant pool after constant pools of java classes are merged under a single file in ConstantPool.cap. It is saved to a file called `/AID/c`. The pool can have at most 65536 entries.

Content:

```
constant_pool_component {
    u1 tag; // 5
    u2 size;
    u2 count;
    cp_info constant_pool[count];
};

cp_info {
    u1 tag;
    u1 info[3];
}
```

It's easy to access cp_info by its index.

### Methods

All methods are merged into `Method.cap` as well. The file is saved to `/AID/m` and its structure:

```
method_component_compat {
    u1 tag; // 7
    u2 size;
    u1 handler_count;
    exception_handler_info exception_handlers[handler_count];
    method_info methods[];
}

exception_handler_info {
    u2 start_offset;
    u2 stop_bit: 1;
    u2 active_length: 15;
    u2 handler_offset;
    u2 catch_type_index;
}

method_info {
    method_header method_header;
    u1 bytecodes[];
}

method_header_info {
    u1 flags: 4;
    u1 max_stack: 4;
    u1 nargs: 4;
    u1 max_locals: 4;
}
```

Because each method info has variable length, to speedup lookup, we store the offsets into a file called `/AID/M`. Its content:

```
method_lookup {
    u4 offsets[method_count];
}
```

### Classes

As with others, `Class.cap` is saved to `/AID/c` and it has the following structure:

```
class_component_compat {
    u1 tag; // 6
    u2 size;
    interface_info interfaces[];
    class_info_compat classes[];
}
```
