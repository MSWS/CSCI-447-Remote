/*
 *	PageSim.h
 *
 *	Header file for CSCI 447 memory management exercise
 *
 *	Filip Jagodzinski, Computer Science, WWU, 21 February 2018
 */

void Terminate(int pid);	// inform student code that the process has terminated

int Access(int pid, int address, int write);	
					// process pid wants to access address
					// write access if write is non-zero
					// return 0 indicates process request not satisfied
