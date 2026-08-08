/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#ifndef SKYFIRE_SOURCE_DIR
#define SKYFIRE_SOURCE_DIR "."
#endif

namespace
{
    std::string ReadFile(char const* relativePath)
    {
        std::ifstream file(std::string(SKYFIRE_SOURCE_DIR) + "/" + relativePath, std::ios::in | std::ios::binary);
        std::ostringstream stream;
        stream << file.rdbuf();
        return stream.str();
    }

    bool Expect(bool condition, char const* message)
    {
        if (!condition)
            std::cerr << "FAIL: " << message << std::endl;

        return condition;
    }

    bool Contains(std::string const& haystack, char const* needle)
    {
        return haystack.find(needle) != std::string::npos;
    }

    bool TestTransitionSwimFlyOpcodeMappings()
    {
        std::string const opcodesHeader = ReadFile("src/server/game/Server/Protocol/Opcodes.h");
        std::string const opcodesSource = ReadFile("src/server/game/Server/Protocol/Opcodes.cpp");
        std::string const movementSource = ReadFile("src/server/game/Movement/MovementStructures.cpp");

        bool passed = true;
        passed &= Expect(Contains(opcodesHeader, "CMSG_MOVE_SET_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY_ACK,"),
            "Transition swim/fly client ACK should exist in the CMSG opcode enum");
        passed &= Expect(Contains(opcodesSource,
            "DEFINE_OPCODE_HANDLER(CMSG_MOVE_SET_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY_ACK, 0x11DB"),
            "Transition swim/fly client ACK should use opcode 0x11DB");
        passed &= Expect(Contains(opcodesSource,
            "&WorldSession::HandleMoveSetCanTransitionBetweenSwimAndFlyAck"),
            "Transition swim/fly client ACK should have an explicit movement handler");
        passed &= Expect(Contains(opcodesSource,
            "DEFINE_OPCODE_HANDLER(SMSG_MOVE_SET_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY,   0x0A03"),
            "Transition swim/fly server set packet should use opcode 0x0A03");
        passed &= Expect(!Contains(opcodesSource,
            "DEFINE_OPCODE_HANDLER(SMSG_MOVE_SET_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY,   0x11DB"),
            "Transition swim/fly server set packet should not use the client ACK opcode");
        passed &= Expect(Contains(movementSource,
            "case SMSG_MOVE_SET_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY:\n            return MoveSetCanTransitionBetweenSwimAndFly;"),
            "Transition swim/fly server set packet should have a movement sequence");
        passed &= Expect(Contains(movementSource,
            "case SMSG_MOVE_UNSET_CAN_TRANSITION_BETWEEN_SWIM_AND_FLY:\n            return MoveUnsetCanTransitionBetweenSwimAndFly;"),
            "Transition swim/fly server unset packet should have a movement sequence");

        return passed;
    }
}

int main()
{
    bool const passed = TestTransitionSwimFlyOpcodeMappings();
    std::cout << (passed ? "Movement opcode table tests passed" : "Movement opcode table tests failed") << std::endl;
    return passed ? 0 : 1;
}
