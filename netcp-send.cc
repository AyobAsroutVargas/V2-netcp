/** 
 * Universidad de La Laguna
 * Asignatura: Sistemas Operativos
 * Netcp multihilo
 * @author Ayob Asrout Vargas
 * @date 29/12/2020
 * Contact: alu0101350158@ull.edu.es
 * 
*/

#include "socket.hpp"
#include "file.hpp"

int protected_main() {
  netcp::Socket socket_send("10.0.2.15", 1024);
  char* filename = "prueba.txt";
  netcp::File file_send(filename, O_RDONLY, 0);

  netcp::Message message;
  message.text.fill('\0');
  size_t count = 1023;
  sockaddr_in address{netcp::make_ip_address(1024, "127.0.0.1")};

  socket_send.send_to("prueba1.txt", address);

  int file_length = lseek( file_send.get_fd(), 0, SEEK_END );
  std::string string_file_length = std::to_string(file_length);
  socket_send.send_to(string_file_length, address);
  int file_start = lseek( file_send.get_fd(), 0, SEEK_SET );

  int bytes_sent = 0;
  int remaining_bytes = file_length;
  while (bytes_sent < file_length) {
    if (remaining_bytes < 4096) {
      socket_send.send_to(file_send.data() + bytes_sent, remaining_bytes, address);
      bytes_sent += remaining_bytes;
      remaining_bytes -= remaining_bytes;
    } else {
      socket_send.send_to(file_send.data() + bytes_sent, 4096, address);
      bytes_sent += 4096;
      remaining_bytes -= 4096;
    }
  }

//   while ( file_send.read_from_file(&message.text, count) != 0 ) {
//     message.text[1023] = '\0';
//     socket_send.send_to(message,address);
//     message.text.fill('\0');
//   }
  return 0;
}

int main() {
  try {
    return protected_main();
  }    
  catch(std::bad_alloc& e) {
    std::cerr << "mytalk" << ": memoria insuficiente\n";
    return 1;
  }
  catch(std::system_error& e) {
    std::cerr << "mitalk" << ": " << e.what() << '\n';
    return 2;
  }
  catch (...) {
    std::cout << "Error desconocido\n";
    return 99;
  }
}