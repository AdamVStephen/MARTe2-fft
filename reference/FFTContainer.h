//******************************************************************************
//
//      $Log: FFTContainer.h,v $
//      Revision 1.1  2011/01/14 10:07:05  dalves
//      Added the FFTContainer and made output signals optional
//
//
//******************************************************************************

#ifndef FFT_CONTAINER_H_
#define FFT_CONTAINER_H_

#include "GAM.h"
#include "System.h"
#include <fftw3.h>

// these aren't working ... using the standard ones
//void *fftw_malloc(size_t);
//void *fftw_free(void *);

/**
 * A GAM  implementing a FFT controller with antiwindup
 * and saturation with slew rate
 */
OBJECT_DLL(FFTContainer)
class FFTContainer : public GAM, public HttpInterface {
OBJECT_DLL_STUFF(FFTContainer)

// DDB Interfaces
private:
    /** Input interface to read data from */
    DDBInputInterface                      *input;
    /** Output interface to write data to */
    DDBOutputInterface                     *outputReal;
    DDBOutputInterface                     *outputImag;
    DDBOutputInterface                     *outputPow;
    DDBOutputInterface                     *outputPhase;

// Parameters
private:

    int32          NFFT;

    uint32         inputSignalNumberOfPoints;

    int32          NCYCLES;
    
    int32          Fs;

    float          freqRes;

    int32          numberOfTopFreqs2Display;

    int32          httpRefreshMsecTime;

    uint32         delayIndex;

    fftwf_plan     fftPlan;

    uint32         fftFlags;

    float         *in;

    fftwf_complex *out;

private:

    float *yMin;

    float *yMax;

private:

    bool REFRESH;

public:
    /** Constructor */
    FFTContainer() {
        input       = NULL;
        outputReal  = NULL;
        outputImag  = NULL;
        outputPow   = NULL;
        outputPhase = NULL;

	in        = NULL;
	out       = NULL;

	yMin      = NULL;
	yMax      = NULL;

	NFFT      = 0;
	NCYCLES   = 1;
	inputSignalNumberOfPoints = 1;
	Fs        = 0;
	freqRes   = 0;
	numberOfTopFreqs2Display = 10;

	httpRefreshMsecTime = 250;

	delayIndex = 1;

	REFRESH   = True;

	fftFlags  = 0x0;
	fftFlags |= FFTW_ESTIMATE;
    }

    /** Destructor */
    ~FFTContainer(){
	if(in != NULL) {
	    //fftw_free(in);
	    free((void*&)in);
	}
	in = NULL;
	if(out != NULL) {
	    //fftw_free(out);
	    free((void*&)out);
	}
	out = NULL;
	if(yMin != NULL) {
	    free((void*&)yMin);
	}
	yMin = NULL;
	if(yMax != NULL) {
	    free((void*&)yMax);
	}
	yMax = NULL;
    }

    /**
    * Loads GAM parameters from a CDB
    * @param cdbData the CDB
    * @return True if the initialisation went ok, False otherwise
    */
    virtual bool Initialise(ConfigurationDataBase& cdbData);

    /**
    * GAM main body
    * @param functionNumber The current state of MARTe
    * @return False on error, True otherwise
    */
    virtual bool Execute(GAM_FunctionNumbers functionNumber);

    /**
    * Saves parameters to a CDB
    * @param info the CDB to save to
    * @return True
    */
    virtual bool ObjectSaveSetup(ConfigurationDataBase &info, StreamInterface *err){ return True; };

    virtual bool ProcessHttpMessage(HttpStream &hStream);

};
#endif /* FFT_CONTAINER_H_ */
