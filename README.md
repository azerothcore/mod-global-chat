# ![logo](https://raw.githubusercontent.com/azerothcore/azerothcore.github.io/master/images/logo-github.png) AzerothCore
## World Chat
- Latest build status with azerothcore: [![Build Status](https://github.com/azerothcore/mod-global-chat/workflows/core-build/badge.svg?branch=master&event=push)](https://github.com/azerothcore/mod-global-chat)

# Description
Global Chat Module is a simple global chat for AzerothCore.

# Functionality
* How to chat?
    - .chat Message - This works for everyone, GM and players.
    - /join World and talk as in a normal chat ( This name is subject of change based on the config file )

# Commands
List of fully functional commands:
* .chat <$TEXT>
  - Used to talk on the World Chat
  
# Installation
## Core Setup

To add the module follow the next steps:
1. Go into the folder <source_of_335>/modules
2. Clone the repository here.

On windows, open git bash and paste this command:
```
git clone https://github.com/wizzymore/mod_global_chat.git
```
On linux:

```
git clone https://github.com/wizzymore/mod_global_chat.git
```

## Server Config Setup
### On Windows
Modify the config : "GlobalChat.conf" by your needs.
Edit the settings in the .conf file but keep both!

### On Linux
Navigate to /etc/ folder from your azeroth build files and execute this command:
```bash
cp GlobalChat.conf.dist GlobalChat.conf
```
Modify the config : "GlobalChat.conf" by your needs.
Edit the settings in the .conf file but keep both!
## Start the server and enjoy
Done, you are ready to use the Global Chat System! Go online and try it out!
