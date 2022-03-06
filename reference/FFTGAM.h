//******************************************************************************
//
//      $Log: FFTGAM.h,v $
//      Revision 1.8  2011/01/05 13:38:08  dalves
//      Added the FFTContainer and made output signals optional
//
//      Revision 1.7  2010/12/22 10:19:37  dalves
//      Output signals are no longer mandatory.
//
//      Revision 1.6  2010/12/22 09:28:04  dalves
//      Rewritten using a new approach to allow input and vectors of samples.
//      Cint is no longer required.
//
//      Revision 1.5  2010/12/10 15:43:32  dalves
//      Added HttpRefreshMsecTime cfg parameter
//
//      Revision 1.4  2010/12/10 13:52:59  dalves
//      A non-flickering working version
//
//      Revision 1.3  2010/12/07 17:04:08  dalves
//      Minor changes
//
//      Revision 1.2  2010/12/02 14:58:10  dalves
//      Added a cfg parameter for the number of overlaping points
//
//      Revision 1.1  2010/12/02 11:42:22  dalves
//      Initial version
//
//
//******************************************************************************

#ifndef FFT_H_
#define FFT_H_

#include "FFTContainer.h"
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
OBJECT_DLL(FFTGAM)
class FFTGAM : public GAM {
OBJECT_DLL_STUFF(FFTGAM)

// DDB Interfaces
private:

    uint32                      numberOfFFTs;

    GCRTemplate<FFTContainer>  *gcrtfc;

public:
    /** Constructor */
    FFTGAM() {
	numberOfFFTs = 0;
	gcrtfc       = NULL;
    }

    /** Destructor */
    ~FFTGAM(){
	if(gcrtfc != NULL) {
	    delete gcrtfc;
	    gcrtfc = NULL;
	}
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
};
#endif /* FFT_H_ */
