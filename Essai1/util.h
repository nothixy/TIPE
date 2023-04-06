#include "vector.h"
#include <stdio.h>
#include <printf.h>
#include <stdbool.h>

int print_bool(FILE *stream,
              const struct printf_info *info,
              const void *const *args) {
				  const bool *b;
  char *buffer;
  int len;

  /* Format the output into a string. */
  b = *((const bool **) (args[0]));
  len = asprintf (&buffer, "%s", b ? "TRUE" : "FALSE");
  if (len == -1)
    return -1;

  /* Pad to the minimum field width and print to the stream. */
  len = fprintf (stream, "%*s",
                 (info->left ? -info->width : info->width),
                 buffer);

  /* Clean up and return. */
  free (buffer);
  return len;
}
int x = register_printf_function('B', &print_bool, NULL);
