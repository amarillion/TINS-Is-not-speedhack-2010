/* Super Battle Thor II, TINS 2010
 * Copyright (C) 2010  Johnathan Roatch
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * email: jroatch@gmail.com
 */

#include <stdio.h>
#include <allegro5/allegro.h>
#include "global_vars.h"
#include "initializations.h"
#include "intro_screen.h"
#include "main_game.h"


int
main ( /*int argc, char *argv[] */ )
{
  if (!initialize ())
    {
      return 1;
    }

  run_into_screen ();

  run_main_game ();

  deinitialize ();

  return 0;
}

