// #include "HID.hpp"

// namespace HID
// {
//     void Interface::lock(void)
//     {
//         Keyboard.set_modifier(KEY_LEFT_GUI);
//         Keyboard.set_key1(KEY_L);
//         Keyboard.send_now();

//         this->end();
//     }

//     void Interface::unlock(String password)
//     {
//         Keyboard.set_key1(KEY_ENTER);
//         Keyboard.send_now();

//         delay(100);

//         Keyboard.print(password);

//         delay(100);

//         Keyboard.set_key1(KEY_ENTER);
//         Keyboard.send_now();

//         this->end();
//     }

//     void Interface::end(void)
//     {
//         Keyboard.set_modifier(0);
//         Keyboard.set_key1(0);
//         Keyboard.send_now();
//     }
// }
