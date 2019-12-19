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
            std::cout << "[!] Login failed.... Something went wrong! Check connetion and login details" << std::endl;
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

      TwitchMessageParser * parser = new TwitchMessageParser();

      while (true) {
            for (int i = 0; i < MAX_PACKET_SIZE; ++i) packet[i] = 0;

            twitch_user->receive_chunk(MAX_PACKET_SIZE, packet); /* RECEIVE MESSAGE */
            parser->parse_message((char*)packet);                /*  PARSE MESSAGE  */

            TwitchMessageParser::show_message(parser->username, parser->message);
      }
      
      std::cout << std::endl;

      return 0;
}
