#include <stdio.h>
#include <stdlib.h>

static void print_bytes(const unsigned char *buf, size_t n) {
    for (size_t i = 0; i < n; i++) {
        printf("%02x ", buf[i]);
    }
    printf("\n");
}

int main(void) {
    const char *exe_path   = "ttt3-NODECRYPTOR.exe";
    const char *blob_path  = "DECRYPTEDBODY.bin";

    const long patch_offset = 0x2000;   // file offset for VMA 0x800000
    const size_t patch_size = 0x4010;   // expected size of decrypted region

    FILE *fexe  = fopen(exe_path, "r+b");
    FILE *fblob = fopen(blob_path, "rb");
    if (!fexe || !fblob) {
        perror("fopen");
        return 1;
    }

    unsigned char *buf = malloc(patch_size);
    if (!buf) {
        perror("malloc");
        return 1;
    }

    size_t n = fread(buf, 1, patch_size, fblob);
    if (n != patch_size) {
        fprintf(stderr, "blob too small: expected %zu, got %zu\n", patch_size, n);
        return 1;
    }

    printf("First 16 bytes of DECRYPTEDBODY.bin:\n");
    print_bytes(buf, 16);

    if (fseek(fexe, patch_offset, SEEK_SET) != 0) {
        perror("fseek exe");
        return 1;
    }

    n = fwrite(buf, 1, patch_size, fexe);
    if (n != patch_size) {
        fprintf(stderr, "short write: expected %zu, wrote %zu\n", patch_size, n);
        return 1;
    }

    fflush(fexe);
    fclose(fexe);
    fclose(fblob);
    free(buf);

    printf("Wrote %zu bytes at file offset 0x%lx\n", patch_size, patch_offset);

    // Reopen EXE and verify
    fexe = fopen(exe_path, "rb");
    if (!fexe) {
        perror("reopen exe");
        return 1;
    }

    if (fseek(fexe, patch_offset, SEEK_SET) != 0) {
        perror("fseek verify");
        return 1;
    }

    unsigned char verify[16];
    n = fread(verify, 1, sizeof(verify), fexe);
    fclose(fexe);

    printf("First 16 bytes at patch location in EXE:\n");
    print_bytes(verify, n);

    return 0;
}
