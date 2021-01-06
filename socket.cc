/** 
 * Universidad de La Laguna
 * Asignatura: Sistemas Operativos
 * Primer prototipo netcp
 * @author Ayob Asrout Vargas
 * @date 14/12/2020
 * Contact: alu0101350158@ull.edu.es
 * 
 * Namespace que contiene todo lo necesario para la ejecucion tanto de netcp-send
 * como de netcp-receive
 */

#include "socket.hpp"

void netcp::Socket::create_socket(const sockaddr_in& address) {
  //creamos el socket
  fd_ = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd_ < 0) {
    throw std::system_error(errno, std::system_category(),
    "no se pudo crear el socket");
  }

  //le asignamos una direccion
  int bind_result = bind(fd_, reinterpret_cast<const sockaddr*>(&address), sizeof(address));
  if (bind_result < 0) {
    throw std::system_error(errno, std::system_category(),
    "falló el bind del socket");
  }
}

netcp::Socket::Socket(const std::string& ip_address, int port) {
  sockaddr_in address {make_ip_address(port, ip_address)};
  create_socket(address);
}

netcp::Socket::~Socket() {
  close(fd_);
}

void netcp::Socket::send_to(const Message& message, const sockaddr_in& address) {
  int result = sendto(fd_, &message, sizeof(message), 0,
  reinterpret_cast<const sockaddr*>(&address),sizeof(address));

  if (result < 0) {
    throw std::system_error(errno, std::system_category(),
    "falló el sendto");
  }
}

void netcp::Socket::send_to(const void * message, int size, const sockaddr_in& address) {
  int result = sendto(fd_, message, size, 0,
  reinterpret_cast<const sockaddr*>(&address),sizeof(address));

  if (result < 0) {
    throw std::system_error(errno, std::system_category(),
    "falló el sendto");
  }
}

void netcp::Socket::send_to(const std::string& message, const sockaddr_in& address) {
  Message message_text;
  message_text.text.fill('\0');
  message.copy(message_text.text.data(), message_text.text.size() -1, 0);
  send_to(message_text, address);
}

void netcp::Socket::receive_from(Message& message, sockaddr_in& address) {
  socklen_t src_len = sizeof(address);

  int result = recvfrom(fd_, &message, sizeof(message), 0, reinterpret_cast<sockaddr*>(&address), &src_len);
  if (result < 0) {
    throw std::system_error(errno, std::system_category(),
    "falló recvfrom: ");
  }
 char* remote_ip = inet_ntoa(address.sin_addr);
 int remote_port = ntohs(address.sin_port);
  message.text[1023] = '\0';
  std::cout << "El sistema " << remote_ip << ":" << remote_port <<
  " envió el mensaje:\n'" << message.text.data() << "'\n";
}

void netcp::Socket::receive_from(void * message, int size, sockaddr_in& address) {
  socklen_t src_len = sizeof(address);

  int result = recvfrom(fd_, message, size, 0, reinterpret_cast<sockaddr*>(&address), &src_len);
  if (result < 0) {
    throw std::system_error(errno, std::system_category(),
    "falló recvfrom: ");
  }
}

sockaddr_in netcp::make_ip_address(int port, const std::string& ip_address_input) {
  if ((port < 0) || (65525  < port)) {
    throw std::system_error(errno, std::system_category(),
    "Puerto no válido");
  }
  sockaddr_in result_address{};
  result_address.sin_family = AF_INET;

  if (ip_address_input == "kk") {
    result_address.sin_addr.s_addr = htonl(INADDR_ANY);
  } else {
    int ip_string_length = ip_address_input.length();
    char ip_address_char [ip_string_length + 1];
    strcpy(ip_address_char, ip_address_input.c_str()); // Pasamos la direccion ip de string a char[]

    in_addr ip_address;

    int error = inet_aton(ip_address_char, &ip_address);// Convertimos la cadena de input en enteros de 32 bits

    if (error == 0) {
      throw std::system_error(errno, std::system_category(),
      "direccion ip no valida");
    }

    result_address.sin_addr.s_addr = ip_address.s_addr;
  }
  result_address.sin_port = htons(port);
  
  return result_address;
}