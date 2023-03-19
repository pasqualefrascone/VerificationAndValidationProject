/*
 * Util.h
 *
 *  Created on: 14 dic 2022
 *      Author: pasquale
 */

#ifndef UTILS_UTIL_H_
#define UTILS_UTIL_H_

#include <sys/time.h> //timeval
#include <time.h>  //strftime
#include <stdio.h> //snprintf

#include <iostream>
#include <string.h>

namespace Util {
	void getTimeStamp(char *dest){


		struct timeval tt;
		gettimeofday(&tt,NULL);
		//unsigned long milli=tt.tv_sec*(unsigned long)1000+tt.tv_usec/1000; //millisecondi
		//unsigned long microseconds_since_epoch = chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now().time_since_epoch()).count();
		char buffer[20];
		strftime(buffer, 20, "%Y-%m-%d %H:%M:%S", localtime(&tt.tv_sec));
		snprintf(dest,27,"%s.%ld",buffer,tt.tv_usec);

	}

	/**
	 * same thing as memset
	 */
	void resetCharBuf(char *in,int sizeOfBuf){
		for(int i=0;i<sizeOfBuf;i++){
			in[i]='\0';
		}
		return;
	}

};

#endif /* UTILS_UTIL_H_ */
