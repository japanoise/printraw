#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<termios.h>
#include<unistd.h>

#define ESC "\033"
#define CSI ESC"["
#define CRLF "\r\n"
#define ISCTRL(c) ((0 < c && c < 0x20) || c == 0x7f)

struct termios orig_termios;

void die(const char *s) {
	perror(s);
	exit(1);
}

void disableRawMode() {
	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios)==-1)
		die("disableRawMode tcsetattr");
}

void enableRawMode() {
	if(tcgetattr(STDIN_FILENO, &orig_termios)==-1)
		die("tcgetattr");
	atexit(disableRawMode);

	struct termios raw = orig_termios;
	raw.c_iflag &= ~(BRKINT|ICRNL|INPCK|ISTRIP|IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO|ICANON|IEXTEN|ISIG);
	if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw)==-1)
		die("enableRawMode tcsetattr");
}

int main(int argc, char *argv[]) {
	enableRawMode();
	printf("Press q to quit"CRLF"Dec Hex  Oct"CRLF);
	unsigned char c;
	while (1) {
		if(read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN)
			die("read");
		if (c == 127) {
			printf("%3d 0x%02x 0%03o ('"CSI"7m^?"CSI"m')"CRLF, c, c, c);
		} else if (ISCTRL(c)) {
			printf("%3d 0x%02x 0%03o ('"CSI"7m^%c"CSI"m')"CRLF, c, c, c, c|0x40);
		} else if (c < 127) {
			printf("%3d 0x%02x 0%03o ('%c')"CRLF, c, c, c, c);
		} else {
			printf("%3d 0x%02x 0%03o"CRLF, c, c, c);
		}
		if (c == 'q') break;
	}
	return 0;
}

