#include "object_base.h"

OBJECT_BASE::OBJECT_BASE()
{
    alive = true;
}

bool OBJECT_BASE::is_alive()
{
    return alive;
}

void OBJECT_BASE::kill()
{
    alive = false;
}
