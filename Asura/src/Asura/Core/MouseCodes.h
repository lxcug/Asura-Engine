// @author: lx
// @time: 2022/10/14
// @brief: 

#pragma once
#ifndef GAME_MOUSECODES_H
#define GAME_MOUSECODES_H


namespace Asura {
    using MouseCode = uint16_t;

    namespace Mouse {
        enum : MouseCode {
            Button0                = 0,
            Button1                = 1,
            Button2                = 2,
            Button3                = 3,
            Button4                = 4,
            Button5                = 5,
            Button6                = 6,
            Button7                = 7,
            ButtonLast             = Button7,
            ButtonLeft             = Button0,
            ButtonRight            = Button1,
            ButtonMiddle           = Button2
        };
    }
}

#endif //GAME_MOUSECODES_H
