#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define _1GB (1024*1024*1024LL)

int test_read_one_byte(int fd, uint64_t offset) {
  char out;
  ssize_t char_read;

  printf("Try to read at position %li\n", offset);
  char_read = pread(fd, &out, 1, offset);
  if (char_read == -1) {
    perror("Cannot read\n");
    return -1;
  }
  if (char_read == 1) {
    printf("Byte at position %li is 0x%x.\n", offset, out);
  } else {
    printf("Cannot read but no error...\n");
  }

  return char_read;
}

int test_big_file(char* filename) {
  int fd = open(filename, O_RDONLY);
  if (fd == -1) {
    perror("Cannot open filename");
    return -1;
  }

  if (test_read_one_byte(fd, 0) == -1) {
    printf("Fail reading at positon 0\n");
    return -1;
  }


  // Read at 1Gb+1 offset
  if (test_read_one_byte(fd, _1GB+1) == -1) {
    printf("Fail reading at positon 1GB+1\n");
    return -1;
  }

  // Read at 2Gb+1 offset
  if (test_read_one_byte(fd, 2*_1GB+1) == -1) {
    printf("Fail reading at positon 2GB+1\n");
    return -1;
  }
  // Read at 3Gb+1 offset
  if (test_read_one_byte(fd, 3*_1GB+1) == -1) {
    printf("Fail reading at positon 3GB+1\n");
    return -1;
  }
  // Read at 4Gb+1 offset
  if (test_read_one_byte(fd, 4*_1GB+1) == -1) {
    printf("Fail reading at positon 4GB+1\n");
    return -1;
  }

  // Everything ok
  return 0;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Usage :\n%s <file_bigger_than_4g>\n", argv[0]);
    return -1;
  }

  return test_big_file(argv[1]);
}

