# $Id: Makefile.inc,v 1.4 2015/09/07 14:49:50 dkarkin Exp $

OBJSX=FFTContainer.x

MAKEDEFAULTDIR=../../../MakeDefaults

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

CPPFLAGS+= -I.
CPPFLAGS+= -I../..
CPPFLAGS+= -I../../../BaseLib2/Level0
CPPFLAGS+= -I../../../BaseLib2/Level1
CPPFLAGS+= -I../../../BaseLib2/Level2
CPPFLAGS+= -I../../../BaseLib2/Level3
CPPFLAGS+= -I../../../BaseLib2/Level4
CPPFLAGS+= -I../../../BaseLib2/Level5
CPPFLAGS+= -I../../../BaseLib2/Level6
CPPFLAGS+= -I../../../BaseLib2/LoggerService

#all: $(OBJS) \
#    $(TARGET)/FFTGAM$(GAMEXT)

all: $(OBJS) \
     $(TARGET)/FFTGAM$(GAMEXT)

include depends.$(TARGET)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)
