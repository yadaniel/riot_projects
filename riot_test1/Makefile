# name of your application
APPLICATION = riot_test1

# If no BOARD is found in the environment, use this default:
BOARD ?= native

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
CFLAGS += -DDEVELHELP
CFLAGS=-DAUTO_INIT_LED0

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

USEMODULE += xtimer

FEATURES_REQUIRED += periph_timer

include $(RIOTBASE)/Makefile.include
