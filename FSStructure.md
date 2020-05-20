# File System Structure

All persistent data is saved in file system. It includes code and heap.

## Context

Each CAP file corresponds to one context. And each CAP has its own AID, so all data of this context lives under `/AID`.

### Constant pool

Here constant pool refers to the variable length one in .class file.

Each java class has its own constant pool, and constant pools are merged under a single file called `/AID/c`. The merged pool can have at most 65536 entries.

Content:

```
merged_constant_pool {
    u4 constant_pool_size;
    u4 constant_pool_offset[constant_pool_size];
    cp_info consts[constant_pool_size];
};
```

Struct `cp_info` is defined in Java Class File definition.

Each `u4` of `constant_pool_offset` saves the file offset of the corresponding `cp_info` struct. To lookup a `cp_info`, a two-step sequence can be:

```
1. seek to (index + 1) * 4 and read offset
2. seek to offset and read cp_info
```

Thus, indexes in .class files need to be relocated by an offset.

### Methods

All methods are merged into one file as well. The file is called `/AID/m` and it consists of several `method_info` structs:

```
method_info {
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attributes_count;
    attribute_info attributes[attributes_info];
}
```

Currently, we use linear search for lookup from method name to method info. The offsets are stored in a file called `/AID/M`. Its content:

```
method_lookup {
    u4 method_count;
    u4 offsets[method_count];
}
```

In the future, we can use cuckoo hashing to speed it up.

### Classes

All classes are merged into one file as well. The file is called `/AID/c` and it consists of several `class_info` structs:

```
class_info {
    u2 access_flags;
    u2 this_class;
    u2 super_class;
    u2 interfaces_count;
    u2 interfaces[interfaces_count];
    u2 fields_count;
    field_info fields[fields_count];
}
```

Currently, we use linear search for lookup from class name to class info. The offsets are stored in a file called `/AID/C`. Its content:

```
class_lookup {
    u4 class_count;
    u4 offsets[class_count];
}
```

In the future, we can use cuckoo hashing to speed it up.
