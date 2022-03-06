#define protected public
#define private public
#include "FFTGAMClassInfo.h"
#include "ObjectRegistryItem.h"
#include "ClassStructure.h"
#include "ObjectMacros.h"
static ClassStructureEntry FFTGAMOutputStructure_fftRealArray_CSE_EL("float","",100000,0,0,0,0 ,"fftRealArray",msizeof(FFTGAMOutputStructure,fftRealArray),indexof(FFTGAMOutputStructure,fftRealArray));
static ClassStructureEntry FFTGAMOutputStructure_fftImagArray_CSE_EL("float","",100000,0,0,0,0 ,"fftImagArray",msizeof(FFTGAMOutputStructure,fftImagArray),indexof(FFTGAMOutputStructure,fftImagArray));
static ClassStructureEntry * FFTGAMOutputStructure__CSE__[] = {
    &FFTGAMOutputStructure_fftRealArray_CSE_EL,
    &FFTGAMOutputStructure_fftImagArray_CSE_EL,
    NULL
};
ClassStructure FFTGAMOutputStructure__CS__("FFTGAMOutputStructure",sizeof(FFTGAMOutputStructure),0 ,FFTGAMOutputStructure__CSE__);
STRUCTREGISTER("FFTGAMOutputStructure",FFTGAMOutputStructure__CS__)
static ClassStructureEntry FFTGAMInputStructure_signal_CSE_EL("float","",0,0,0,0,0 ,"signal",msizeof(FFTGAMInputStructure,signal),indexof(FFTGAMInputStructure,signal));
static ClassStructureEntry * FFTGAMInputStructure__CSE__[] = {
    &FFTGAMInputStructure_signal_CSE_EL,
    NULL
};
ClassStructure FFTGAMInputStructure__CS__("FFTGAMInputStructure",sizeof(FFTGAMInputStructure),0 ,FFTGAMInputStructure__CSE__);
STRUCTREGISTER("FFTGAMInputStructure",FFTGAMInputStructure__CS__)
static ClassStructureEntry FFTGAMClassInfo_input_CSE_EL("FFTGAMInputStructure","",0,0,0,0,0 ,"input",msizeof(FFTGAMClassInfo,input),indexof(FFTGAMClassInfo,input));
static ClassStructureEntry FFTGAMClassInfo_output_CSE_EL("FFTGAMOutputStructure","",0,0,0,0,0 ,"output",msizeof(FFTGAMClassInfo,output),indexof(FFTGAMClassInfo,output));
static ClassStructureEntry * FFTGAMClassInfo__CSE__[] = {
    &FFTGAMClassInfo_input_CSE_EL,
    &FFTGAMClassInfo_output_CSE_EL,
    NULL
};
ClassStructure FFTGAMClassInfo__CS__("FFTGAMClassInfo",sizeof(FFTGAMClassInfo),0 ,FFTGAMClassInfo__CSE__);
STRUCTREGISTER("FFTGAMClassInfo",FFTGAMClassInfo__CS__)
ClassStructure * FFTGAMClassInfo_sinfo[] = {
    &FFTGAMOutputStructure__CS__,
    &FFTGAMInputStructure__CS__,
    &FFTGAMClassInfo__CS__,
    NULL
};
