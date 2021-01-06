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
  netcp::Socket socket_receive("127.0.0.1", 1024);
  netcp::Message message{};
  sockaddr_in address{netcp::make_ip_address(1024, "10.0.2.15")};

  socket_receive.receive_from(message, address);
  std::string file_name_string = message.text.data();
  const char* file_name = file_name_string.c_str();

  socket_receive.receive_from(message, address);
  std::string file_length_string = message.text.data();
  int file_length = std::stoi(file_length_string);

  netcp::File file_receive(file_name, O_RDWR, file_length);

  int bytes_received = 0;
  int remaining_bytes = file_length;

  while (bytes_received < file_length) {
    if (remaining_bytes < 4096) {
      socket_receive.receive_from(file_receive.data() + bytes_received, remaining_bytes, address);
      bytes_received += remaining_bytes;
      remaining_bytes -= remaining_bytes;
    } else {
      socket_receive.receive_from(file_receive.data() + bytes_received, 4096, address);
      bytes_received += 4096;
      remaining_bytes -= 4096;
    }
  }

//   while (1 == 1) {
//     socket_receive.receive_from(message, address);
//     std::cout << message.text.data();
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