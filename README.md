# twitchplusplus
A cool &amp; easy to use C++ Header(++) file for the Twitch IRC server

1) Download/Clone this repository
2) Go to https://twitchapps.com/tmi to generate your oauth key to twitch
3) Edit line 16 AND 17 in `twitch.hpp` and replace the current oauth key and username with your oauth key and username
```c++
#define OAUTH_PASS     "oauth:asdfg12345asdfg12345asdfg12345"
                    TO
#define OAUTH_PASS     "oauth:your_oauth_key_123abc"



#define USERNAME       "Testing"
                    TO
#define USERNAME       "YourTwitch_Username"
```
4) Compile
```bash
$ make clean
$ make
```
5) Test out the examples!
```bash
$ ls
chat  example_chat.cpp  example_watch.cpp  Makefile  twitch.hpp  watch
```

# Watch

```bash
Usage   $ ./watch [STREAMER NAME]
Example $ ./watch timthetatman

[+] Connected to Twitch IRC successfully
[+] Login successful!
[+] Joined channel

______________________________________

[    username1] WeirdChamp
[    username2] Hello :)
```

# Chat

```bash
Usage   $ ./chat [STREAMER NAME]
Example $ ./char timthetatman

[+] Connected to Twitch IRC successfully
[+] Login successful!
[+] Joined channel

______________________________________

your_username>hello world! #Sends message to twitch chat
your_username>
```
