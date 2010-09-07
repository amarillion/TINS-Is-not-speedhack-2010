#ifndef TITLE_H
#define TITLE_H

#define TITLE_MAX_MENUS      32
#define TITLE_MENU_MAIN       0
#define TITLE_MENU_DIFFICULTY 1

bool title_init(void);
void title_logic(void);
void title_render(void);

#endif
