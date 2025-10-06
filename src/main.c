#include "load.h"
#include <xcb/xcb.h>
int main(void) {

  xcb_connection_t *c;
  xcb_screen_t *screen;
  int screen_n;
  xcb_screen_iterator_t iter;
  /*
        c = xcb_connect(NULL, &screen_n);

  iter = xcb_setup_roots_iterator(xcb_get_setup(c));
  for (; iter.rem; --screen_n, xcb_screen_next(&iter))
    if (screen_n == 0) {
      screen = iter.data;
      break;
    }
  xcb_disconnect(c);
*/
  load_font("/usr/share/fonts/TTF/Roboto-Black.ttf");

  return 0;
}
