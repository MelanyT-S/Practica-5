TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cpp \
    Particula.cpp \
    Obstaculo.cpp \
    gestordearchivo.cpp \
    Simulador.cpp

HEADERS += \
    Particula.h \
    Obstaculo.h \
    gestordearchivo.h \
    Simulador.h