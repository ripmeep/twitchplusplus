#define TWITCH_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>

#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <net/if.h>

#define OAUTH_PASS     "oauth:asdfg12345asdfg12345asdfg12345"
#define USERNAME       "Testing"
#define TWITCH_IRC     "irc.twitch.tv"
#define TWITCH_PORT     6667
#define MAX_PACKET_SIZE 65535

typedef int8_t net_status_t;
typedef uint8_t packet_t;

char * hostname_to_ip(const char hostname[]) {
      struct hostent * entity = gethostbyname(hostname);
      return entity==NULL?(char*)NULL:inet_ntoa((struct in_addr)*((struct in_addr *)entity->h_addr_list[0]));
}

bool success(int code) {
      return code<0?false:true;
}

class TwitchUser {
      private:
            bool sent_channel, logged_in = false;
      public:
            int client_stream;
            char * current_channel;
            char * last_message;

            net_status_t send_message(const char message[]) {
                  if (!logged_in || !sent_channel) return -1;

                  //delete [] last_message;

                  size_t message_size = (MAX_PACKET_SIZE/16)+1;
                  char * packet = new char[message_size];
                  snprintf(packet, message_size, "PRIVMSG #%s :%s\r\n", this->current_channel, message);

                  net_status_t send_status = send(this->client_stream, packet, strlen(packet), 0);
                  
                  //last_message = new char[strlen(message)];
                  //last_message = (char*)message;

                  return send_status<0?-1:0;
            }

            int32_t receive_chunk(size_t chunk_size, packet_t * packet_arr) {
                  net_status_t rec_status = recv(this->client_stream, packet_arr, chunk_size, 0);
                  return rec_status<0?-1:rec_status;
            }

            
            net_status_t send_login(const char nickname[], const char password[]) {
                  size_t login_size = (MAX_PACKET_SIZE/16)+1;
                  char * packet = new char[login_size];
                  snprintf(packet, login_size, "PASS %s\r\n", password);
                  net_status_t send_status = send(this->client_stream, packet, strlen(packet), 0);

                  if (send_status <= 0) return -1;

                  memset(packet, '\0', login_size);
                  packet[login_size] = '\0';

                  snprintf(packet, login_size, "NICK %s\r\n", nickname);
                  send_status = send(this->client_stream, packet, strlen(packet), 0);

                  packet_t response[MAX_PACKET_SIZE];
                  if (recv(this->client_stream, response, MAX_PACKET_SIZE, 0) < 0) return -1;

                  std::string res((char*)response);
                  if (res.find("fail") != std::string::npos) return -1;

                  delete [] packet;

                  if (send_status <= 0) return -1;

                  logged_in = true;

                  return 0;
            }

            net_status_t connect_irc(const char host[], const int port) {
                  struct sockaddr_in server;

                  this->client_stream = socket(AF_INET, SOCK_STREAM, 0);

                  if (this->client_stream < 0) return -1;

                  fd_set fd;
                  timeval tv;

                  FD_ZERO(&fd);
                  FD_SET(this->client_stream, &fd);

                  tv.tv_sec = 4;
                  tv.tv_usec = 100;

                  setsockopt(this->client_stream, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv));
                  setsockopt(this->client_stream, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv, sizeof(tv));

                  server.sin_addr.s_addr = inet_addr(hostname_to_ip(host));
                  server.sin_port = htons(port);
                  server.sin_family = AF_INET;

                  net_status_t con_status = connect(this->client_stream, (struct sockaddr*)&server, sizeof(server));
                  return con_status<0?-1:0;
            }

            net_status_t join_channel(const char channel_name[]) {
                  if (!logged_in) return -1;

                  //delete [] current_channel;

                  size_t join_size = MAX_PACKET_SIZE/4;
                  char * packet = new char[join_size];
                  char resp[65535];
                  snprintf(packet, join_size, "JOIN #%s\r\n", channel_name);
                  net_status_t send_status = send(this->client_stream, packet, strlen(packet), 0);
                  this->receive_chunk(MAX_PACKET_SIZE, (packet_t*)resp);

                  std::string response(resp);

                  if (response.find("366") != std::string::npos && response.find("End of") != std::string::npos) return -1;

                  current_channel = (char*)channel_name;

                  sent_channel = true;

                  return 0;
            }      

            void disconnect() {
                  //fclose(this->client_stream);
                  return;
            }
};

class TwitchMessageParser {
      public:
            std::string username;
            std::string message;            
      public:
            void parse_message(const char message_str[]) {
                  std::string message(message_str);

                  unsigned first = message.find(message[1]);
                  unsigned last  = message.find("!");

                  this->username = message.substr(first, last-first);

                  std::string message_chunk = message;

                  first = message_chunk.find("PRIVMSG");
                  last = message_chunk.find("\r\n");
                  
                  message_chunk = message_chunk.substr(first, last-first); // PRIVMSG #channel :hello world

                  first = message_chunk.find(":");
                  last  = message_chunk.length();

                  message_chunk = message_chunk.substr(first+1, last-first);

                  this->message = message_chunk;

                  return;
            }

            static void show_message(std::string sender, std::string message) {
                  #ifndef COLORS
                        #define  SENDER_COLOR  "\033[01;36m"
                        #define  RESET         "\033[0m"
                        #define  YOU           "\033[01;33m"
                        #define  MESSGAE_COLOR "\033[01;37m"
                        #define  COLORS
                  #endif

                  char * output = new char[2048];

                  snprintf(output, 2048, "%s[%s%20s%s] %s%s", MESSGAE_COLOR, (!strcmp(USERNAME, sender.c_str()))?YOU:SENDER_COLOR, sender.c_str(), MESSGAE_COLOR, message.c_str(), RESET);

                  std::cout << output << std::endl;

                  return;
            }
};
