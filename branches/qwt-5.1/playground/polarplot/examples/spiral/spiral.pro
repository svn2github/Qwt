# -*- mode: sh -*- ################################################
# Qwt Widget Library
# Copyright (C) 1997   Josef Wilgen
# Copyright (C) 2002   Uwe Rathmann
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Qwt License, Version 1.0
###################################################################

TEMPLATE     = app

MOC_DIR      = moc
OBJECTS_DIR  = obj

INCLUDEPATH += ../../src
DEPENDPATH  += ../../src
INCLUDEPATH += ../../../../src
DEPENDPATH  += ../../../../src

LIBS        += -L../../lib -lpolarplot
LIBS        += -L../../../../lib -lqwt

TARGET       = spiral

SOURCES = \
	spiral.cpp

