#include "floating_point.h"
#include <string.h>

void show_bytes_reverted(byte *pointer, size_t size, char **destbuff) {
    size_t buffer_size = 3 * sizeof(char) * size + 10;
    *destbuff = malloc(buffer_size);
    (*destbuff)[0] = '\0';

    for (size_t i = size; i > 0; i--) {
        snprintf(*destbuff + strlen(*destbuff), buffer_size - strlen(*destbuff),
                 "%02x ", *(pointer + i - 1));
    }
    snprintf(*destbuff + strlen(*destbuff), buffer_size - strlen(*destbuff),
             "\n");
}
char *show_double_bytes(double d) {
    char *destbuff;
    show_bytes_reverted((byte *)&d, sizeof(double), &destbuff);
    return destbuff;
}
char *show_float_bytes(float f) {
    char *destbuff;
    show_bytes_reverted((byte *)&f, sizeof(float), &destbuff);
    return destbuff;
}

#ifdef STANDALONE_TEST
int main() {
    show_float_bytes(strtof("1.00f", NULL));
    show_double_bytes(strtod("1.00", NULL));
    return 0;
}
#endif
