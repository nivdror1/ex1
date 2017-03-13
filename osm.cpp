
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <libltdl/lt_system.h>
#include "osm.h"

#define MAX_MACHINE_NAME_CHAR 255
#define SECONDS_TO_NANO_SECONDS 1000000000
#define MICRO_SECONDS_TO_NANO_SECONDS 1000
#define FAILURE -1
#define DEFAULT_ITERATIONS 1000
#define NUM_ARITHMETIC_OPERATIONS 8

/**
 * convert to nano second
 * @param before the time before the operation
 * @param after the time after the operation
 * @return return the converted time
 */
inline double conversionToNanoSecond(timeval &before, timeval &after){
	return ((after.tv_sec-before.tv_sec)*SECONDS_TO_NANO_SECONDS)
	       +((after.tv_usec- before.tv_usec)*MICRO_SECONDS_TO_NANO_SECONDS);
}

inline unsigned int IsIterationsValid(unsigned int iterations){
	iterations==0?DEFAULT_ITERATIONS:iterations;
}

/** Initialization function that the user must call
 * before running any other library function.
 * The function may, for example, allocate memory or
 * create/open files.
 * Pay attention: this function may be empty for some desings. It's fine.
 * Returns 0 upon success and -1 on failure
 */
int osm_init(){
}


/** finalizer function that the user must call
 * after running any other library function.
 * The function may, for example, free memory or1000
 * close/delete files.
 * Returns 0 upon success and -1 on failure
 */
int osm_finalizer(){
}


/** Time measurement function for a simple arithmetic operation.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_operation_time(unsigned int iterations){
	struct timeval before, after;
	int a,b,c,d;
	double assignmentTime, arithmeticTime,time, roundUp;

	roundUp =  NUM_ARITHMETIC_OPERATIONS- (iterations % NUM_ARITHMETIC_OPERATIONS);

//	//get the time of a assignment
//	gettimeofday(&begin1,NULL);
//	for(int i=0;i<iterations+roundUp;i+=8){
//		a= 1;
//		b=2;
//		c=3;
//		d=4;
//		a= 1;
//		b=2;
//		c=3;
//		d=4;
//	}
//	gettimeofday(&after1,NULL);
//	assignmentTime= ((after.tv_sec-begin.tv_sec)*1000000000) +((after.tv_usec- begin.tv_usec)*1000);

	//get the time of a arithmetic operation
	gettimeofday(&before,NULL);
	for(int i=0;i<iterations+roundUp;i+=NUM_ARITHMETIC_OPERATIONS){
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
	arithmeticTime= conversionToNanoSecond(before,after);
	//calculate the time for the actual arithmetic operations
	time =arithmeticTime;
	return time/(iterations+roundUp);
}

/**
 * a function filled with arithmetic operation
 */
void emptyFun(){
	int a,b,c,d;
	a= 5+1;
	b=2-7;
	c=4+8;
	d=7-4;
}
/**
 * return the time the arithmetic operation in emptyFun
 * @return the time the arithmetic operation in emptyFun
 */
double timeInFunc(){
	struct timeval before, after;
	gettimeofday(&before,NULL);
	int a,b,c,d;
	a= 5+1;
	b=2-7;
	c=4+8;
	d=7-4;
	gettimeofday(&after,NULL);
	return conversionToNanoSecond(before, after);

}


/** Time measurement function for an empty function call.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_function_time(unsigned int iterations){
	struct timeval before, after;
	gettimeofday(&before,NULL);
	for(int i=0;i<iterations;i++){
		emptyFun();
	}
	gettimeofday(&after,NULL);

	double time= conversionToNanoSecond(before, after);
	return (time/iterations)-timeInFunc();
}


/** Time measurement function for an empty trap into the operating system.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_syscall_time(unsigned int iterations){
	struct timeval before, after;
	gettimeofday(&before,NULL);
	//calling the trap system call
	for(int i=0;i<iterations;i++){
		OSM_NULLSYSCALL;
	}
	gettimeofday(&after,NULL);
	double time= conversionToNanoSecond(before,after);
	return time/iterations;
}

/** Time measurement function for accessing the disk.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_disk_time(unsigned int iterations){
	struct timeval before, after;
	//open a file
	std::ofstream file;
	file.exceptions(std::fstream::failbit|std::fstream::badbit);

	try {
		file.open("temp");
		//writing nonsense
		gettimeofday(&before, NULL);
		for (int i = 0; i < iterations; i++) {
			file << i << std::flush; //flushing the disk access
		}
		gettimeofday(&after, NULL);

		//closing and removing
		file.close();
	}catch(std::fstream::failure &e){
		std::cout<<"exception opening|closing|writing file"<<std::endl;
	}
	if(remove("temp")!=0){
		std::cout<<"error trying to delete a file"<<std::endl;
	}

	double time = conversionToNanoSecond(before,after);
	return time/iterations;
}
/**
 * assign value to the struct data member to the library functions
 * @param operation_iterations the number of operation iterations
 * @param function_iterations the number of empty functions iterations
 * @param syscall_iterations the number of syscall iterations
 * @param disk_iterations the number of disk iterations
 * @return a instance of timeMeasurmentStructure
 */
timeMeasurmentStructure measureTimes (unsigned int operation_iterations,
                                      unsigned int function_iterations,
                                      unsigned int syscall_iterations,
                                      unsigned int disk_iterations){
	//check the iterations input
	operation_iterations= IsIterationsValid(operation_iterations);
	function_iterations= IsIterationsValid(function_iterations);
	syscall_iterations= IsIterationsValid(syscall_iterations);
	disk_iterations= IsIterationsValid(disk_iterations);

	timeMeasurmentStructure time;
	time.machineName[MAX_MACHINE_NAME_CHAR];
	if (gethostname(time.machineName, MAX_MACHINE_NAME_CHAR-1) == FAILURE){
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

