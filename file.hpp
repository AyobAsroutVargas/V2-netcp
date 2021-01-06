/** 
 * Universidad de La Laguna
 * Asignatura: Sistemas Operativos
 * Netcp multihilo
 * @author Ayob Asrout Vargas
 * @date 29/12/2020
 * Contact: alu0101350158@ull.edu.es
 * 
 */

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <array>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <string>
#include <system_error>
#include <unistd.h>
#include<string>
#include <sys/mman.h>

namespace netcp{
  class File {
   public:
    File(const char* pathname, int flags, int file_length = 0);
    ~File();
    ssize_t read_from_file(void* buf, size_t count);
    ssize_t write_to_file(void* buf, size_t count);
    int get_fd(void);
    void* data();
    int get_file_length(void);
   private:
    int file_length_;
    void* memory_mapped_;
    int fd_;
  };
}