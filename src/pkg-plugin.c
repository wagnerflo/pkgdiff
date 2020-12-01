/* Copyright 2020 Florian Wagner <florian@wagner-flo.net>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include <pkg.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>
#include "pkgdiff.h"

static char plugin_name[] = "diff";
static char plugin_desc[] = "Compare package files by metadata.";
static char plugin_version[] = "0.1";

int plugin_usage(void) {
  fprintf(stderr,
          "Usage: pkg %1$s [-q] [-x <item>] ... <pkg-name> <pkg-name>\n"
          "\n"
          "For more information see 'pkg help %1$s.'\n",
          plugin_name);
  return EX_USAGE;
}

void print_result(
    const char* name,
    struct pkgdiff_result* diff,
    unsigned int diff_len)
{
  printf("--- %s\n+++ %s\n", name, name);
  for (unsigned int i = 0; i < diff_len; i++) {
    for (unsigned int j = 0; j < diff[i].lines_len; j++) {
      fwrite(&diff[i].op, sizeof(char), 1, stdout);
      fwrite(diff[i].lines[j]->start, sizeof(char), diff[i].lines[j]->length, stdout);
      fwrite("\n", sizeof(char), 1, stdout);
    }
  }
}

int plugin_callback(int argc, char** argv) {
  pkgdiff_result_fn result_fn = &print_result;
  const char** query = malloc(PKGDIFF_QUERY_LEN * sizeof(const char*));
  memcpy(query, PKGDIFF_QUERY, PKGDIFF_QUERY_LEN * sizeof(const char*));
  char opt;

  while ((opt = getopt(argc, argv, "qx:")) != -1) {
    switch (opt) {
      case 'x':
        for (unsigned int i = 0; i < PKGDIFF_QUERY_LEN; i++) {
          if (query[i] != NULL && strcmp(query[i], optarg) == 0)
            query[i] = NULL;
        }
        break;

      case 'q':
        result_fn = NULL;
        break;

      default:
        free(query);
        return plugin_usage();
    }
  }

  argc -= optind;
  argv += optind;

  if (argc != 2) {
    free(query);
    return plugin_usage();
  }

  int ret = pkgdiff(argv[0], argv[1], query, result_fn);
  free(query);
  return ret;
}

int pkg_plugin_init(struct pkg_plugin* p) {
  pkg_plugin_set(p, PKG_PLUGIN_NAME, plugin_name);
  pkg_plugin_set(p, PKG_PLUGIN_DESC, plugin_desc);
  pkg_plugin_set(p, PKG_PLUGIN_VERSION, plugin_version);
  pkg_plugin_parse(p);
  return EPKG_OK;
}

int pkg_plugin_shutdown(struct pkg_plugin* p __unused) {
  return EPKG_OK;
}

int pkg_register_cmd(int idx,
                     const char** name,
                     const char** desc,
                     int (**exec)(int argc, char** argv)) {
  *name = plugin_name;
  *desc = plugin_desc;
  *exec = plugin_callback;
  return EPKG_OK;
}

int pkg_register_cmd_count (void) {
  return 1;
}
