
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sys/time.h>
#include "osm.h"

#define MAX_MACHINE_NAME_CHAR 255




/* Initialization function that the user must call
 * before running any other library function.
 * The function may, for example, allocate memory or
 * create/open files.
 * Pay attention: this function may be empty for some desings. It's fine.
 * Returns 0 upon success and -1 on failure
 */
int osm_init(){

}


/* finalizer function that the user must call
 * after running any other library function.
 * The function may, for example, free memory or
 * close/delete files.
 * Returns 0 upon success and -1 on failure
 */
int osm_finalizer(){
}


/* Time measurement function for a simple arithmetic operation.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_operation_time(unsigned int iterations){
	struct timeval begin, after,begin1, after1;
	int a,b,c,d;
	double assignmentTime, arithmeticTime,time, roundUp;
	roundUp = iterations % 8;

	//get the time of a assignment
	gettimeofday(&begin1,NULL);
	for(int i=0;i<iterations+roundUp;i+=8){
		a= 1;
		b=2;
		c=3;
		d=4;
		a= 1;
		b=2;
		c=3;
		d=4;
	}
	gettimeofday(&after1,NULL);
	assignmentTime= ((after.tv_sec-begin.tv_sec)*1000000000) +((after.tv_usec- begin.tv_usec)*1000);

	//get the time of a arithmetic operation
	gettimeofday(&begin,NULL);
	for(int i=0;i<iterations+roundUp;i+=8){
		a= 5+1;
		b=2-7;
		c=4+8;
		d=7-4;
		a= 5+1;
		b=2-7;
		c=4+8;
		d=7-4;
	}
	gettimeofday(&after,NULL);
	arithmeticTime= ((after.tv_sec-begin.tv_sec)*1000000000) +((after.tv_usec- begin.tv_usec)*1000);
	//calculate the time for the actual arithmetic operations
	time =arithmeticTime-assignmentTime;
	return time/(iterations);
}


void emptyFun(){
	int a,b,c,d;
	a= 5+1;
	b=2-7;
	c=4+8;
	d=7-4;
}

double timeInFunc(){
	struct timeval begin, after;
	double timeInFunction;
	gettimeofday(&begin,NULL);
	int a,b,c,d;
	a= 5+1;
	b=2-7;
	c=4+8;
	d=7-4;
	gettimeofday(&after,NULL);
	timeInFunction = ((after.tv_sec-begin.tv_sec)*1000000000) +(after.tv_usec- begin.tv_usec)*1000;
	return timeInFunction;
}


/* Time measurement function for an empty function call.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_function_time(unsigned int iterations){
	struct timeval begin, after;
	gettimeofday(&begin,NULL);
	for(int i=0;i<iterations;i++){
		emptyFun();
	}
	gettimeofday(&after,NULL);

	double time= ((after.tv_sec-begin.tv_sec)*1000000000) +(after.tv_usec- begin.tv_usec)*1000;
	return (time/iterations)-timeInFunc();
}


/* Time measurement function for an empty trap into the operating system.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_syscall_time(unsigned int iterations){
	struct timeval begin, after;
	gettimeofday(&begin,NULL);
	for(int i=0;i<iterations;i++){
		OSM_NULLSYSCALL;
	}

	gettimeofday(&after,NULL);
	double time= ((after.tv_sec-begin.tv_sec)*1000000000) +((after.tv_usec- begin.tv_usec)*1000);
	return time/iterations;
}

/* Time measurement function for accessing the disk.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_disk_time(unsigned int iterations){
	struct timeval begin, after;
	gettimeofday(&begin,NULL);
	for(int i=0;i<iterations;i++){
		std::ifstream file;
		file.open("temp" + i);
	}
	gettimeofday(&after,NULL);

	for(int i=0;i<iterations;i++){
		remove("temp" + i);
	}

	double time = ((after.tv_sec-begin.tv_sec)*1000000000) +((after.tv_usec- begin.tv_usec)*1000);
	return time/iterations;
}

timeMeasurmentStructure measureTimes (unsigned int operation_iterations,
                                      unsigned int function_iterations,
                                      unsigned int syscall_iterations,
                                      unsigned int disk_iterations){
	timeMeasurmentStructure time;
	time.machineName[255];
	if (gethostname(time.machineName, 254) == -1){
		std::cout << "Problem" << std::endl;
	}
	time.functionTimeNanoSecond = osm_function_time(function_iterations);
	time.instructionTimeNanoSecond = osm_operation_time(operation_iterations);
	time.trapTimeNanoSecond = osm_syscall_time(syscall_iterations);
	time.diskTimeNanoSecond = osm_disk_time(disk_iterations);
	time.functionInstructionRatio = time.functionTimeNanoSecond/time.instructionTimeNanoSecond;
	time.trapInstructionRatio = time.trapTimeNanoSecond/time.instructionTimeNanoSecond;
	time.diskInstructionRatio = time.diskInstructionRatio/time.instructionTimeNanoSecond;
	return time;
}

