#include "raylib.h"
#include "ChatCommands.h"

std::unordered_map<std::string, std::function<void(Player&)>> ChatCommands::m_commands = {};

void testCommand(Player& sender) {
	TraceLog(8, "Testen");
}

void playerInfo(Player& sender) {
	TraceLog(8, "Here lies player info :D");
}

void ChatCommands::Init() {
	m_commands["/TEST"] = testCommand;
	m_commands["/PLR"] = playerInfo;
}

std::function<void(Player&)> ChatCommands::searchCommand(std::string command) {
	auto& find_command = m_commands.find(command);
	if (find_command != m_commands.end()) {
		return (*find_command).second;
	}
	else {
		return nullptr;
	}
}