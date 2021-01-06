#include "file.hpp"

netcp::File::File(const char* pathname, int flags, int file_length) {
  fd_ = open(pathname, flags);
  if (fd_ < 0) {
    throw std::system_error(errno, std::system_category(),
    "No se pudo abrir el archivo"); 
  }

  lockf(fd_, F_LOCK, 0);

  if (flags == O_RDONLY) {
    memory_mapped_ = mmap(
      NULL,
      file_length_,
      PROT_READ,
      MAP_SHARED,
      fd_,
      0
    );
  } else {
    int result = ftruncate(fd_, file_length);
    if (result < 0) {
      throw std::system_error(errno, std::system_category(),
      "No se pudo truncar el tamaño del archivo"); 
    }
    file_length_ = file_length;

    memory_mapped_ = mmap(
      NULL,
      file_length_,
      PROT_WRITE,
      MAP_SHARED,
      fd_,
      0
    );
  }

  if (memory_mapped_ == MAP_FAILED) {
    throw std::system_error(errno, std::system_category(),
    "Falló el mapeado del archivo en memoria");
  }
  
  
}

int netcp::File::get_fd(void) {
  return fd_;
}

void* netcp::File::data() {
  return memory_mapped_;
}

netcp::File::~File() {
  lockf(fd_, F_ULOCK, 0);
  int result = close(fd_);
  if (result < 0) {
    throw std::system_error(errno, std::system_category(),
    "No se pudo cerrar el archivo"); 
  }

  int result_memory_unmap = munmap(memory_mapped_, file_length_);
  if (result_memory_unmap < 0) {
    throw std::system_error(errno, std::system_category(),
    "Falló el munmap del archivo"); 
  }
}

ssize_t netcp::File::read_from_file(void* buf, size_t count) {
  return read(fd_, buf, count);
}

ssize_t netcp::File::write_to_file(void* buf, size_t count) {
  return write(fd_, buf, count);
}