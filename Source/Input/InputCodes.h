#pragma once

namespace RayEngine {
    enum class KeyCode : int {
        // Alphanumeric keys
        A = 65, B = 66, C = 67, D = 68, E = 69, F = 70, G = 71, H = 72, I = 73, J = 74,
        K = 75, L = 76, M = 77, N = 78, O = 79, P = 80, Q = 81, R = 82, S = 83, T = 84,
        U = 85, V = 86, W = 87, X = 88, Y = 89, Z = 90,

        // Numbers
        Zero = 48, One = 49, Two = 50, Three = 51, Four = 52,
        Five = 53, Six = 54, Seven = 55, Eight = 56, Nine = 57,

        // Function keys
        F1 = 290, F2 = 291, F3 = 292, F4 = 293, F5 = 294, F6 = 295,
        F7 = 296, F8 = 297, F9 = 298, F10 = 299, F11 = 300, F12 = 301,

        // Special keys
        Space = 32, Escape = 256, Enter = 257, Tab = 258, Backspace = 259, Insert = 260,
        Delete = 261, Right = 262, Left = 263, Down = 264, Up = 265, PageUp = 266,
        PageDown = 267, Home = 268, End = 269, CapsLock = 280, ScrollLock = 281,
        NumLock = 282, PrintScreen = 283, Pause = 284, LeftShift = 340, LeftControl = 341,
        LeftAlt = 342, LeftSuper = 343, RightShift = 344, RightControl = 345, RightAlt = 346,
        RightSuper = 347, Menu = 348,

        // Keypad keys
        KP0 = 320, KP1 = 321, KP2 = 322, KP3 = 323, KP4 = 324,
        KP5 = 325, KP6 = 326, KP7 = 327, KP8 = 328, KP9 = 329,
        KPDecimal = 330, KPDivide = 331, KPMultiply = 332,
        KPSubtract = 333, KPAdd = 334, KPEnter = 335, KPEqual = 336,

        // Android keys
        Back = 4, Menu_Android = 82, VolumeUp = 24, VolumeDown = 25
    };

    enum class MouseButton : int {
        Left = 0,
        Right = 1,
        Middle = 2,
        Side = 3,
        Extra = 4,
        Forward = 5,
        Back = 6
    };
}