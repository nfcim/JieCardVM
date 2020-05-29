#include <assert.h>
#include <fstream>
#include <iostream>
#include <map>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

// targets export file format 2.3

typedef uint8_t u1;
typedef uint16_t u2;
typedef uint32_t u4;

vector<char> read_file_binary(const string &path) {
  ifstream file(path, ios::binary);
  return vector<char>(istreambuf_iterator<char>(file),
                      istreambuf_iterator<char>());
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s path_to_exp\n", argv[0]);
    return 1;
  }
  string path = argv[1];
  auto content = read_file_binary(path);
  u1 *p = (u1 *)&content[0];
  map<int, string> name_mapping;

  u4 magic = *(u4 *)p;
  if (magic != 0xDECAFA00) {
    printf("Bad magic\n");
    return 1;
  }
  u1 minor_version = *(p + 4);
  u1 major_version = *(p + 5);
  printf("Version %d.%d\n", major_version, minor_version);

  u2 constant_pool_count = ntohs(*(u2 *)(p + 6));
  printf("Found %d constants\n", constant_pool_count);
  u1 *cur = p + 8;
  for (u2 i = 0; i < constant_pool_count; i++) {
    u1 tag = *cur++;
    u2 length;
    u2 name_index;
    u4 number;
    u1 flags;
    string str;
    switch (tag) {
    case 1:
      // utf8
      length = ntohs(*(u2 *)cur);
      cur += 2;
      str = string((char *)cur, length);
      cur += length;
      printf("%d: utf8 %s\n", i, str.c_str());
      name_mapping[i] = str;
      break;
    case 3:
      // integer
      number = ntohl(*(u4 *)cur);
      cur += 4;
      printf("%d: integer %d\n", i, number);
      break;
    case 7:
      // class ref
      name_index = ntohs(*(u2 *)cur);
      cur += 2;
      printf("%d: class ref name %d(%s)\n", i, name_index,
             name_mapping[name_index].c_str());
      name_mapping[i] = name_mapping[name_index];
      break;
    case 13:
      // package
      flags = *cur++;
      name_index = ntohs(*(u2 *)cur);
      cur += 2;
      minor_version = *cur++;
      major_version = *cur++;
      length = *cur++;
      printf("%d: package aid name %d(%s) version %d.%d", i, name_index,
             name_mapping[name_index].c_str(), major_version, minor_version);
      for (int i = 0; i < length; i++) {
        printf(" %02X", cur[i]);
      }
      printf("\n");
      cur += length;
      break;
    default:
      assert(false);
      break;
    }
  }

  u2 this_package = ntohs(*(u2 *)cur);
  cur += 2;
  printf("This package is %d\n", this_package);

  u1 referenced_package_count = *cur++;
  printf("Referenced %d packages:", referenced_package_count);
  for (int i = 0; i < referenced_package_count; i++) {
    u2 package = ntohs(*(u2 *)cur);
    cur += 2;
    printf(" %d", package);
  }
  printf("\n");

  u1 export_class_count = *cur++;
  printf("Export %d classes:\n", export_class_count);
  for (int i = 0; i < export_class_count; i++) {
    u1 token = *cur++;
    u2 access_flags = ntohs(*(u2 *)cur);
    cur += 2;
    u2 name_index = ntohs(*(u2 *)cur);
    cur += 2;
    u2 export_supers_count = ntohs(*(u2 *)cur);
    cur += 2;
    cur += 2 * export_supers_count;
    u1 export_interfaces_count = *cur++;
    cur += 2 * export_interfaces_count;
    u2 export_fields_count = ntohs(*(u2 *)cur);
    cur += 2;
    printf("%d: token %d access flags 0x%x name %d(%s) supers %d interfaces %d "
           "fields %d\n",
           i, token, access_flags, name_index, name_mapping[name_index].c_str(),
           export_supers_count, export_interfaces_count, export_fields_count);
    for (u1 j = 0; j < export_fields_count; j++) {
      u1 token = *cur++;
      u2 access_flags = ntohs(*(u2 *)cur);
      cur += 2;
      u2 name_index = ntohs(*(u2 *)cur);
      cur += 2;
      u2 descriptor_index = ntohs(*(u2 *)cur);
      cur += 2;
      u2 attributes_count = ntohs(*(u2 *)cur);
      cur += 2;
      for (u2 k = 0; k < attributes_count; k++) {
        u2 name_index = ntohs(*(u2 *)cur);
        cur += 2;
        u4 length = ntohl(*(u4 *)cur);
        cur += 4;
        cur += length;
      }
    }

    u2 export_methods_count = ntohs(*(u2 *)cur);
    cur += 2;
    printf("\tFound %d methods:\n", export_methods_count);
    for (u2 j = 0; j < export_methods_count; j++) {
      u1 token = *cur++;
      u2 access_flags = ntohs(*(u2 *)cur);
      cur += 2;
      u2 name_index = ntohs(*(u2 *)cur);
      cur += 2;
      u2 descriptor_index = ntohs(*(u2 *)cur);
      cur += 2;
      printf("\t%d: token %d access flags 0x%x name %d(%s) desc %d\n", j, token,
             access_flags, name_index, name_mapping[name_index].c_str(),
             descriptor_index);
    }
    u1 inheritable_public_method_token_count = *cur++;
  }

  return 0;
}