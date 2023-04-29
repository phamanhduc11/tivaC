#include <stdint.h>
#include <stdbool.h>

enum {
    CONSOLE_MENU_LEVEL,
    CONSOLE_MENU_FLASH_LEVEL,
    CONSOLE_MENU_LEVEL_MAX,
};

enum {
    CONSOLE_TYPE,
    CONSOLE_MENU,
    CONSOLE_HANDLER,
};

void ConsoleHandler(void);
