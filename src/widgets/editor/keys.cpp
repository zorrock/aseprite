/* ASE - Allegro Sprite Editor
 * Copyright (C) 2001-2010  David Capello
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "config.h"

#include <allegro/keyboard.h>

#include "jinete/jrect.h"
#include "jinete/jsystem.h"
#include "jinete/jview.h"
#include "jinete/jwidget.h"

#include "app.h"
#include "core/color.h"
#include "modules/editors.h"
#include "modules/gui.h"
#include "raster/image.h"
#include "raster/sprite.h"
#include "settings/settings.h"
#include "ui_context.h"
#include "widgets/colbar.h"
#include "widgets/editor.h"

bool Editor::editor_keys_toset_zoom(int scancode)
{
  if ((m_sprite) &&
      (jwidget_has_mouse(this)) &&
      !(key_shifts & (KB_SHIFT_FLAG | KB_CTRL_FLAG | KB_ALT_FLAG))) {
    JWidget view = jwidget_get_view(this);
    JRect vp = jview_get_viewport_position(view);
    int x, y, zoom;

    x = 0;
    y = 0;
    zoom = -1;

    switch (scancode) { /* TODO make these keys configurable */
      case KEY_1: zoom = 0; break;
      case KEY_2: zoom = 1; break;
      case KEY_3: zoom = 2; break;
      case KEY_4: zoom = 3; break;
      case KEY_5: zoom = 4; break;
      case KEY_6: zoom = 5; break;
    }

    /* zoom */
    if (zoom >= 0) {
      hide_drawing_cursor();
      screen_to_editor(jmouse_x(0), jmouse_y(0), &x, &y);

      x = m_offset_x - jrect_w(vp)/2 + ((1<<zoom)>>1) + (x << zoom);
      y = m_offset_y - jrect_h(vp)/2 + ((1<<zoom)>>1) + (y << zoom);

      if ((m_zoom != zoom) ||
	  (m_cursor_editor_x != (vp->x1+vp->x2)/2) ||
	  (m_cursor_editor_y != (vp->y1+vp->y2)/2)) {
	int use_refresh_region = (m_zoom == zoom) ? true: false;

	m_zoom = zoom;

	editor_update();
	editor_set_scroll(x, y, use_refresh_region);

	jmouse_set_position((vp->x1+vp->x2)/2, (vp->y1+vp->y2)/2);
	jrect_free(vp);

	show_drawing_cursor();
	return true;
      }
    }

    jrect_free(vp);
  }

  return false;
}

bool Editor::editor_keys_toset_pensize(int scancode)
{
  Tool* current_tool = UIContext::instance()->getSettings()->getCurrentTool();
  IToolSettings* tool_settings = UIContext::instance()->getSettings()->getToolSettings(current_tool);
  IPenSettings* pen = tool_settings->getPen();

  if ((m_sprite) &&
      (jwidget_has_mouse(this)) &&
      !(key_shifts & (KB_SHIFT_FLAG | KB_CTRL_FLAG | KB_ALT_FLAG))) {
    if (scancode == KEY_MINUS_PAD) { // TODO configurable keys
      if (pen->getSize() > 1) {
	pen->setSize(pen->getSize()-1);
      }
      return true;
    }
    else if (scancode == KEY_PLUS_PAD) {
      if (pen->getSize() < 32) {
	pen->setSize(pen->getSize()+1);
      }
      return true;
    }
  }

  return false;
}
