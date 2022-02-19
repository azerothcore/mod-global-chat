/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptMgr.h"
#include "Log.h"
#include "Player.h"
#include "Channel.h"
#include "Chat.h"
#include "Common.h"
#include "World.h"
#include "WorldSession.h"
#include "Config.h"
#include <unordered_map>

const char* CLASS_ICON;

#define FACTION_SPECIFIC 0

using namespace Acore::ChatCommands;

/* Config Variables */
struct GCConfig
{
    bool Enabled;
    bool Announce;
};

GCConfig GC_Config;

class GlobalChat_Config : public WorldScript
{
public: GlobalChat_Config() : WorldScript("GlobalChat_Config") { };
      void OnBeforeConfigLoad(bool reload) override
      {
          if (!reload)
          {
              GC_Config.Enabled = sConfigMgr->GetOption<bool>("GlobalChat.Enable", true);
              GC_Config.Announce = sConfigMgr->GetOption<bool>("GlobalChat.Announce", true);
          }
      }
};

/* STRUCTURE FOR GlobalChat map */
struct ChatElements
{
    uint8 chat = (GC_Config.Enabled) ? 0 : 1; // CHAT DISABLED BY DEFAULT
};

/* UNORDERED MAP FOR STORING IF CHAT IS ENABLED OR DISABLED */
std::unordered_map<uint32, ChatElements>GlobalChat;

std::string GetNameLink(Player* player)
{
    std::string name = player->GetName();
    std::string color;
    switch (player->getClass())
    {
    case CLASS_DEATH_KNIGHT:
        color = "|cffC41F3B";
        CLASS_ICON = "|TInterface\\icons\\Spell_Deathknight_ClassIcon:15|t|r";
        break;

    case CLASS_DRUID:
        color = "|cffFF7D0A";
        CLASS_ICON = "|TInterface\\icons\\Ability_Druid_Maul:15|t|r";
        break;

    case CLASS_HUNTER:
        color = "|cffABD473";
        CLASS_ICON = "|TInterface\\icons\\INV_Weapon_Bow_07:15|t|r";
        break;

    case CLASS_MAGE:
        color = "|cff69CCF0";
        CLASS_ICON = "|TInterface\\icons\\INV_Staff_13:15|t|r";
        break;

    case CLASS_PALADIN:
        color = "|cffF58CBA";
        CLASS_ICON = "|TInterface\\icons\\INV_Hammer_01:15|t|r";
        break;

    case CLASS_PRIEST:
        color = "|cffFFFFFF";
        CLASS_ICON = "|TInterface\\icons\\INV_Staff_30:15|t|r";
        break;

    case CLASS_ROGUE:
        color = "|cffFFF569";
        CLASS_ICON = "|TInterface\\icons\\INV_ThrowingKnife_04:15|t|r";
        break;

    case CLASS_SHAMAN:
        color = "|cff0070DE";
        CLASS_ICON = "|TInterface\\icons\\Spell_Nature_BloodLust:15|t|r";
        break;

    case CLASS_WARLOCK:
        color = "|cff9482C9";
        CLASS_ICON = "|TInterface\\icons\\Spell_Nature_FaerieFire:15|t|r";
        break;

    case CLASS_WARRIOR:
        color = "|cffC79C6E";
        CLASS_ICON = "|TInterface\\icons\\INV_Sword_27.png:15|t|r";
        break;

    }
    return "|Hplayer:" + name + "|h" + CLASS_ICON + "|h|cffFFFFFF[" + color + name + "|cffFFFFFF]|h|r";
}

class cs_global_chat : public CommandScript
{
public:
    cs_global_chat() : CommandScript("cs_global_chat") {}

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable gcCommandTable =
        {
            { "on",       SEC_PLAYER,     false,     &HandleGlobalChatOnCommand,        "" },
            { "off",      SEC_PLAYER,     false,     &HandleGlobalChatOffCommand,       "" },
             { "",        SEC_PLAYER,     false,     &HandleGlobalChatCommand,          "" },
        };
        static ChatCommandTable HandleGlobalChatCommandTable =
        {
            { "chat",     SEC_PLAYER,     true,      nullptr,     "",    gcCommandTable},
        };
        return HandleGlobalChatCommandTable;

    }

    static bool HandleGlobalChatOnCommand(ChatHandler* handler, const char* /*msg*/)
    {
        Player* player = handler->GetSession()->GetPlayer();
        uint64 guid = player->GetGUID().GetCounter();

        if (!GC_Config.Enabled)
        {
            ChatHandler(player->GetSession()).PSendSysMessage("Global Chat System is disabled.|r");
            return true;
        }

        if (GlobalChat[guid].chat)
        {
            ChatHandler(player->GetSession()).PSendSysMessage("Global Chat is already visible.|r");
            return true;
        }

        GlobalChat[guid].chat = 1;

        ChatHandler(player->GetSession()).PSendSysMessage("Global is now visible.|r");

        return true;
    };

    static bool HandleGlobalChatOffCommand(ChatHandler* handler, const char* /*msg*/)
    {
        Player* player = handler->GetSession()->GetPlayer();
        uint64 guid = player->GetGUID().GetCounter();

        if (!GC_Config.Enabled)
        {
            ChatHandler(player->GetSession()).PSendSysMessage("Global Chat System is disabled.|r");
            return true;
        }

        if (!GlobalChat[guid].chat)
        {
            ChatHandler(player->GetSession()).PSendSysMessage("Global Chat is already hidden.|r");
            return true;
        }

        GlobalChat[guid].chat = 0;

        ChatHandler(player->GetSession()).PSendSysMessage("Global Chat is now hidden.|r");

        return true;
    };

    static bool HandleGlobalChatCommand(ChatHandler* handler, const char* args)
    {
        if (!handler->GetSession()->GetPlayer())
            return false;
        std::string temp = args;

        if (!args || temp.find_first_not_of(' ') == std::string::npos)
            return false;

        std::string msg = "";
        Player* player = handler->GetSession()->GetPlayer();

        switch (player->GetSession()->GetSecurity())
        {
            // Player
        case SEC_PLAYER:
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                msg += "|cffABD473[Global] ";
                msg += "|cff0000ff|TInterface\\pvpframe\\pvp-currency-alliance:17|t|r ";
                msg += GetNameLink(player);
                msg += " |cfffaeb00";
            }

            else
            {
                msg += "|cffABD473[Global] ";
                msg += "|cffff0000|TInterface\\pvpframe\\pvp-currency-horde:17|t|r ";
                msg += GetNameLink(player);
                msg += " |cfffaeb00";
            }
            break;

            // Moderator
        case SEC_MODERATOR:
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                msg += "|cffABD473[Global] ";
                msg += "|TINTERFACE/CHATFRAME/UI-CHATICON-BLIZZ:15|t";
                msg += "|cff0000ff|TInterface\\pvpframe\\pvp-currency-alliance:17|t|r ";
                msg += GetNameLink(player);
                msg += " |cfffaeb00";
            }

            else
            {
                msg += "|cffABD473[Global] ";
                msg += "|TINTERFACE/CHATFRAME/UI-CHATICON-BLIZZ:15|t";
                msg += "|cffff0000|TInterface\\pvpframe\\pvp-currency-horde:17|t|r ";
                msg += GetNameLink(player);
                msg += " |cfffaeb00";
            }
            break;

            // GM
        case SEC_GAMEMASTER:
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                msg += "|cffABD473[Global] ";
                msg += "|TINTERFACE/CHATFRAME/UI-CHATICON-BLIZZ:15|t";
                msg += "|cff0000ff|TInterface\\pvpframe\\pvp-currency-alliance:17|t|r ";
                msg += GetNameLink(player);
                msg += " |cfffaeb00";
            }

            else
            {
                msg += "|cffABD473[Global] ";
                msg += "|TINTERFACE/CHATFRAME/UI-CHATICON-BLIZZ:15|t";
                msg += "|cffff0000|TInterface\\pvpframe\\pvp-currency-horde:17|t|r ";
                msg += GetNameLink(player);
                msg += " |cfffaeb00";
            }
            break;

            // Admin
        case SEC_ADMINISTRATOR:
            if (player->GetTeamId() == TEAM_ALLIANCE)
            {
                msg += "|cffABD473[Global] ";
                msg += "|TINTERFACE/CHATFRAME/UI-CHATICON-BLIZZ:15|t";
                msg += "|cff0000ff|TInterface\\pvpframe\\pvp-currency-alliance:17|t|r ";
                msg += GetNameLink(player);
                msg += " |cfffaeb00";
            }

            else
            {
                msg += "|cffABD473[Global] ";
                msg += "|TINTERFACE/CHATFRAME/UI-CHATICON-BLIZZ:15|t";
                msg += "|cffff0000|TInterface\\pvpframe\\pvp-currency-horde:17|t|r ";
                msg += GetNameLink(player);
                msg += " |cfffaeb00";
            }
            break;

        }

        if (!GC_Config.Enabled)
        {
            ChatHandler(player->GetSession()).PSendSysMessage("Global Chat System is disabled.|r");
            return false;
        }

        if (!player->CanSpeak())
        {
            ChatHandler(player->GetSession()).PSendSysMessage("You can't use World Chat while muted!|r");
            return false;
        }

        if (!GlobalChat[player->GetGUID().GetCounter()].chat)
        {
            ChatHandler(player->GetSession()).PSendSysMessage("Global Chat is hidden. (.chat off)|r");
            return false;
        }

        char message[1024];

        SessionMap sessions = sWorld->GetAllSessions();

        for (SessionMap::iterator itr = sessions.begin(); itr != sessions.end(); ++itr)
        {
            if (!itr->second)
                continue;
            if (!itr->second->GetPlayer())
            {
                continue;
            }
            if (!itr->second->GetPlayer()->IsInWorld())
            {
                continue;
            }
            msg += args;
            if (FACTION_SPECIFIC)
            {
                SessionMap sessions = sWorld->GetAllSessions();
                for (SessionMap::iterator itr = sessions.begin(); itr != sessions.end(); ++itr)
                    if (Player* plr = itr->second->GetPlayer())
                        if (plr->GetTeamId() == player->GetTeamId())
                            sWorld->SendServerMessage(SERVER_MSG_STRING, msg.c_str(), plr);
            }
            else
                sWorld->SendServerMessage(SERVER_MSG_STRING, msg.c_str(), 0);

            return true;
        }
    }
};

class GlobalChat_Announce : public PlayerScript
{
public:

    GlobalChat_Announce() : PlayerScript("GlobalChat_Announce") {}

    void OnLogin(Player* player)
    {
        // Announce Module
        if (GC_Config.Enabled && GC_Config.Announce)
        {
            ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00Azerothcore Global Chat |rmodule. Use .chat to chat globally");
        }
    }
};

void AddSC_global_chat()
{
    new GlobalChat_Config();
    new cs_global_chat();
    new GlobalChat_Announce();
}
