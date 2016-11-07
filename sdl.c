extern void ZNESWrapperInitSDL();
void init_sdl(void) {
	ZNESWrapperInitSDL();
}

extern int ZNESWrapperUpdateSDL();
int sdl_update(void) {
	return ZNESWrapperUpdateSDL();
}

extern void ZNESWrapperFrameSDL();
void sdl_frame(void) {
	ZNESWrapperFrameSDL();
}
