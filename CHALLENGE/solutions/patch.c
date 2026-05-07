#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <elf.h>

static long va_to_offset(FILE *f, uint64_t va) {
    Elf64_Ehdr eh;
    Elf64_Phdr ph;

    fseek(f, 0, SEEK_SET);
    fread(&eh, 1, sizeof(eh), f);

    fseek(f, eh.e_phoff, SEEK_SET);
    for (int i = 0; i < eh.e_phnum; i++) {
        fread(&ph, 1, sizeof(ph), f);

        uint64_t seg_start = ph.p_vaddr;
        uint64_t seg_end   = ph.p_vaddr + ph.p_memsz;

        if (va >= seg_start && va < seg_end) {
            return (va - ph.p_vaddr) + ph.p_offset;
        }
    }

    return -1;
}

int patch_with_bin(const char *exe_path, const char *bin_path) {
    // Load patch bytes
    FILE *bin = fopen(bin_path, "rb");
    if (!bin) {
        perror("open bin");
        return 1;
    }

    fseek(bin, 0, SEEK_END);
    long bin_size = ftell(bin);
    fseek(bin, 0, SEEK_SET);

    uint8_t *buf = malloc(bin_size);
    if (!buf) {
        fprintf(stderr, "malloc failed\n");
        fclose(bin);
        return 1;
    }

    fread(buf, 1, bin_size, bin);
    fclose(bin);

    // Open executable
    FILE *exe = fopen(exe_path, "r+b");
    if (!exe) {
        perror("open exe");
        free(buf);
        return 1;
    }

    uint64_t target_va = 0x4022c9;

    long file_off = va_to_offset(exe, target_va);
    if (file_off < 0) {
        fprintf(stderr, "Could not map VA to file offset\n");
        fclose(exe);
        free(buf);
        return 1;
    }

    // Write patch bytes
    fseek(exe, file_off, SEEK_SET);
    fwrite(buf, 1, bin_size, exe);

    fclose(exe);
    free(buf);
    return 0;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <executable> <patch.bin>\n", argv[0]);
        return 1;
    }

    return patch_with_bin(argv[1], argv[2]);
}
