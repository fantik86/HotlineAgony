#pragma once
#include <unordered_map>
#include <functional>
#include <string>
#include "Player.h"

using game::living::Player;

class ChatCommands {
public:
	static std::function<void(Player&)> searchCommand(std::string command);
	static void Init();
private:
	static std::unordered_map<std::string, std::function<void(Player&)>> m_commands;
};