#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DEV "/dev/input/by-path/platform-i8042-serio-0-event-kbd"

int main() {
	int device = open(DEV, O_RDONLY);
	if (device == -1) {
		perror("error: ");
		return 1;
	}
	struct input_event iev;
	printf("Press any key\n==================\n");
	fflush(stdout);
	read(device, &iev, sizeof(struct input_event)); /* dump the input enter when running ./ex2 in terminal */
	read(device, &iev, sizeof(struct input_event));  
	while(1) {
		read(device, &iev, sizeof(struct input_event));
		if(iev.type != 4 && iev.code != EV_SYN) { 
			if (iev.value == 1) printf("PRESSED 0x%.4x (%d)\n", iev.code, iev.code); 
			if (iev.value == 0) printf("RELEASED 0x%.4x (%d)\n==================\n", iev.code, iev.code); 
			fflush(stdout);
		}
	}
	return 0;
}
