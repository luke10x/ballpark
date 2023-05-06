/*
 * Entry point of the application.
 * Delegate away everything from this file.
 * Main loop is the core of this application,
 * please delegate all logic to the main loop.
 */
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "ctx.c"

/*
 * This context holds data between cycles
 */
ctx_t* ctx;

/*
 * one iteration of the main loop of the application
 */
void main_loop() {
  ctx_perform_one_cycle(ctx);
#ifdef __EMSCRIPTEN__
  if (ctx->should_continue == 0) {
    // After this iteration the application should exit
    emscripten_cancel_main_loop();
    ctx_delete(ctx);
  }
#endif
}

/*
 * the entry point of the application
 * 
 * This function initializes the context struct,
 * and starts the main loop.
 */
int main() {
  ctx = ctx_create();
#ifdef __EMSCRIPTEN__
  int fps = 0;
  emscripten_set_main_loop(main_loop, fps, 1);
#else
  while (ctx->should_continue != 0){
    main_loop();
  }
  ctx_delete(ctx);
#endif
// Attention, do not add anything bellow
// because in Emscripten it cannot reach this point.
}
