#ifndef GLFWKEYCODECONVERTER_H
#define GLFWKEYCODECONVERTER_H

#include "RenderInclude.h"
#include <optional>
#include <unordered_map>
#include <functional>

std::optional<std::string> fromKeyCode(int keycode) {
    static const std::unordered_map<int,std::function<void(std::string&)>> keyMap{
        {GLFW_KEY_0, [](std::string& result){ result = "0"; }},
        {GLFW_KEY_1, [](std::string& result){ result = "1"; }},
        {GLFW_KEY_2, [](std::string& result){ result = "2"; }},
        {GLFW_KEY_3, [](std::string& result){ result = "3"; }},
        {GLFW_KEY_4, [](std::string& result){ result = "4"; }},
        {GLFW_KEY_5, [](std::string& result){ result = "5"; }},
        {GLFW_KEY_6, [](std::string& result){ result = "6"; }},
        {GLFW_KEY_7, [](std::string& result){ result = "7"; }},
        {GLFW_KEY_8, [](std::string& result){ result = "8"; }},
        {GLFW_KEY_9, [](std::string& result){ result = "9"; }},
        {GLFW_KEY_A, [](std::string& result){ result = "a"; }},
        {GLFW_KEY_B, [](std::string& result){ result = "b"; }},
        {GLFW_KEY_C, [](std::string& result){ result = "c"; }},
        {GLFW_KEY_D, [](std::string& result){ result = "d"; }},
        {GLFW_KEY_E, [](std::string& result){ result = "e"; }},
        {GLFW_KEY_F, [](std::string& result){ result = "f"; }},
        {GLFW_KEY_G, [](std::string& result){ result = "g"; }},
        {GLFW_KEY_H, [](std::string& result){ result = "h"; }},
        {GLFW_KEY_I, [](std::string& result){ result = "i"; }},
        {GLFW_KEY_J, [](std::string& result){ result = "j"; }},
        {GLFW_KEY_K, [](std::string& result){ result = "k"; }},
        {GLFW_KEY_L, [](std::string& result){ result = "l"; }},
        {GLFW_KEY_M, [](std::string& result){ result = "m"; }},
        {GLFW_KEY_N, [](std::string& result){ result = "n"; }},
        {GLFW_KEY_O, [](std::string& result){ result = "o"; }},
        {GLFW_KEY_P, [](std::string& result){ result = "p"; }},
        {GLFW_KEY_Q, [](std::string& result){ result = "q"; }},
        {GLFW_KEY_R, [](std::string& result){ result = "r"; }},
        {GLFW_KEY_S, [](std::string& result){ result = "s"; }},
        {GLFW_KEY_T, [](std::string& result){ result = "t"; }},
        {GLFW_KEY_U, [](std::string& result){ result = "u"; }},
        {GLFW_KEY_V, [](std::string& result){ result = "v"; }},
        {GLFW_KEY_W, [](std::string& result){ result = "w"; }},
        {GLFW_KEY_X, [](std::string& result){ result = "x"; }},
        {GLFW_KEY_Y, [](std::string& result){ result = "y"; }},
        {GLFW_KEY_Z, [](std::string& result){ result = "z"; }},
        {GLFW_KEY_SPACE, [](std::string& result){ result = " "; }},
        {GLFW_KEY_LEFT_SHIFT, [](std::string& result){ result = "LEFT_SHIFT"; }},
        {GLFW_KEY_RIGHT_SHIFT, [](std::string& result){ result = "RIGHT_SHIFT"; }} 
    };
    static const auto end = keyMap.end();
    auto it = keyMap.find(keycode);

    std::optional<std::string> result;
    std::string resultBuffer = "";

    if (it != end) {
        it->second(resultBuffer);
        result = resultBuffer;
    } else {
        result = std::nullopt;
    }
    return result;
}

std::optional<int> toKeyCode(std::string c) {
    static const std::unordered_map<std::string,std::function<void(int&)>> keyMap{
        {"0", [](int& result){ result = GLFW_KEY_0; }},
        {"1", [](int& result){ result = GLFW_KEY_1; }},
        {"2", [](int& result){ result = GLFW_KEY_2; }},
        {"3", [](int& result){ result = GLFW_KEY_3; }},
        {"4", [](int& result){ result = GLFW_KEY_4; }},
        {"5", [](int& result){ result = GLFW_KEY_5; }},
        {"6", [](int& result){ result = GLFW_KEY_6; }},
        {"7", [](int& result){ result = GLFW_KEY_7; }},
        {"8", [](int& result){ result = GLFW_KEY_8; }},
        {"9", [](int& result){ result = GLFW_KEY_9; }},
        {"a", [](int& result){ result = GLFW_KEY_A; }},
        {"b", [](int& result){ result = GLFW_KEY_B; }},
        {"c", [](int& result){ result = GLFW_KEY_C; }},
        {"d", [](int& result){ result = GLFW_KEY_D; }},
        {"e", [](int& result){ result = GLFW_KEY_E; }},
        {"f", [](int& result){ result = GLFW_KEY_F; }},
        {"g", [](int& result){ result = GLFW_KEY_G; }},
        {"h", [](int& result){ result = GLFW_KEY_H; }},
        {"i", [](int& result){ result = GLFW_KEY_I; }},
        {"j", [](int& result){ result = GLFW_KEY_J; }},
        {"k", [](int& result){ result = GLFW_KEY_K; }},
        {"l", [](int& result){ result = GLFW_KEY_L; }},
        {"m", [](int& result){ result = GLFW_KEY_M; }},
        {"n", [](int& result){ result = GLFW_KEY_N; }},
        {"o", [](int& result){ result = GLFW_KEY_O; }},
        {"p", [](int& result){ result = GLFW_KEY_P; }},
        {"q", [](int& result){ result = GLFW_KEY_Q; }},
        {"r", [](int& result){ result = GLFW_KEY_R; }},
        {"s", [](int& result){ result = GLFW_KEY_S; }},
        {"t", [](int& result){ result = GLFW_KEY_T; }},
        {"u", [](int& result){ result = GLFW_KEY_U; }},
        {"v", [](int& result){ result = GLFW_KEY_V; }},
        {"w", [](int& result){ result = GLFW_KEY_W; }},
        {"x", [](int& result){ result = GLFW_KEY_X; }},
        {"y", [](int& result){ result = GLFW_KEY_Y; }},
        {"z", [](int& result){ result = GLFW_KEY_Z; }},
        {" ", [](int& result){ result = GLFW_KEY_SPACE; }},
        {"LEFT_SHIFT", [](int& result){ result = GLFW_KEY_LEFT_SHIFT; }},
        {"RIGHT_SHIFT", [](int& result){ result = GLFW_KEY_RIGHT_SHIFT; }} 
    };
    static const auto end = keyMap.end();
    auto it = keyMap.find(c);

    std::optional<int> result;
    int resultBuffer;

    if (it != end) {
        it->second(resultBuffer);
        result = resultBuffer;
    } else {
        result = std::nullopt;
    }
    return result;
}
#endif