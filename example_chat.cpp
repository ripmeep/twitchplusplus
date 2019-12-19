#include "twitch.hpp"

packet_t packet[MAX_PACKET_SIZE];

int main(int argc, char ** argv) {
      if (!argv[1]) return -1;

      std::string streamer(argv[1]);

      TwitchUser * twitch_user = new TwitchUser();      

      net_status_t connection = twitch_user->connect_irc(TWITCH_IRC, TWITCH_PORT);
      
      if (success(connection)) {
            std::cout << "[+] Connected to Twitch IRC successfully" << std::endl;
      } else {
            std::cout << "[!] Could not connect to Twitch IRC" << std::endl;
            return -1;
      }

      if (success(twitch_user->send_login(USERNAME, OAUTH_PASS))) {
            std::cout << "[+] Login successful!" << std::endl;
      } else {
            std::cout << "[!] Login failed.... Wrong OAuth" << std::endl;
            return -1;      
      }
            
      if (success(twitch_user->join_channel(streamer.c_str()))) {
            std::cout << "[+] Joined channel" << std::endl;
            twitch_user->receive_chunk(MAX_PACKET_SIZE, packet); //receive joined channel msg

            std::cout << std::endl << "______________________________________" << std::endl << std::endl;
      } else {
            std::cout << "[!] Could not join channel " << streamer << std::endl;
            return -1;
      }

      std::string your_message;

      while(true) {
            std::cout << "\033[01;33m" << USERNAME << "\033[0m>";
            std::getline(std::cin,your_message);
            twitch_user->send_message(your_message.c_str());
      }

      std::cout << std::endl;

      return 0;
}