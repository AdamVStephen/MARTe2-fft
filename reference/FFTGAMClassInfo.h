//******************************************************************************
//
//      $Log: FFTGAMClassInfo.h,v $
//      Revision 1.1  2010/12/02 11:42:22  dalves
//      Initial version
//
//
//******************************************************************************

#ifndef FFTGAMCLASSINFO_H_
#define FFTGAMCLASSINFO_H_

#include "System.h"
#include "FFTGAMInputStructure.h"
#include "FFTGAMOutputStructure.h"

struct FFTGAMClassInfo {
    FFTGAMInputStructure  input;
    FFTGAMOutputStructure output;
};
#endif /* FFTGAMCLASSINFO_H_ */
