#include <stub.h>
#define FA1
#include "fa.h"

#define Zahl 180
#define sleep_turn 300
#define sleep_U_turn 1500
#define analog_left 0
#define analog_right 2
#define light_limit 40
#define timer 120000

//RoutePlanning defines
#define SIZE 70

void followTheLine(int power);
void forward();
void left();
void right();
void U_turn();
void Gate();

//RoutePlanning functions
void printInstructions();
int isQueueEmpty();
void enqueue(int num);
int dequeue();
void markVisited(int num);
void getNeighbours(int index);
int contains(int* array, int element, int array_length);
int mapContainsF();
void clearArray(int* array, int size_of_array);
int BFS(int startNode);
void reconstructPath(int* parent, int startNode, int destinationNode);
void reserveArray(int* array);
void pathToInstructionsToF();
void driveBackInstructions();
void getStartPosition();
void changeTurningSound();
void checkTimer();

int index = 0;
int exit_loop = 0;
int exit_loop_1 = 0;
int gateLock = 0;
int isDrivingBack = 0;
char instructions[SIZE];

//RoutePlanning variables
//Map--------------------------------------------------------------------------------
char* map;
int startPosition = 68;
//--------------------------------------------------------------------------------
int queue[SIZE];
int enqueue_index = 0;
int dequeue_index = 0;
int visited[SIZE];
int visited_index = 0;
int parent[SIZE];
int map_length = SIZE;
int neighbours[4];
int pre_path[SIZE];
int path[SIZE];
int FToDelete;
int FNotFound;

//Hauptprogrammroutine
void getStartPosition() {
    if (dip_pin(0)) {
        startPosition = 64;
    }
}

void AksenMain(void) {
    int light = 255;
    map = _fa;

    getStartPosition();

    //Loop for waiting for the light.

    while(light > light_limit)  {
        light = (analog(8));
        lcd_ubyte(light);
        lcd_puts(" > ");
        lcd_ubyte(light_limit);
        lcd_puts("; Map: ");
        lcd_ubyte(_fa_nr);

        sleep(350);
        lcd_cls();
    }
    clear_time();

    //Main loop
    motor_richtung(3, 0);
    motor_pwm(3, 10);

	while (1) {
            
            lcd_cls();
            lcd_puts("Searching Route");

			FNotFound = BFS(startPosition);
			pathToInstructionsToF();
            printInstructions();
            gateLock = 0;
		while(FNotFound == 0) {

            checkTimer();

			//Gate Activation
			if (digital_in(0)== 0 && gateLock == 0){

				driveBackInstructions();
                lcd_cls();
                printInstructions();
				isDrivingBack = 1;

				index = 0;
				gateLock = 1;
				motor_pwm(0, 0);
				motor_pwm(1, 0);
				Gate();
			}
			//If both are black, decide the what to do next.
			else if ((analog(0) > Zahl) && (analog(2) > Zahl)) {

				if (instructions[index] == 'F') {
                    index++;
                    lcd_cls();
					forward();
                    printInstructions();
					
				}
				
				else if (instructions[index] == 'L') {
                    index++;
					left();
                    lcd_cls();
                    printInstructions();
					
				}

				else if (instructions[index] == 'R'){
                    index++;
					right();
                    lcd_cls();
                    printInstructions();
					
					
				}
				else if (instructions[index] == 'U'){
                    index++;
					U_turn();
                    lcd_cls();
					printInstructions();

				}
			} else {

                if (instructions[index] == 'F' && instructions[index + 1] == 'F') {
                    followTheLine(4);
                } else {
                    followTheLine(3);
                }
				

                if (instructions[index] == '\0' && isDrivingBack == 0) {
                    int button = 1;
                    while(button == 1) {

                        followTheLine(2);
                        checkTimer();
                        button = digital_in(0);
                    }
                }

                if (instructions[index] == '\0' && isDrivingBack == 1) {
                    int i;

                    forward();

                    for (i = 0; i < 3000; i++) {
                        followTheLine(2);
                    }

					U_turn();
                    motor_pwm(0, 0);
                    motor_pwm(1, 0);

					isDrivingBack = 0;
                    index = 0;
					break;
				}
			}
		}

		if (FNotFound == 1) {
			lcd_cls();
			lcd_puts("End of the Drive!");
			motor_richtung(0, 0);
			motor_pwm(0, 0);
			motor_richtung(1, 0);
			motor_pwm(1,0);
            motor_pwm(3, 0);
			while (1) {

			}
		}
		//Delete F from the map.
		map[FToDelete] = 'x';
	}
}

void followTheLine(int power) {
	// Drive forward if white
	if ((analog(0) < Zahl) && (analog(2) < Zahl)) {
		motor_richtung(0, 0);
		motor_pwm(0, power);
		motor_richtung(1, 0);
		motor_pwm(1, power);
	}
	//Turn left a bit
	else if ((analog(analog_right) > Zahl) && (analog(analog_left) < Zahl)) {
		motor_richtung(0, 0);
		motor_pwm(0, 0);
		motor_richtung(1, 0);
		motor_pwm(1, power);
	}
	//Turn right a bit
	else if ((analog(analog_right) < Zahl) && (analog(analog_left) > Zahl)) {
		motor_richtung(0, 0);
		motor_pwm(0, power);
		motor_richtung(1, 0);
		motor_pwm(1, 0);
	}
}

void left(){
    changeTurningSound();

	exit_loop = 0;

    motor_richtung(0, 0);
	motor_pwm(0, 3);
	motor_richtung(1, 0);
	motor_pwm(1, 3);
    sleep(580);

	motor_richtung(0, 0);
	motor_pwm(0, 2);
	motor_richtung(1, 1);
	motor_pwm(1, 2);
	sleep(sleep_turn);

	while (exit_loop < Zahl) {
		exit_loop = analog(analog_left);
	}

    motor_pwm(3, 10);
}

void right(){
    changeTurningSound();

	exit_loop = 0;

    motor_richtung(0, 0);
	motor_pwm(0, 3);
	motor_richtung(1, 0);
	motor_pwm(1, 3);
    sleep(580);

	motor_richtung(0, 1);
	motor_pwm(0, 2);
	motor_richtung(1, 0);
	motor_pwm(1, 2);
	sleep(sleep_turn);

	while (exit_loop < Zahl) {
		exit_loop = analog(analog_right);
	}

    motor_pwm(3, 10);
}

void forward() {
    exit_loop = 255;
    exit_loop_1 = 255;
    lcd_puts("FORWARD");

    while (exit_loop > Zahl && exit_loop_1 > Zahl) {
        exit_loop = analog(analog_left);
		exit_loop_1 = analog(analog_right);

        motor_richtung(0, 0);
	    motor_pwm(0, 3);
	    motor_richtung(1, 0);
	    motor_pwm(1, 3);
	    
    }
    sleep(70);
    lcd_cls();
}

void U_turn() {
    checkTimer();

	exit_loop = 0;

	motor_richtung(0, 0);
	motor_pwm(0, 2);
	motor_richtung(1, 1);
	motor_pwm(1, 2);

	sleep(sleep_U_turn);
	checkTimer();
	while (exit_loop < Zahl) {
		exit_loop = analog(analog_left);
	}
}

void Gate() {
	exit_loop = 0;
	exit_loop_1 = 0;
	motor_richtung(2, 0);
	motor_pwm(2, 2);
	sleep(600);

	motor_richtung(2, 1);
	motor_pwm(2, 2);
	sleep(680);

    checkTimer();

	motor_richtung(2, 1);
	motor_pwm(2, 0);

	lcd_cls();
	lcd_puts("Backwards");
		
	//Drive backwards till next crossroad
	while((exit_loop < Zahl) || (exit_loop_1 < Zahl)) {
		exit_loop = analog(analog_left);
		exit_loop_1 = analog(analog_right);

        motor_richtung(0, 1);
		motor_pwm(0, 2);
		motor_richtung(1, 1);
		motor_pwm(1, 2);

        checkTimer();
	}
}

void changeTurningSound() {
    motor_pwm(3, 2);
}

void checkTimer() {
    if(akt_time() > timer) {
        lcd_cls();
        lcd_puts("Time ran out!");
        motor_richtung(0, 0);
        motor_pwm(0, 0);
        motor_richtung(1, 0);
        motor_pwm(1,0);
        motor_pwm(3, 0);
        while (1) {

        }
    }
}
//RoutePlanning--------------------------------------------------------------------------------

// Helpful functions for visualisation

void printInstructions() {
    int i;
    char num;

    lcd_cls();

    for(i = index; instructions[i] != 0; i++) {
        num = instructions[i];

        switch (num) {
            case 'F':
                lcd_puts("F");
                break;

            case 'L':
                lcd_puts("L");
                break;

            case 'R':
                lcd_puts("R");
                break;

            case 'U':
                lcd_puts("U");
                break;

            default:
                lcd_puts("0");
                break;
        }
    }
}

//Queue Logic

int isQueueEmpty() {
	int i;
    for (i = 0; i < SIZE; i++) {
        if(queue[i] != 0) {
            return 0;
        }
    }
    return 1;
}

void enqueue(int num) {
    queue[enqueue_index] = num;
    enqueue_index++;
}

int dequeue() {
    int num = queue[dequeue_index];
    queue[dequeue_index] = 0;
    dequeue_index++;
    return num;
}

//Visited array
void markVisited(int num){
        visited[visited_index] = num;
        visited_index++;
    }

//Check neighbours

void getNeighbours(int index) {
    int neighbours_index = 0;
	int i;

    for (i = 0; i < 4; i++) {
        neighbours[i] = -1;
    }

    //Left
    if(!(index % 7 == 0)) {
        neighbours[neighbours_index] = index - 1;
        neighbours_index++;
    }

    //Right
    if(!((index + 1) % 7 == 0)) {
        neighbours[neighbours_index] = index + 1;
        neighbours_index++;
    }

    //Top
    if (index > 7) {
        neighbours[neighbours_index] = index - 7;
        neighbours_index++;
    }

    //Bottom
    if (index < SIZE - 7 ) {
        neighbours[neighbours_index] = index + 7;
    }
}

//Check if Array Contains.

int contains(int* array, int element, int array_length) {
	int i;
    for (i = 0; i < array_length; i++) {
        if (array[i] == element) {
            return 1;
        }
    }
    return 0;
}

int mapContainsF() {
	int i;
    for (i = 0; i < SIZE; i++) {
        if (map[i] == 'F') {
            return 1;
        }
    }
    return 0;
}

//Clear arrays
void clearArray(int* array, int size_of_array){
	int i;
    for (i = 0; i < size_of_array; i++) {
        array[i] = 0;
    }
}

//BFS
int BFS(int startNode) {
    clearArray(queue, SIZE);
    clearArray(visited, SIZE);
    clearArray(parent, SIZE);
    clearArray(pre_path, SIZE);
    clearArray(path, SIZE);

    parent[startNode] = -1;
    enqueue(startNode);
    markVisited(startNode);

    while(!isQueueEmpty()) {
		int i;
        int current = dequeue();

        if (map[current] == 'F') {
            //Shortest path found, reconstruct the path.
            //Remove F from map.
			FToDelete = current;

            reconstructPath(parent, startNode, current);
            clearArray(visited, SIZE);
            enqueue_index = 0;
            dequeue_index = 0;
            visited_index = 0;
            return 0;
        }

        getNeighbours(current);
        for (i = 0; i < 4; i++) {
            if (!(contains(visited, neighbours[i], SIZE)) && neighbours[i] != -1) {
                if ((map[neighbours[i]] != 'x')) {
                    markVisited(neighbours[i]);
                    parent[neighbours[i]] = current;
                    enqueue(neighbours[i]);
                }
            }
        }
    }
    //Route Not Found.
    return 1;
}

void reconstructPath(int* parent, int startNode, int destinationNode){
	int current = destinationNode;
    int i = 0;
	int j;
    clearArray(pre_path, SIZE);
    
    while(current != startNode) {
        pre_path[i] = current;
        i++;
        current = parent[current];
    }

    pre_path[i] = startNode;

    //remove zeros from array

    
    for (j = 0; j < SIZE; j++) {
        if (pre_path[j] == 0) {
            break;
        }
    }

    for (i = 0; i < j; i++) {
        path[i] = pre_path[i];
    }

    reserveArray(path);
}

void reserveArray(int* array) {
    int i = 0;
    int array_length = SIZE;
    int start = 0;
    int end = array_length - 1;

    while (start < end) {
        //Swap elements at start and end indices
        int temp = array[start];
        array[start] = array[end];
        array[end] = temp;
        start++;
        end--;
    }

    //move elements left

    while (array[0] == 0) {
        for (i = 0; i < array_length; i++) {
            if (array[i] != 0) {
            array[i - 1] = array[i];
            array[i] = 0;
            }
        }
    }
}

void pathToInstructionsToF() {
    int i;
    char new_direction = 'N';
    char old_direction = '?';
    
    for (i = 0; path[i] != 0; i++) {

        old_direction = new_direction;
        //North
        if(path[i] - 7 == path[i + 1]) {
            new_direction = 'N';
        }
        //South
        if(path[i] + 7 == path[i + 1]) {
            new_direction = 'S';
        }
        //West
        if(path[i] - 1 == path[i + 1]) {
            new_direction = 'W';
        }
        //East
        if(path[i] + 1 == path[i + 1]) {
            new_direction = 'E';
        }

        //Check every possible direction 
        if (old_direction == new_direction) {
            instructions[i] = 'F';
        } else {

            //North
            if(old_direction == 'N') {
                if (new_direction == 'W') {
                    instructions[i] = 'L';
                } else {
                    instructions[i] = 'R';
                }
            }

            //South
            if(old_direction == 'S') {
                if (new_direction == 'E') {
                    instructions[i] = 'L';
                } else {
                    instructions[i] = 'R';
                }
            }

            //West
            if(old_direction == 'W') {
                if (new_direction == 'S') {
                    instructions[i] = 'L';
                } else {
                    instructions[i] = 'R';
                }
            }

            //East
            if(old_direction == 'E') {
                if (new_direction == 'N') {
                    instructions[i] = 'L';
                } else {
                    instructions[i] = 'R';
                }
            }
        }
    }
    instructions[i-1] = '\0';
}

void driveBackInstructions() {
    int i = 0;
    int array_length = SIZE;
    int start = 0;
    int end = array_length - 1;

    while (start < end) {
        //Swap elements at start and end indices
        int temp = instructions[start];
        instructions[start] = instructions[end];
        instructions[end] = temp;
        start++;
        end--;
    }

    //move elements left

    while (instructions[0] == '\0') {
        for (i = 0; i < array_length; i++) {
            if (instructions[i] != 0) {
            instructions[i - 1] = instructions[i];
            instructions[i] = '\0';
            }
        }
    }
    //"if (i = 0)" because we don't need to overwrite first instruction, unless it's 'F'. After the delivery, the robot will drive backwards.
    for (i = 0; instructions[i] != '\0'; i++) {
		if (i == 0) {
			if (instructions[i] == 'F') {
				instructions[i] = 'U';
			}
		} else if (instructions[i] == 'L') {
            instructions[i] = 'R';
        } else if (instructions[i] == 'R') {
            instructions[i] = 'L';
        }
    }
}