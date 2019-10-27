# name of your application
APPLICATION = riot_test2

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
USEMODULE += app_metadata
USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += ps

RIOT_TERMINAL ?= socat

DISABLE_MODULE += auto_init

# chronos is missing a getchar implementation
BOARD_BLACKLIST += chronos

APP_SHELL_FMT ?= NONE

FEATURES_REQUIRED += periph_timer

include $(RIOTBASE)/Makefile.include
