#include <assert.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

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
      printf("%d: class ref name %d\n", i, name_index);
      break;
    case 13:
      // package
      flags = *cur++;
      name_index = ntohs(*(u2 *)cur);
      cur += 2;
      minor_version = *cur++;
      major_version = *cur++;
      length = *cur++;
      printf("%d: package aid name %d version %d.%d", i, name_index,
             major_version, minor_version);
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

  return 0;
}