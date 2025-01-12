// modified from https://github.com/BushRobotics/replay.h

#include <cstdlib>
#include <stdio.h>


typedef struct { 
	double leftWheels[4]; // wheels
    double rightWheels[4]; // wheels
	double intake; // intake
    double conveyor; // conveyor
    bool arm; // arm
    bool grabber; // goal  grabber
	
	int last; // indicate last step 
} ReplayStep;


void write_replay(ReplayStep *step, char* filename) {
	FILE *f = fopen(filename, "wb");
	for (ReplayStep *i = step; (i-1)->last != 1; i++) {
		fwrite(i, sizeof(ReplayStep), 1, f);
	}
	fclose(f);
}

ReplayStep *read_replay(char* filename) {
	FILE *f = fopen(filename, "rb");
	
	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	fseek(f, 0, SEEK_SET);
	
	ReplayStep* replay = (ReplayStep*) malloc(size); 
	ReplayStep currentstep;

	int i = 0;
	while (fread(&currentstep, sizeof(ReplayStep), 1, f)) {
        // yes i know how stupid this is its midnight before competition leave me alone
		replay[i].leftWheels[0] = currentstep.leftWheels[0];
        replay[i].leftWheels[1] = currentstep.leftWheels[1];
        replay[i].leftWheels[2] = currentstep.leftWheels[2];
		replay[i].leftWheels[3] = currentstep.leftWheels[3];
        replay[i].rightWheels[0] = currentstep.leftWheels[0];
        replay[i].rightWheels[1] = currentstep.rightWheels[1];
		replay[i].rightWheels[2] = currentstep.rightWheels[2];
		replay[i].rightWheels[3] = currentstep.rightWheels[3];
        replay[i].intake = currentstep.intake;
        replay[i].conveyor = currentstep.conveyor;
		replay[i].arm = currentstep.arm;
        replay[i].grabber = currentstep.grabber;

        replay[i].last = 1;
        
        if(i > 0) {
            replay[i - 1].last = 0; // set prev last bool to zero so we know it wasn't the last one
        }
		i++;
	}
	replay[i].last = 1;
	
	fclose(f);
	return replay;
}