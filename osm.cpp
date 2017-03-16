
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <cstring>
#include <malloc.h>
#include "osm.h"

#define MAX_MACHINE_NAME_CHAR 255
#define SECONDS_TO_NANO_SECONDS 1000000000
#define MICRO_SECONDS_TO_NANO_SECONDS 1000
#define FAILURE -1
#define DEFAULT_ITERATIONS 1000
#define NUM_OPERATIONS 8
//---------------------- global variables------------------------------
void *buff;
char *machineName;
__blksize_t blockSize;
//----------------------- fuctions------------------------------
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
/**
 * check if the number of iterations is valid if not assign DEFAULT_ITERATIONS
 * @param iterations the number of iterations
 * @return if iteration eqaul zero return DEFAULT_ITERATIONS else return iterations
*/
inline unsigned int IsIterationsValid(unsigned int iterations){
	return iterations==0?DEFAULT_ITERATIONS:iterations;
}

/** Initialization function that the user must call
 * before running any other library function.
 * The function may, for example, allocate memory or
 * create/open files.
 * Pay attention: this function may be empty for some desings. It's fine.
 * Returns 0 upon success and -1 on failure
 */
int osm_init(){
	struct stat stat1;
	//getting the block size
	stat("/tmp",&stat1);
	blockSize=stat1.st_blksize;

	//allocating the buffer for disk accessing function
	buff = aligned_alloc(blockSize,blockSize*sizeof(void*));

	//allocating machineName
	machineName=(char *)malloc(MAX_MACHINE_NAME_CHAR);
	if((buff==NULL)|(machineName==NULL)){
		return FAILURE;
	}
	return 0;
}


/** finalizer function that the user must call
 * after running any other library function.
 * The function may, for example, free memory or1000
 * close/delete files.
 * Returns 0 upon success and -1 on failure
 */
int osm_finalizer(){
	free(buff);
	free(machineName);
	return 0;
}


/** Time measurement function for a simple arithmetic operation.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_operation_time(unsigned int iterations){
	struct timeval before, after;
	double  arithmeticTime,time, roundUp;
	int beforeStatus, afterStatus;
	int a,b,c,d,e,f,g,h;
	roundUp =  NUM_OPERATIONS- (iterations % NUM_OPERATIONS);


	//get the time of a arithmetic operation
	beforeStatus = gettimeofday(&before,NULL);
	for(unsigned int i=0;i<iterations+roundUp;i+=NUM_OPERATIONS){
		a= 5+1;
		b=2-7;
		c=4+8;
		d=7-4;
		e= 5+1;
		f=2-7;
		g=4+8;
		h=7-4;
	}
	afterStatus= gettimeofday(&after,NULL);
	//just avoiding nonsense warning
	if(a+b+c+d+e+f+g+h==0){};
	if ((beforeStatus|afterStatus)==FAILURE){ //check for a failure in accessing the time
		return FAILURE;
	}
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
	//just avoiding nonsense warning
	if(a+b+c+d==0){};
}
/**
 * return the time the arithmetic operation in emptyFun
 * @return the time the arithmetic operation in emptyFun
 */
double timeInFunc(){
	struct timeval before, after;
	int beforeStatus, afterStatus;
	beforeStatus = gettimeofday(&before,NULL);
	int a,b,c,d;
	a= 5+1;
	b=2-7;
	c=4+8;
	d=7-4;
	//just avoiding nonsense warning
	if(a+b+c+d==0){};
	afterStatus = gettimeofday(&after,NULL);

	if ((beforeStatus|afterStatus)==-1){ //check for a failure in accessing the time
		return FAILURE;
	}
	return conversionToNanoSecond(before, after);

}


/** Time measurement function for an empty function call.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_function_time(unsigned int iterations){
	struct timeval before, after;
	int beforeStatus, afterStatus;
	double funcStatus;
	int roundUp =  NUM_OPERATIONS- (iterations % NUM_OPERATIONS);
	beforeStatus = gettimeofday(&before,NULL);
	for(unsigned int i=0;i<iterations+roundUp;i+=NUM_OPERATIONS){
		emptyFun();
		emptyFun();
		emptyFun();
		emptyFun();
		emptyFun();
		emptyFun();
		emptyFun();
		emptyFun();
	}
	afterStatus= gettimeofday(&after,NULL);

	funcStatus = timeInFunc();
	if (((beforeStatus|afterStatus)==-1)|(funcStatus==-1)){ //check for a failure in accessing the time
		return FAILURE;
	}
	double time= conversionToNanoSecond(before, after);
	return (time/(iterations+roundUp))-funcStatus;
}


/** Time measurement function for an empty trap into the operating system.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_syscall_time(unsigned int iterations){
	struct timeval before, after;
	int beforeStatus, afterStatus;

	int roundUp =  NUM_OPERATIONS- (iterations % NUM_OPERATIONS);
	beforeStatus = gettimeofday(&before,NULL);
	//calling the trap system call
	for(unsigned int i=0;i<iterations+roundUp;i+=NUM_OPERATIONS){
		OSM_NULLSYSCALL;
		OSM_NULLSYSCALL;
		OSM_NULLSYSCALL;
		OSM_NULLSYSCALL;
		OSM_NULLSYSCALL;
		OSM_NULLSYSCALL;
		OSM_NULLSYSCALL;
		OSM_NULLSYSCALL;
	}
	afterStatus = gettimeofday(&after,NULL);

	if ((beforeStatus|afterStatus)==-1){ //check for a failure in accessing the time
		return FAILURE;
	}
	double time= conversionToNanoSecond(before,after);
	return time/(iterations+roundUp);
}

/** Time measurement function for accessing the disk.
   returns time in nano-seconds upon success,
   and -1 upon failure.
   */
double osm_disk_time(unsigned int iterations){
	struct timeval before, after;
	int beforeStatus, afterStatus;

	//open a file
	int fd=open("/tmp/temp.txt",O_DIRECT|O_SYNC|O_WRONLY|O_CREAT);

	if(fd!=FAILURE) {
		//writing nonsense
		beforeStatus = gettimeofday(&before, NULL);
		for (unsigned int i = 0; i < iterations; i++) {
			//writing a single block
			if(write(fd,::buff,blockSize)==FAILURE){
				close(fd);
				remove("/tmp/temp.txt");
				return FAILURE;
			}
		}
		afterStatus = gettimeofday(&after, NULL);

		close(fd);
		remove("/tmp/temp.txt");

		//closing and removing
		if ((beforeStatus | afterStatus) == FAILURE) { //check for a failure in accessing the time
			return FAILURE;
		}
		double time = conversionToNanoSecond(before,after);
		return time/iterations;
	}
	return FAILURE;
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

	time.functionTimeNanoSecond = osm_function_time(function_iterations);
	time.instructionTimeNanoSecond = osm_operation_time(operation_iterations);
	time.trapTimeNanoSecond = osm_syscall_time(syscall_iterations);
	time.diskTimeNanoSecond = osm_disk_time(disk_iterations);
	time.functionInstructionRatio = time.functionTimeNanoSecond/time.instructionTimeNanoSecond;
	time.trapInstructionRatio = time.trapTimeNanoSecond/time.instructionTimeNanoSecond;
	time.diskInstructionRatio = time.diskInstructionRatio/time.instructionTimeNanoSecond;
	if (gethostname(machineName, MAX_MACHINE_NAME_CHAR-1) == FAILURE){
		time.machineName[0]='\0';
	} else{
		time.machineName=machineName;
	}
	return time;
}

