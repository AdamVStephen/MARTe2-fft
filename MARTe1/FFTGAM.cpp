//******************************************************************************
//
//      $Log: FFTGAM.cpp,v $
//      Revision 1.14  2011/01/05 13:38:07  dalves
//      Added the FFTContainer and made output signals optional
//
//      Revision 1.13  2010/12/22 11:00:06  dalves
//      Using NumberOfChildren() instead of BufferWordSize().
//
//      Revision 1.12  2010/12/22 10:19:23  dalves
//      Output signals are no longer mandatory.
//
//      Revision 1.11  2010/12/22 09:27:11  dalves
//      Rewritten using a new approach to allow input and vectors of samples.
//      Cint is no longer required.
//
//      Revision 1.10  2010/12/13 08:58:55  dalves
//      Corrected error message
//
//      Revision 1.9  2010/12/10 15:43:32  dalves
//      Added HttpRefreshMsecTime cfg parameter
//
//      Revision 1.8  2010/12/10 15:27:58  dalves
//      Minor changes
//
//      Revision 1.7  2010/12/10 13:52:59  dalves
//      A non-flickering working version
//
//      Revision 1.6  2010/12/07 17:04:08  dalves
//      Minor changes
//
//      Revision 1.5  2010/12/06 11:22:26  dalves
//      Added graphics in ProcessHttpMessage()
//
//      Revision 1.4  2010/12/02 14:58:10  dalves
//      Added a cfg parameter for the number of overlaping points
//
//      Revision 1.3  2010/12/02 14:34:37  dalves
//      Added a limit to the number of points in the FFT
//
//      Revision 1.2  2010/12/02 12:04:00  dalves
//      Minor changes
//
//      Revision 1.1  2010/12/02 11:42:22  dalves
//      Initial version
//
//
//******************************************************************************

#include "FFTGAM.h"

bool FFTGAM::Initialise(ConfigurationDataBase& cdbData) {

    CDBExtended cdb(cdbData);

    if((numberOfFFTs = Size()) <= 0) {
	AssertErrorCondition(InitialisationError, "FFTGAM::Initialise: %s number of FFTs <= 0", Name());
	return False;
    }

    if((gcrtfc = new GCRTemplate<FFTContainer>[numberOfFFTs]) == NULL) {
	AssertErrorCondition(InitialisationError, "FFTGAM::Initialise: %s unable to allocate memory for FFTContainer references", Name());
	return False;
    }

    for(int i = 0 ; i < numberOfFFTs ; i++) {
	GCRTemplate<FFTContainer> gcrtfcAux = Find(i);
	if(!gcrtfcAux.IsValid()) {
	    AssertErrorCondition(InitialisationError, "FFTGAM::Initialise: %s only FFTContainer objects allowed", Name());
	    return False;
	} else {
	    gcrtfc[i] = gcrtfcAux;
	}
    }
    
    return True;
}

bool FFTGAM::Execute(GAM_FunctionNumbers functionNumber) {
    
    for(int i = 0 ; i < numberOfFFTs ; i++) {
	gcrtfc[i]->Execute(functionNumber);
    }
    
    return True;
}
OBJECTLOADREGISTER(FFTGAM, "$Id: FFTGAM.cpp,v 1.14 2011/01/05 13:38:07 dalves Exp $")
