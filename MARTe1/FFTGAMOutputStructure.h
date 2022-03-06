//******************************************************************************
//
//      $Log: FFTGAMOutputStructure.h,v $
//      Revision 1.2  2010/12/02 14:34:37  dalves
//      Added a limit to the number of points in the FFT
//
//      Revision 1.1  2010/12/02 11:42:22  dalves
//      Initial version
//
//
//******************************************************************************

#ifndef FFTGAMOUTPUTSTRUCTURE_H_
#define FFTGAMOUTPUTSTRUCTURE_H_

#include "System.h"

static const uint32 MAX_N_FFT = 100000;

struct FFTGAMOutputStructure {
    float fftRealArray[MAX_N_FFT];
    float fftImagArray[MAX_N_FFT];
};
#endif /* FFTGAMOUTPUTSTRUCTURE_H_ */
