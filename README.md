# ![logo](https://raw.githubusercontent.com/azerothcore/azerothcore.github.io/master/images/logo-github.png) AzerothCore
## World Chat

# Description
Global Chat Module is a simple global chat for AzerothCore. Current Status is WIP at about 95% complete.
Based in part of the World Chat Module by [wizzymore](https://github.com/azerothcore/mod-world-chat) 
# Functionality
* How to chat?
    - .chat Message - This works for everyone, GM and players.

# Commands
List of fully functional commands:
* .chat <$TEXT>
  - Used to talk on the Global Chat
* . chat on
  - Used to turn on the feature
* .chat off
  - Used to turn off the feature

## Notice
Module works with one exception at the moment.
Players will still recieve the global messages when they turn off global messages.
So far the .chat off only seems to turn off sending of the global messags.

## WIP Status
Currently the only thing left to do is to specify faction only global chatting.

Fix the issue of player still recieving global messages while setting is off.

If chat is off, players are not able to send out global messages but will still recieve them.

# Installation
## Core Setup

To add the module follow the next steps:
1. Go into the folder <source_of_335>/modules
2. Clone the repository here.

On windows, open git bash and paste this command:
```
git clone https://github.com/azerothcore/mod_global_chat.git
```
On linux:

```
git clone https://github.com/azerothcore/mod_global_chat.git
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
