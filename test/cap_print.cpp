#include <assert.h>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

typedef uint8_t u1;
typedef uint16_t u2;
typedef uint32_t u4;

// Target: CAP 2.1

vector<char> read_file_binary(const string &path) {
  ifstream file(path, ios::binary);
  return vector<char>(istreambuf_iterator<char>(file),
                      istreambuf_iterator<char>());
}

string read_file_string(const string &path) {
  ifstream file(path);
  return string(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
}

struct __attribute__((__packed__)) package_info {
  u1 minor_version;
  u1 major_version;
  u1 aid_length;
  u1 aid[0];
};

struct __attribute__((__packed__)) header_component {
  u1 tag;
  u2 size;
  u4 magic;
  u1 cap_format_minor_version;
  u1 cap_format_major_version;
  u1 flags;
  package_info package;
};

void parse_header(const string &javacard_path) {
  auto header = read_file_binary(javacard_path + "/Header.cap");
  u1 *p = (u1 *)&header[0];
  header_component *comp = (header_component *)p;
  assert(comp->tag == 1);
  printf("CAP format version %d.%d, flags 0x%02x\n",
         comp->cap_format_major_version, comp->cap_format_minor_version,
         comp->flags);
  printf("Package version %d.%d\n", comp->package.major_version,
         comp->package.minor_version);
  printf("AID:");
  for (u1 i = 0; i < comp->package.aid_length; i++) {
    printf(" %02X", comp->package.aid[i]);
  }
  printf("\n");
}

struct __attribute__((__packed__)) static_field_size_info {
  u2 image_size;
  u2 array_init_count;
  u2 array_init_size;
};

struct __attribute__((__packed__)) custom_component_info {
  u1 component_tag;
  u2 size;
  u1 aid_length;
  u1 aid[0];
};

struct __attribute__((__packed__)) component_size_info {
  u2 header_component_size;
  u2 directory_component_size;
  u2 applet_component_size;
  u2 import_component_size;
  u2 constant_pool_component_size;
  u2 class_component_size;
  u2 method_component_size;
  u2 static_field_component_size;
  u2 reference_location_component_size;
  u2 export_component_size;
  u2 descriptor_component_size;
};

struct __attribute__((__packed__)) directory_component {
  u1 tag;
  u2 size;
  component_size_info component_sizes;
  static_field_size_info static_field_size;
  u1 import_count;
  u1 applet_count;
  u1 custom_count;
  custom_component_info custom_components[0];
};

void parse_directory(const string &javacard_path) {
  auto header = read_file_binary(javacard_path + "/Directory.cap");
  u1 *p = (u1 *)&header[0];
  directory_component *comp = (directory_component *)p;
  assert(comp->tag == 2);
  printf(
      "Static field size: image %d, array init count %d, array init size %d\n",
      comp->static_field_size.image_size,
      comp->static_field_size.array_init_count,
      comp->static_field_size.array_init_size);
  printf("Import %d, applet %d, custom %d\n", comp->import_count,
         comp->applet_count, comp->custom_count);
}

struct __attribute__((__packed__)) applet_component {
  u1 tag;
  u2 size;
  u1 count;
  struct {
    u1 aid_length;
    u1 aid[0];
    u2 install_method_offset;
  } applets[0];
};

void parse_applet(const string &javacard_path) {
  auto header = read_file_binary(javacard_path + "/Applet.cap");
  u1 *p = (u1 *)&header[0];
  applet_component *comp = (applet_component *)p;
  assert(comp->tag == 3);
  printf("Found %d applets:\n", comp->count);
  p += 4;
  for (int j = 0; j < comp->count; j++) {
    u1 length = *p;
    printf("AID:");
    for (u1 i = 0; i < length; i++) {
      printf(" %02X", p[1 + i]);
    }
    printf("\n");
    p += 1 + length;
    u2 offset = ntohs(*(u2 *)p);
    printf("Install method offset: %d\n", offset);
    p += 2;
  }
}

struct __attribute__((__packed__)) import_component {
  u1 tag;
  u2 size;
  u1 count;
  package_info packages[0];
};

void parse_import(const string &javacard_path) {
  auto header = read_file_binary(javacard_path + "/Import.cap");
  u1 *p = (u1 *)&header[0];
  import_component *comp = (import_component *)p;
  assert(comp->tag == 4);
  printf("Found %d imports:\n", comp->count);
  p += 4;
  for (int j = 0; j < comp->count; j++) {
    package_info *info = (package_info *)p;
    printf("Package: version %d.%d, aid", info->major_version,
           info->minor_version);
    for (u1 i = 0; i < info->aid_length; i++) {
      printf(" %02X", info->aid[i]);
    }
    printf("\n");
    p += sizeof(package_info) + info->aid_length;
  }
}

struct __attribute__((__packed__)) cp_info {
  u1 tag;
  u1 info[3];
};

struct __attribute__((__packed__)) constant_pool_component {
  u1 tag;
  u2 size;
  u2 count;
  cp_info constant_pool[0];
};

void parse_constant_pool(const string &javacard_path) {
  auto header = read_file_binary(javacard_path + "/ConstantPool.cap");
  u1 *p = (u1 *)&header[0];
  constant_pool_component *comp = (constant_pool_component *)p;
  assert(comp->tag == 5);
  u2 count = ntohs(comp->count);
  printf("Found %d constants:\n", count);
  for (u2 i = 0; i < count; i++) {
    printf("Constant #%d: ", i);
    switch (comp->constant_pool[i].tag) {
    case 1:
      // class ref
      printf("class ref\n");
      break;
    case 2:
      // instance field ref
      printf("instance field ref\n");
      break;
    case 3:
      // virtual method ref
      printf("virtual method ref\n");
      break;
    case 4:
      // super method ref
      printf("super method ref\n");
      break;
    case 5:
      // static field ref
      printf("static field ref\n");
      break;
    case 6:
      // static method ref
      printf("static method ref\n");
      break;

    default:
      assert(false);
      break;
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s path_to_decompressed_cap\n", argv[0]);
    return 1;
  }
  string path = argv[1];
  auto manifest = read_file_string(path + "/META-INF/MANIFEST.MF");

  istringstream ss(manifest);
  string line;
  string prefix = "Java-Card-Package-Name: ";
  string package;
  while (getline(ss, line)) {
    if (line.substr(0, prefix.length()) == prefix) {
      package =
          line.substr(prefix.length(), line.length() - prefix.length() - 1);
      printf("Found package name: %s\n", package.c_str());
      break;
    }
  }

  // replace package name '.' -> '/' and append 'javacard' to find cap files
  auto javacard_path = regex_replace(package, regex("\\."), "/");
  javacard_path = path + javacard_path + "/javacard";
  printf("Looking for CAP files under %s\n", javacard_path.c_str());

  parse_header(javacard_path);
  parse_directory(javacard_path);
  parse_applet(javacard_path);
  parse_import(javacard_path);
  parse_constant_pool(javacard_path);
  return 0;
}